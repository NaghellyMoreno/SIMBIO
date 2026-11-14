// =====================================================================
//  SIMBIO – Tiempo (NTP + respaldo en flash), registro en SD y cola de envío
//
//  * Hora: NTP al tener WiFi. Sin NTP se usa la última hora guardada en flash
//    + el tiempo desde el arranque, y el registro queda marcado "ts_estimada".
//  * Cada registro guarda además t_lote_s (segundos desde el inicio del lote).
//  * SD: un CSV por lote en /lotes/<id>.csv y la cola /cola.txt para la nube.
//    Si no hay SD, la cola vive en RAM (máximo COLA_RAM_MAX líneas).
// =====================================================================
#include "simbio.h"
#include <SPI.h>
#include <SD.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <time.h>
#include <deque>

static Preferences prefT;
static uint32_t epochBase = 0, millisBase = 0;
static uint32_t tGuardarHora = 0, tRegistro = 0, tEstado = 0;
static uint32_t eventoSeq = 0;

static const char *COLA = "/cola.txt";
static uint32_t colaOffset = 0;
static volatile uint32_t pendientes = 0;
static std::deque<String> colaRam;
static size_t peekLineas = 0;
static uint32_t peekBytes = 0;
static bool peekDesdeSd = false;
static uint32_t perdidosRam = 0;

// ---------------------------------------------------------------- Tiempo
void tiempoInicio() {
  configTzTime(TZ_POSIX, NTP_1, NTP_2);
  prefT.begin("tiempo", false);
  epochBase = prefT.getULong("epoch", 0);
  millisBase = millis();
  E.boots = prefT.getULong("boots", 0) + 1;
  prefT.putULong("boots", E.boots);
}

uint32_t epochAhora(bool &estimada) {
  time_t t = time(nullptr);
  if ((uint32_t)t > EPOCH_VALIDO) {
    estimada = false;
    if (!E.horaNtp) {
      E.horaNtp = true;
      char b[24];
      fmtLocal((uint32_t)t, b, sizeof(b), "%Y-%m-%d %H:%M:%S");
      Serial.printf("[HORA] Sincronizada por NTP: %s\n", b);
    }
    return (uint32_t)t;
  }
  estimada = true;
  if (epochBase > 0) return epochBase + (millis() - millisBase) / 1000;
  return 0;
}

void tiempoActualizar() {
  if (millis() - tGuardarHora < (uint32_t)TIEMPO_GUARDAR_S * 1000UL) return;
  tGuardarHora = millis();
  bool est;
  uint32_t e = epochAhora(est);
  if (e > 0) prefT.putULong("epoch", e);
}

void fmtIsoUtc(uint32_t epoch, char *buf, size_t n) {
  time_t t = epoch;
  struct tm tmv;
  gmtime_r(&t, &tmv);
  strftime(buf, n, "%Y-%m-%dT%H:%M:%SZ", &tmv);
}

void fmtLocal(uint32_t epoch, char *buf, size_t n, const char *formato) {
  time_t t = epoch;
  struct tm tmv;
  localtime_r(&t, &tmv);
  strftime(buf, n, formato, &tmv);
}

// ---------------------------------------------------------------- Cola
static void colaContar() {
  pendientes = colaRam.size();
  if (!E.sdOk || !SD.exists(COLA)) return;
  File f = SD.open(COLA, FILE_READ);
  if (!f) return;
  uint32_t n = 0;
  while (f.available()) if (f.read() == '\n') n++;
  f.close();
  pendientes += n;
}

static void colaAgregar(char tipo, const String &json) {
  SpiLock l;
  if (E.sdOk) {
    File f = SD.open(COLA, FILE_APPEND);
    if (f) {
      f.print(tipo);
      f.print('\t');
      f.print(json);
      f.print('\n');
      f.close();
      pendientes++;
      return;
    }
    E.sdOk = false;
    Serial.println("[SD] Error de escritura: la cola pasa a memoria RAM");
  }
  if (colaRam.size() >= COLA_RAM_MAX) {
    colaRam.pop_front();
    if (pendientes) pendientes--;
    perdidosRam++;
  }
  colaRam.push_back(String(tipo) + "\t" + json);
  pendientes++;
}

