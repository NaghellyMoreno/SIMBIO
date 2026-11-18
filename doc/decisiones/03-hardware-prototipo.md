# 03 · Hardware del prototipo

**Estado:** vigente (fases iniciales; se revisa si se consigue patrocinio) · **Última actualización:** 2026-11-18

## Configuración resultante
**Entradas (6):** pH (PH-4502C + E201), temperatura del líquido (DS18B20), nivel (HC-SR04), gases (MQ-135), lote (RFID RC522), temperatura y humedad ambiente (DHT11).

**Salidas (4 + 1 opcional):** bomba de aire, calentador sumergible de 25 W, servo SG90 dosificador de melaza, alarma (buzzer + LEDs, cuenta como 1), motor agitador (opcional).

**Control:** ESP32 en el prototipo; PLC LOGO! 12/24RCE para la planta de 4 tanques del informe AyC.

Explicación detallada de cada entrada y salida (pines, señales, reglas de control, alarmas): doc "SIMBIO – Entradas y salidas del prototipo" (https://claude.ai/artifact/SYEPxaJfCsfg6nCDRTSwox).

## Decisiones
| # | Decisión | Fecha | Motivo / alternativa descartada |
|---|---|---|---|
| H1 | Se reutilizan ESP32 + placa de expansión del inventario | 2026-11-05 | Ya los tienen |
| H2 | Nivel con el **HC-SR04** del inventario, montado en la tapa (divisor de 680 Ω en Echo, protección contra condensación) | 2026-11-12 | El JSN-SR04T tiene 25 cm de zona muerta: no cabe en el tanque de 10 L |
| H3 | Dosificación de melaza con **servo SG90 como válvula de pinza** (por gravedad) | 2026-11-12 | Reemplaza la bomba peristáltica de 12 V |
| H4 | Kit de pH: **PH-4502C + E201-BNC + buffers pH 4 y 7** ($110.285, Mercado Libre) | 2026-11-11 | Trae los buffers; no hay que pedirlos prestados |
| H5 | Gases: comprar **MQ-135** como indicador cualitativo | 2026-11-11 | El MQ-5 prestado detecta GLP y gas natural, no gases de fermentación (solo sirve para probar cableado) |
| H6 | Calentador: **sumergible de acuario 25 W**, vidrio, 20 cm, termostato 20–32 °C ($31.999). Perilla 2 °C sobre el setpoint del ESP32; 1–2 cm de las paredes; lejos de las sondas; nunca fuera del agua | 2026-11-11 | Se descartó la resistencia de porcelana con alambre expuesto (electrifica el caldo) |
| H7 | **Alimentación:** adaptador Techman AD-2515 en **7,5 V** (o 9 V) → 1N4007 → 100 µF + 0,33 µF → **L7805** con disipador → 0,1 µF + 470 µF → riel de 5 V. Medir la salida real antes de conectar | 2026-11-11 | Se descartaron la fuente de 12 V y el LM2596 (a 12 V el L7805 disiparía ~4 W) |
| H8 | **Sin ADS1115.** pH al GPIO 36 y MQ-135 al GPIO 39 (ADC1). Divisor 1 kΩ / (1 kΩ + 820 Ω), `analogReadMilliVolts()`, promedio de 64–100 lecturas, 0,1 µF por entrada, calibración de 2 puntos | 2026-11-12 | Ahorro; precisión ±0,2 pH (±0,1 con ADS1115). Se agrega con patrocinio |
| H9 | **Sin RTC DS3231.** Hora por NTP + última hora en flash (`Preferences`) + tiempo transcurrido del lote; registros "hora estimada" tras corte sin red | 2026-11-12 | Ahorro; se agrega con patrocinio |
| H10 | **Módulo SD por SPI** (tamaño completo, con regulador 3,3 V) con la microSD que ya tienen + adaptador; CSV por lote; iniciar a 4 MHz; FAT32 | 2026-11-12 | Registro sin conexión |
| H11 | **Lado de 110 V** (relés, red, bomba, calentador) dentro de una **caja de paso 10 × 10** con bornera de tornillo 10 A / 250 V y pasacables; nunca en la protoboard. Se conserva el termostato del calentador | 2026-11-12 | Seguridad. Las "borneras" del kit CCTV son plugs DC y no sirven |
| H12 | Componentes **prestados**: 2 × RC522 con tarjetas y llaveros, 2+ relés Songle SRD-05VDC-SL-C (10 A/250 V), DHT11, MQ-5, adaptador Techman | 2026-11-11 | — |
| H13 | Se descartaron el sensor de OD SEN0237-A, la bomba peristáltica, el JSN-SR04T, la fuente de 12 V, el LM2596, los 74LS, displays de 7 segmentos y la batería de 9 V | 2026-11-05 a 12 | Costo o no aplican |
| H14 | **Montaje por tramos** (0 a 11), uno a la vez, con sketch de prueba y criterio de aceptación cada uno; la melaza solo en el tramo 11, después de probar todo con agua. Guía: https://claude.ai/artifact/QbPEWdqhXDLZFjRGxTH8FU | 2026-11-18 | Pedido de Karen: comprobar cada tramo antes del siguiente |
| H15 | **Caja de paso:** fusible de 1 A en la fase de entrada; fase por COM–NO de cada relé, neutro directo; salidas a tomas hembra para no cortar los cables de la bomba y el calentador; revisión por alguien con experiencia eléctrica y primera prueba con un bombillo | 2026-11-18 | Seguridad (recomendado; por conseguir) |
| H16 | **Bomba de aire por encima del borde del garrafón** o con válvula antirretorno | 2026-11-18 | Evitar que el agua se devuelva a la bomba al apagarse |
| H17 | **Riel de 5 V ≥ 4,85 V con carga** (MQ-135 en el tramo 1; servo moviéndose en el tramo 9). Si no se cumple a 7,5 V, el adaptador pasa a 9 V | 2026-11-18 | A 7,5 V, tras el 1N4007 quedan ~6,7 V y el L7805 necesita ~7 V para regular |
| H18 | **Aislar el garrafón** (icopor o aislante reflectivo, con ventana para ver el nivel) si el escalón se estabiliza por debajo de 30 °C. La prueba escalón se hace de 0 a 100 % de calentador desde la temperatura ambiente | 2026-11-18 | Estimación: pérdidas de ~2,5 W/°C sin aislar; con 25 W el agua quedaría ~10 °C sobre el ambiente. El termostato del calentador corta a 32 °C, así que la consigna de 37 °C de M11 no es alcanzable |

## Pines del ESP32 (plan del 12 nov 2026, sin I2C)
| Función | GPIO |
|---|---|
| pH (Po, con divisor + 0,1 µF) | 36 |
| MQ-135 (AO, con divisor + 0,1 µF) | 39 |
| Relé bomba de aire / relé calentador | 21 / 22 |
| Pulsador LOTE / OK (pull-up interno) | 32 / 33 |
| DIP calibración (pull-up externo 10 kΩ) | 35 |
| DS18B20 (pull-up 4,7 kΩ) | 4 |
| DHT11 | 13 |
| Servo | 14 |
| HC-SR04 Trig / Echo (con divisor) | 25 / 34 |
| RC522 SS / RST | 5 / 27 |
| Módulo SD CS | 26 |
| SPI SCK / MISO / MOSI | 18 / 19 / 23 |
| LEDs rojo / amarillo / verde | 16 / 17 / 2 |
| Buzzer (vía 2N2222) | 15 |

Si se agregan ADS1115 o RTC: I2C vuelve a 21/22, relés a 32/33 y pulsadores a 35/36/39.

## Reglas de conexión
- Nunca más de 3,3 V en un pin del ESP32: divisor obligatorio en pH y MQ-135.
- RC522 y DHT11 a **3,3 V** (el RC522 se daña a 5 V).
- MQ-135: calefactor ~150 mA desde el riel de 5 V; precalentar 24–48 h la primera vez; montarlo en la tapa protegido de condensación.
- RC522 (CS 5) y SD (CS 26) comparten SPI; si el RC522 deja de responder, mover la SD al segundo bus SPI.
- Cargas inductivas DC con diodo de rueda libre; servo con 470 µF cerca y GND común.
- Interferencia entre sondas: leer una sonda analógica a la vez; compensar el pH con el DS18B20.
- Sonda E201: limpiar, guardar húmeda en KCl, calibrar con buffers 4 y 7.
- Con el ESP32 conectado por USB, desconectar el cable del riel de 5 V a su pin 5V/VIN (el GND común se mantiene).
- Si un relé no se apaga con 3,3 V: un 2N2222 por relé (GPIO → 1 kΩ → base, emisor a GND, colector a IN) y `RELE_ACTIVO_ALTO` en 1.
