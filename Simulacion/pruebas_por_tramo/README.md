# SIMBIO – Sketches de prueba por tramo

Un sketch independiente por tramo del montaje. Cada uno prueba **solo** lo que se acaba de conectar, con los pines del plan del 12 nov 2026 (los mismos de `Simulacion/simbio/config.h`). La guía completa (conexiones, criterios de aceptación y registro) está en el doc **"SIMBIO – Guía de montaje por tramos"**.

| Tramo | Carpeta | Qué prueba | Librerías |
|---|---|---|---|
| 2 | `T02_esp32_wifi_ntp` | ESP32 vivo, wifi, hora NTP (UTC-5) | — |
| 3 | `T03_interfaz_local` | LEDs, buzzer por 2N2222, pulsadores, DIP | — |
| 4 | `T04_temperaturas` | DS18B20 (líquido) y DHT11 (ambiente) | OneWire, DallasTemperature, DHT sensor library for ESPx |
| 5 | `T05_nivel_hcsr04` | Nivel; calcula `DIST_SENSOR_FONDO_CM` y `AREA_TANQUE_CM2` | — |
| 6 | `T06a_sd`, `T06b_rfid`, `T06c_spi_compartido` | SD sola, RC522 solo, los dos en el mismo SPI | MFRC522 |
| 7 | `T07a_mq135`, `T07b_ph` | MQ-135 y pH por el ADC1, con divisor | OneWire, DallasTemperature (pH) |
| 8 | `T08a_reles`, `T08b_temperatura_agua` | Polaridad de los relés (sin 110 V); bomba y calentador con agua; ON-OFF y escalón | OneWire, DallasTemperature |
| 9 y 11 | `T09_servo_valvula` | Ángulos de la pinza y volumen por dosis: primero con agua (9), al final con melaza (11) | — |

El tramo 10 (integración con agua) usa el firmware principal `Simulacion/simbio` con `#define SIMBIO_SIM 0`.

## Cómo cargar cada sketch
1. Arduino IDE 2 → *Archivo → Abrir* → el `.ino` de la carpeta del tramo.
2. Placa **ESP32 Dev Module**; puerto del ESP32.
3. *Subir*. Si se queda en `Connecting....`, mantenga presionado **BOOT** hasta que empiece a escribir.
4. Monitor serie a **115200** baudios y fin de línea **"Nueva línea"** (los comandos lo necesitan).

## Valores que salen de las pruebas y van a `config.h` del firmware
| Valor | Sale del tramo | Constante en `config.h` |
|---|---|---|
| Relé activo en alto o en bajo | 8a | `RELE_ACTIVO_ALTO` |
| Distancia sensor–fondo | 5 | `DIST_SENSOR_FONDO_CM` |
| Área del garrafón | 5 | `AREA_TANQUE_CM2` |
| Ángulos de la pinza | 9 | `SERVO_CERRADO_GRAD`, `SERVO_ABIERTO_GRAD` |
| Tiempo de apertura por dosis | 11 (melaza) | `DOSIS_ABIERTA_MS` (valor real, el segundo de `SIM_O_REAL`) |
| Modo prototipo real | 10 | `SIMBIO_SIM 0` |

Revisados solo con una comprobación de sintaxis; el primer compilado real es en el Arduino IDE.
