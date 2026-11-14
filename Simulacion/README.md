# SIMBIO – Simulación del prototipo en Wokwi (Arduino IDE + VS Code)

Simulación completa del prototipo SIMBIO: ESP32 con 6 entradas y 4 salidas, control del proceso, trazabilidad por lote con RFID, registro en SD y envío a Supabase.

**Arduino IDE** compila el firmware y **VS Code** solo ejecuta Wokwi, porque Wokwi no tiene extensión para Arduino IDE.
El **mismo firmware** sirve para Wokwi y para el ESP32 real: se cambia con una línea en `simbio/config.h`.

```
Simulacion/
├── wokwi.toml            le dice a Wokwi qué binario cargar
├── diagram.json          el circuito (ESP32 + sensores + actuadores)
├── simbio/               SKETCH de Arduino IDE (abrir simbio.ino)
│   ├── simbio.ino        programa principal y comandos del monitor serie
│   ├── config.h          MODO (simulación/real), pines y parámetros
│   ├── secrets.h         WiFi y claves de Supabase
│   └── sensores, control, actuadores, lote, registro, nube (.cpp) + simbio.h
└── supabase/esquema.sql  tablas lotes, lecturas y eventos
```

---

## 1. Instalación (una sola vez, en macOS)

### Arduino IDE (compilar)
1. Descarga **Arduino IDE 2** de https://www.arduino.cc/en/software e instálalo.
2. **Placas ESP32:** *Arduino IDE → Settings…* → en *Additional boards manager URLs* pega
   `https://espressif.github.io/arduino-esp32/package_esp32_index.json` → OK.
   Luego, en el **Boards Manager** (icono de placa a la izquierda), busca **esp32** (de Espressif Systems) e instálalo.
3. **Librerías:** en el **Library Manager** (icono de libros), busca e instala:
   - **MFRC522** (de GithubCommunity / miguelbalboa)
   - **OneWire** (de Paul Stoffregen)
   - **DallasTemperature** (de Miles Burton)
   - **DHT sensor library for ESPx** (de beegee_tokyo)
   - **ArduinoJson** (de Benoit Blanchon), versión 7

   Si al instalar una librería pregunta por sus dependencias, elige *Install all*.

### VS Code (simular)
4. Descarga **VS Code** de https://code.visualstudio.com y arrástralo a *Aplicaciones*.
5. Panel de extensiones (⇧⌘X) → instala solo **Wokwi Simulator**. No hace falta PlatformIO.
6. **Licencia de Wokwi (gratis):** `F1` → **Wokwi: Request a New License** → inicia sesión o crea una cuenta en wokwi.com → confirma en el navegador y en VS Code. Si vence, repite este paso. Con la licencia gratuita, Wokwi necesita internet.

## 2. Ejecutar la simulación

1. En `simbio/config.h` verifica que esté `#define SIMBIO_SIM 1`.
2. En **Arduino IDE**: *File → Open…* → `Simulacion/simbio/simbio.ino`.
3. Placa: *Tools → Board → esp32 → **ESP32 Dev Module***. No hace falta conectar ningún ESP32.
4. **Exporta el binario:** *Sketch → **Export Compiled Binary*** (⌥⌘S). La primera vez tarda unos minutos.
   Se crea `simbio/build/esp32.esp32.esp32/` con `simbio.ino.bin` y `simbio.ino.elf`.
5. En **VS Code**: *File → Open Folder…* → `Simulacion` (la carpeta de arriba, donde están `wokwi.toml` y `diagram.json`).
6. `F1` → **Wokwi: Start Simulator**. El monitor serie aparece debajo del circuito; escribe `ayuda`.

> **Ciclo de trabajo:** edita en Arduino IDE → ⌥⌘S (exportar) → Wokwi en VS Code recarga solo.
> El botón ✓ (*Verify*) de Arduino IDE **no** genera el binario para Wokwi: usa *Export Compiled Binary*.
> Mantén visible la pestaña del simulador; si se oculta, Wokwi pausa.

## 3. Guion de demostración (protocolo de pruebas en simulación)