uint32_t colaPendientes() { return pendientes; }

// Junta hasta maxLineas registros consecutivos del mismo tipo en un arreglo JSON.
// Los registros de lotes ('O') van de a uno, porque cada uno trae columnas distintas.
static bool agregarLinea(const String &ln, char &tipo, String &cuerpo, size_t maxL) {
  char t = ln[0];
  if (peekLineas == 0) tipo = t;
  else if (t != tipo || tipo == 'O' || peekLineas >= maxL) return false;
  if (peekLineas) cuerpo += ',';
  cuerpo += ln.substring(2);
  peekLineas++;
  return true;
}

bool colaPeek(char &tipo, String &cuerpo, size_t maxL) {
  SpiLock l;
  peekLineas = 0;
  peekBytes = 0;
  cuerpo = "";
  peekDesdeSd = false;

  if (E.sdOk && SD.exists(COLA)) {
    File f = SD.open(COLA, FILE_READ);
    if (f) {
      if (colaOffset >= f.size()) {
        f.close();
        SD.remove(COLA);
        colaOffset = 0;
      } else {
        f.seek(colaOffset);
        uint32_t vacios = 0;
        while (f.available()) {
          uint32_t pos = f.position();
          String ln = f.readStringUntil('\n');
          uint32_t usados = f.position() - pos;
          ln.trim();
          if (ln.length() < 3 || ln[1] != '\t') {        // línea vacía o dañada
            if (peekLineas == 0) vacios += usados;
            else break;
            continue;
          }
          if (!agregarLinea(ln, tipo, cuerpo, maxL)) break;
          peekBytes += usados;
        }
        f.close();
        if (peekLineas > 0) {
          peekBytes += vacios;
          peekDesdeSd = true;
          cuerpo = "[" + cuerpo + "]";
          return true;
        }
        colaOffset += vacios;
        return false;
      }
    }
  }
  for (auto &ln : colaRam) {
    if (!agregarLinea(ln, tipo, cuerpo, maxL)) break;
  }
  if (peekLineas == 0) return false;
  cuerpo = "[" + cuerpo + "]";
  return true;
}

void colaConsumir() {
  SpiLock l;
  if (peekDesdeSd) {
    colaOffset += peekBytes;
    File f = SD.open(COLA, FILE_READ);
    bool vacia = !f || colaOffset >= f.size();
    if (f) f.close();
    if (vacia) {
      SD.remove(COLA);
      colaOffset = 0;
    }
  } else {
    for (size_t i = 0; i < peekLineas && !colaRam.empty(); i++) colaRam.pop_front();
  }
  pendientes = (pendientes > peekLineas) ? pendientes - peekLineas : 0;
  peekLineas = 0;
}

// ---------------------------------------------------------------- SD / CSV
static String rutaCsv() { return String("/lotes/") + E.lote.id + ".csv"; }

void registroInicio() {
  SpiLock l;
  E.sdOk = SD.begin(PIN_SD_CS, SPI, 4000000);
  if (E.sdOk) {
    if (!SD.exists("/lotes")) SD.mkdir("/lotes");
    Serial.printf("[SD] OK, %llu MB\n", SD.cardSize() / (1024ULL * 1024ULL));
  } else {
    Serial.println("[SD] No disponible: los datos se guardan en RAM hasta poder enviarlos");
  }
  colaContar();
  if (pendientes) Serial.printf("[SD] %lu registros pendientes de enviar\n", (unsigned long)pendientes);
}

