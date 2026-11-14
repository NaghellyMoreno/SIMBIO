// =====================================================================
//  SIMBIO – WiFi y envío a Supabase (API REST de PostgREST)
//  Corre en su propia tarea (núcleo 0) para que el control nunca se
//  detenga mientras se espera la respuesta del servidor.
//  Envíos idempotentes: si un registro se reenvía tras un corte, Supabase
//  lo ignora gracias a las restricciones UNIQUE (on_conflict).
// =====================================================================
#include "simbio.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <algorithm>

static uint32_t ultimoIntentoWifi = 0;
static uint32_t fallosSeguidos = 0;
static uint32_t esperaHastaMs = 0;

bool nubeConfigurada() {
  return strlen(SUPABASE_URL) > 12 && strlen(SUPABASE_KEY) > 20;
}

static void conectarWifi() {
  Serial.printf("[WIFI] Conectando a \"%s\"...\n", WIFI_SSID);
#if SIMBIO_SIM
  WiFi.begin(WIFI_SSID, WIFI_PASS, 6);   // Wokwi-GUEST está en el canal 6 (conexión más rápida)
#else
  WiFi.begin(WIFI_SSID, WIFI_PASS);
#endif
}

static int postSupabase(const char *tabla, const char *conflicto, bool combinar, const String &cuerpo) {
  WiFiClientSecure cliente;
  cliente.setInsecure();   // prototipo: no valida el certificado. Producción: setCACert()
  HTTPClient http;
  http.setTimeout(10000);
  String url = String(SUPABASE_URL) + "/rest/v1/" + tabla + "?on_conflict=" + conflicto;
  if (!http.begin(cliente, url)) return -1;
  http.addHeader("apikey", SUPABASE_KEY);
  if (strncmp(SUPABASE_KEY, "eyJ", 3) == 0)          // clave "anon" clásica (JWT)
    http.addHeader("Authorization", String("Bearer ") + SUPABASE_KEY);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Prefer", combinar ? "resolution=merge-duplicates,return=minimal"
                                    : "resolution=ignore-duplicates,return=minimal");
  int codigo = http.POST(cuerpo);
  if (codigo < 200 || codigo >= 300) {
    String resp = codigo > 0 ? http.getString() : HTTPClient::errorToString(codigo);
    Serial.printf("[NUBE] %s -> HTTP %d: %s\n", tabla, codigo, resp.substring(0, 220).c_str());
  }
  http.end();
  return codigo;
}

static void enviarPendientes() {
  char tipo = 0;
  String cuerpo;
  if (!colaPeek(tipo, cuerpo, NUBE_LOTE_MAX)) return;

  const char *tabla = "lecturas", *conflicto = "lote_id,t_lote_s";
  if (tipo == 'E') { tabla = "eventos"; conflicto = "uid_evento"; }
  else if (tipo == 'O') { tabla = "lotes"; conflicto = "id"; }

  int codigo = postSupabase(tabla, conflicto, tipo == 'O', cuerpo);
  if (codigo >= 200 && codigo < 300) {
    colaConsumir();
    fallosSeguidos = 0;
    Serial.printf("[NUBE] %s: enviado (%lu pendientes)\n", tabla, (unsigned long)colaPendientes());
  } else if (codigo == 400 || codigo == 409 || codigo == 422) {
    // Datos que Supabase nunca aceptará: se descartan para no bloquear la cola.
    // Siguen guardados en el CSV de la SD.
    Serial.println("[NUBE] Registro rechazado: se descarta de la cola (queda en el CSV)");
    colaConsumir();
  } else {
    // Sin conexión, clave incorrecta (401/403), tabla inexistente (404)...: reintentar con espera
    fallosSeguidos++;
    uint32_t espera = std::min<uint32_t>(60000, 3000 * fallosSeguidos);
    esperaHastaMs = millis() + espera;
    if (codigo == 401 || codigo == 403) Serial.println("[NUBE] Revise SUPABASE_KEY y las políticas RLS (supabase/esquema.sql)");
    if (codigo == 404) Serial.println("[NUBE] ¿Ejecutó supabase/esquema.sql? Revise también SUPABASE_URL");
  }
}

static void tareaNube(void *) {
  bool conectadoAntes = false;
  bool avisoSinConfig = false;
  for (;;) {
    bool conectado = WiFi.status() == WL_CONNECTED;
    if (conectado != conectadoAntes) {
      conectadoAntes = conectado;
      E.wifiOk = conectado;
      if (conectado) {
        Serial.printf("[WIFI] Conectado. IP %s, RSSI %d dBm\n", WiFi.localIP().toString().c_str(), WiFi.RSSI());
        configTzTime(TZ_POSIX, NTP_1, NTP_2);   // relanza NTP con la red ya disponible
      } else {
        Serial.println("[WIFI] Sin conexión: los datos quedan en la SD hasta reconectar");
      }
    }
    if (!conectado && millis() - ultimoIntentoWifi > 20000) {
      ultimoIntentoWifi = millis();
      WiFi.disconnect();
      conectarWifi();
    }
    if (conectado && !nubeConfigurada() && !avisoSinConfig) {
      avisoSinConfig = true;
      Serial.println("[NUBE] Supabase no configurado (simbio/secrets.h): solo se guarda en la SD");
    }
    if (conectado && nubeConfigurada() && colaPendientes() > 0 &&
        (int32_t)(millis() - esperaHastaMs) >= 0) {
      enviarPendientes();
      vTaskDelay(pdMS_TO_TICKS(50));
      continue;
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void nubeInicio() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  conectarWifi();
  ultimoIntentoWifi = millis();
  xTaskCreatePinnedToCore(tareaNube, "nube", 16384, nullptr, 1, nullptr, 0);
}