En Wokwi los tiempos van acelerados (ver la tabla de la sección 6). Al hacer clic en un sensor aparece su control deslizante.

| # | Acción en Wokwi | Resultado esperado |
|---|---|---|
| 1 | Arrancar | Destello verde (espera). El monitor serie muestra `[WIFI] Conectado` y `[HORA] Sincronizada por NTP` |
| 2 | Clic en el RC522 → elegir tarjeta (tecla `b`, `g`, `k`…) → **Tap** | Amarillo rápido (lote armado) y pitido |
| 3 | Presionar **LOTE** antes de 15 s | `*** LOTE MM-… INICIADO ***`, LED verde. Calentador ON (T = 24 °C < 29,5 °C). Bomba de aire en ciclos de 20 s |
| 4 | DS18B20 → 31 °C, luego 29,4 °C | Calentador OFF y luego ON (histéresis ±0,5 °C) |
| 5 | DS18B20 → 39 °C | Rojo + buzzer, calentador bloqueado. **OK** silencia el buzzer |
| 6 | HC-SR04 → 12 cm (nivel 18 cm) | Rojo `nivel_bajo`: el calentador se bloquea aunque haga frío |
| 7 | HC-SR04 → 3 cm (nivel 27 cm) | Rojo `nivel_alto`: la dosis de melaza se pospone |
| 8 | Esperar 90 s o escribir `dosis` | El servo abre 3 s (válvula de melaza) y se registra `dosis_melaza` |
| 9 | Potenciómetro de pH a la derecha | El pH baja; con pH < 3 se pone en rojo. Pasados 120 s, pH > 6,5 da amarillo (no acidifica) |
| 10 | Sensor de gas → subir ppm | Amarillo `gas_alto` |
| 11 | DIP 1 en ON | Calibración: amarillo fijo + verde intermitente, salidas apagadas. Potenciómetro → **OK** (pH 7). Girarlo a la derecha ~100 pasos → **OK** (pH 4). DIP en OFF |
| 12 | Escribir `reiniciar` | El ESP32 reinicia y **reanuda el mismo lote** (evento `reanudacion`) |
| 13 | Escribir `csv` | Muestra el CSV del lote guardado en la SD |
| 14 | Mantener **LOTE** 2,5 s | `*** LOTE … CERRADO ***` con duración, minutos de aire y número de dosis |

## 4. Conectar con Supabase (opcional, recomendado)

Sin Supabase todo funciona igual y los datos quedan en la SD.

1. Crea un proyecto gratuito en https://supabase.com.
2. **SQL Editor → New query** → pega `supabase/esquema.sql` → **Run**.
3. En *Project Settings → API* (o el botón *Connect*) copia la **Project URL** y la clave **anon / publishable**. Nunca uses la `service_role`.
4. Pégalas en `simbio/secrets.h`:
   ```c
   #define SUPABASE_URL "https://xxxxxxxx.supabase.co"
   #define SUPABASE_KEY "eyJ...  o  sb_publishable_..."
   ```
5. Exporta el binario de nuevo, simula e inicia un lote. En *Table Editor* verás `lotes`, `lecturas` y `eventos` llenándose. La vista `ultima_lectura` es la que conviene usar para la web y la app.

El envío corre en una tarea aparte (núcleo 0), así el control nunca se detiene. Si no hay red, los registros esperan en `/cola.txt` de la SD y se suben al volver la conexión, sin duplicados.

## 5. Funciones del firmware