void registroAbrirLote() {
  SpiLock l;
  if (!E.sdOk) return;
  String ruta = rutaCsv();
  if (SD.exists(ruta)) return;
  File f = SD.open(ruta, FILE_WRITE);
  if (!f) { E.sdOk = false; return; }
  f.print("fecha_hora,hora_estimada,t_lote_s,ph,ph_mv,temp_liquido_c,temp_amb_c,hum_amb_pct,"
          "nivel_cm,volumen_l,gas_idx,bomba_aire,calentador,valvula,aire_min,dosis,setpoint_c,alarmas\n");
  f.close();
}

static float r2(float v) { return roundf(v * 100.0f) / 100.0f; }

static void ponerNum(JsonDocument &d, const char *k, float v, bool ok) {
  if (ok && !isnan(v)) d[k] = r2(v);
  else d[k] = nullptr;
}

void registrarAhora() {
  if (!E.lote.activo) return;
  bool est;
  uint32_t ep = epochAhora(est);
  uint32_t ls = loteSegundos();
  const Lecturas &L = E.lec;

  // ---- JSON para Supabase (tabla lecturas): siempre las mismas columnas ----
  JsonDocument d;
  d["lote_id"] = E.lote.id;
  d["t_lote_s"] = ls;
  char iso[24] = "";
  if (ep > 0) { fmtIsoUtc(ep, iso, sizeof(iso)); d["ts"] = iso; }
  else d["ts"] = nullptr;
  d["ts_estimada"] = est;
  ponerNum(d, "ph", L.ph, L.okPh);
  ponerNum(d, "ph_mv", L.phMv, L.okPh);
  ponerNum(d, "temp_liquido", L.tempLiq, L.okTemp);
  ponerNum(d, "temp_ambiente", L.tempAmb, L.okDht);
  ponerNum(d, "humedad_ambiente", L.humAmb, L.okDht);
  ponerNum(d, "nivel_cm", L.nivelCm, L.okNivel);
  ponerNum(d, "volumen_l", L.volumenL, L.okNivel);
  ponerNum(d, "gas_indice", L.gasIdx, L.okGas);
  d["bomba_aire"] = E.sal.aire;
  d["calentador"] = E.sal.calentador;
  d["valvula_melaza"] = E.sal.valvula;
  d["aire_min_acum"] = r2(E.lote.aireS / 60.0f);
  d["dosis_acum"] = E.lote.dosis;
  d["setpoint"] = r2(E.setpoint);
  d["alarmas"] = E.alarmas;
  d["semaforo"] = textoSemaforo();
  String json;
  serializeJson(d, json);
  colaAgregar('L', json);

  // ---- CSV local ----
  SpiLock l;
  if (!E.sdOk) return;
  File f = SD.open(rutaCsv(), FILE_APPEND);
  if (!f) { E.sdOk = false; return; }
  char fecha[24] = "";
  if (ep > 0) fmtLocal(ep, fecha, sizeof(fecha), "%Y-%m-%d %H:%M:%S");
  auto num = [&](float v, bool ok, int dec) { if (ok && !isnan(v)) f.print(v, dec); f.print(','); };
  f.print(fecha); f.print(',');
  f.print(est ? 1 : 0); f.print(',');
  f.print(ls); f.print(',');
  num(L.ph, L.okPh, 2);
  num(L.phMv, L.okPh, 0);
  num(L.tempLiq, L.okTemp, 2);
  num(L.tempAmb, L.okDht, 1);
  num(L.humAmb, L.okDht, 0);
  num(L.nivelCm, L.okNivel, 1);
  num(L.volumenL, L.okNivel, 2);
  num(L.gasIdx, L.okGas, 1);
  f.print(E.sal.aire ? 1 : 0); f.print(',');
  f.print(E.sal.calentador ? 1 : 0); f.print(',');
  f.print(E.sal.valvula ? 1 : 0); f.print(',');
  f.print(E.lote.aireS / 60.0f, 2); f.print(',');
  f.print(E.lote.dosis); f.print(',');
  f.print(E.setpoint, 1); f.print(',');
  f.print('"'); f.print(textoAlarmas(E.alarmas)); f.print('"');
  f.print('\n');
  f.close();
}