- **Temperatura:** on/off con histéresis de ±0,5 °C sobre el setpoint (30 °C; comando `sp 28`). **Enclavamientos:** el calentador no enciende con nivel bajo, sin lectura de nivel o de temperatura, ni por encima de 38 °C.
- **Aireación:** ciclos ON/OFF con minutos acumulados por lote. Es la variable que reemplaza al oxígeno disuelto (opción B).
- **Melaza:** dosis programadas con el servo, que actúa como válvula de pinza. Se posponen si el tanque está lleno.
- **pH:** calibración de dos puntos con los buffers 7 y 4, guardada en la flash y validada (pendiente entre 100 y 250 mV/pH). Lleva compensación de temperatura con el DS18B20.
- **Trazabilidad:** el lote se identifica con la tarjeta RFID; ID `MM-AAAAMMDD-HHMMSS`. Cada registro guarda `t_lote_s` (segundos desde el inicio) y `ts_estimada` si no había hora NTP.
- **Robustez:** el estado del lote se guarda en la flash (sobrevive a reinicios). La última hora conocida también se guarda en la flash. La cola de envío está en la SD, o en RAM si no hay SD.
- **Semáforo:** verde = OK, amarillo = aviso, rojo + buzzer = crítico. Destello verde = en espera; amarillo rápido = tarjeta leída, esperando LOTE.

### Alarmas (columna `alarmas`, máscara de bits)

| Bit | Nombre | Tipo | Bit | Nombre | Tipo |
|---|---|---|---|---|---|
| 1 | nivel_bajo | crítica | 128 | ph_alto | aviso |
| 2 | nivel_alto | crítica | 256 | sensor_ph | aviso |
| 4 | sensor_nivel | crítica | 512 | gas_alto | aviso |
| 8 | temp_alta | crítica | 1024 | sensor_dht | aviso |
| 16 | sensor_temp | crítica | 2048 | sd | aviso |
| 32 | ph_bajo | crítica | 4096 | sin_red | aviso |
| 64 | temp_fuera | aviso | 8192 | rfid | aviso |
| | | | 16384 | ph_sin_calibrar | aviso |

## 6. Simulación frente al prototipo real

| Aspecto | Wokwi (`sim`) | Real (`esp32`) |
|---|---|---|
| DHT | DHT22 (Wokwi no tiene DHT11) | DHT11 |
| Sonda de pH | Potenciómetro que da el voltaje ya dividido | PH-4502C + divisor 1 k / 1,82 k + 0,1 µF |
| MQ-135 | Sensor de gas genérico (MQ-2) | MQ-135 con divisor |
| Eco del HC-SR04 | Directo | Con divisor de 680 Ω |
| Buzzer | Pasivo (tono de 2,2 kHz) | KY-012 activo vía 2N2222 |
| Cargas de 110 V | LEDs azul y naranja en los relés | Bomba y calentador en la caja de paso |
| Ciclo de aire | 20 s / 20 s | 15 min / 15 min |
| Dosis de melaza | Cada 90 s, 3 s abierta | Cada 24 h, 8 s abierta |
| Registro | Cada 5 s | Cada 60 s |
| Gracia de temp. / pH | 60 s / 120 s | 2 h / 48 h |

Todos los valores están en `simbio/config.h` (macro `SIM_O_REAL`).

**Para cargar al ESP32 real:**
1. En `simbio/config.h` pon `#define SIMBIO_SIM 0`.
2. En `secrets.h` pon tu WiFi.
3. Revisa `RELE_ACTIVO_ALTO`: si tu módulo de relé se activa con LOW, ponlo en 0.
4. Conecta el ESP32 por USB, elige el puerto en *Tools → Port* y presiona **Upload** (→).
5. Calibra el pH con los buffers antes del primer lote.

## 7. Si algo falla

- **Wokwi dice que no encuentra el firmware:** no se ha exportado el binario (⌥⌘S), o la placa no es *ESP32 Dev Module*. La carpeta debe llamarse `build/esp32.esp32.esp32`.
- **Error `ledcSetup was not declared`:** no debería pasar, porque el código es compatible con los núcleos ESP32 2.x y 3.x. Si aparece, avisa con el mensaje.
- **Falta una librería (`No such file or directory`):** instálala desde el Library Manager (lista en la sección 1).
- **WiFi no conecta en Wokwi:** revisa la licencia y la conexión a internet. La red es `Wokwi-GUEST`.
- **`[NUBE] … HTTP 401/403`:** la clave o las políticas RLS están mal; vuelve a ejecutar `esquema.sql`. **`HTTP 404`:** la URL está mal o faltan las tablas.
- **Cualquier otro error de compilación:** copia el mensaje completo y compártelo.