void evento(const char *tipo, const String &detalle) {
  bool est;
  uint32_t ep = epochAhora(est);
  Serial.printf("[EVENTO] %s: %s\n", tipo, detalle.c_str());
  JsonDocument d;
  d["uid_evento"] = String(DISPOSITIVO_ID) + "-" + String(E.boots) + "-" + String(++eventoSeq);
  if (E.lote.activo) d["lote_id"] = E.lote.id;
  else d["lote_id"] = nullptr;
  d["t_lote_s"] = E.lote.activo ? loteSegundos() : 0;
  char iso[24] = "";
  if (ep > 0) { fmtIsoUtc(ep, iso, sizeof(iso)); d["ts"] = iso; }
  else d["ts"] = nullptr;
  d["ts_estimada"] = est;
  d["tipo"] = tipo;
  d["detalle"] = detalle;
  d["dispositivo"] = DISPOSITIVO_ID;
  String json;
  serializeJson(d, json);
  colaAgregar('E', json);
}

void encolarLote(bool cerrado) {
  JsonDocument d;
  d["id"] = E.lote.id;
  d["uid_rfid"] = E.lote.uid;
  d["dispositivo"] = DISPOSITIVO_ID;
  char iso[24] = "";
  if (E.lote.inicioEpoch > 0) { fmtIsoUtc(E.lote.inicioEpoch, iso, sizeof(iso)); d["inicio"] = iso; }
  d["inicio_estimado"] = E.lote.inicioEst;
  d["estado"] = cerrado ? "cerrado" : "activo";
  char isoFin[24] = "";
  if (cerrado) {
    bool est;
    uint32_t ep = epochAhora(est);
    if (ep > 0) { fmtIsoUtc(ep, isoFin, sizeof(isoFin)); d["fin"] = isoFin; }
    d["duracion_s"] = loteSegundos();
    d["aire_min_total"] = r2(E.lote.aireS / 60.0f);
    d["dosis_total"] = E.lote.dosis;
  }
  String json;
  serializeJson(d, json);
  colaAgregar('O', json);
}

void registroActualizar() {
  uint32_t ahora = millis();
  if (E.lote.activo && E.modo != Modo::CALIBRACION &&
      ahora - tRegistro >= (uint32_t)REG_INTERVALO_S * 1000UL) {
    tRegistro = ahora;
    registrarAhora();
  }
  if (ahora - tEstado >= (uint32_t)ESTADO_INTERVALO_S * 1000UL) {
    tEstado = ahora;
    imprimirEstado();
  }
}

// ---------------------------------------------------------------- Comandos de SD
void sdListar() {
  SpiLock l;
  if (!E.sdOk) { Serial.println("[SD] No disponible"); return; }
  File dir = SD.open("/lotes");
  Serial.println("[SD] /lotes:");
  if (dir) {
    File f = dir.openNextFile();
    while (f) {
      Serial.printf("   %-32s %8lu bytes\n", f.name(), (unsigned long)f.size());
      f = dir.openNextFile();
    }
    dir.close();
  }
  if (SD.exists(COLA)) {
    File c = SD.open(COLA, FILE_READ);
    if (c) { Serial.printf("   /cola.txt %lu bytes (enviados hasta %lu)\n", (unsigned long)c.size(), (unsigned long)colaOffset); c.close(); }
  }
  if (perdidosRam) Serial.printf("   Registros descartados por cola RAM llena: %lu\n", (unsigned long)perdidosRam);
}

void sdVolcarCsvLote() {
  if (!E.lote.id[0]) { Serial.println("[SD] Aún no hay lotes"); return; }
  SpiLock l;
  if (!E.sdOk) { Serial.println("[SD] No disponible"); return; }
  File f = SD.open(rutaCsv(), FILE_READ);
  if (!f) { Serial.println("[SD] No se encontró el CSV del lote"); return; }
  Serial.printf("----- %s -----\n", rutaCsv().c_str());
  while (f.available()) Serial.write(f.read());
  f.close();
  Serial.println("----- fin -----");
}
