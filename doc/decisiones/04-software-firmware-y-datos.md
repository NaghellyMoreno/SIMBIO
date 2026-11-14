# 04 · Software: firmware, simulación, base de datos y frontend

**Estado:** vigente · **Última actualización:** 2026-11-14
**Código:** `Simulacion/` (ver `Simulacion/README.md`)

## Decisiones
| # | Decisión | Fecha | Motivo / alternativa descartada |
|---|---|---|---|
| S1 | **Arduino IDE 2** compila y exporta el binario; **Wokwi para VS Code** lo simula | 2026-11-13 | Wokwi no tiene extensión para Arduino IDE. Se descartó **PlatformIO** |
| S2 | **Un solo firmware** para simulación y prototipo real: `#define SIMBIO_SIM 1/0` en `simbio/config.h` (macro `SIM_O_REAL`) | 2026-11-13 | Evita mantener dos versiones |
| S3 | En Wokwi: DHT22 en lugar de DHT11, potenciómetro como sonda de pH, MQ-2 genérico como MQ-135, LEDs en lugar de cargas de 110 V, tiempos acelerados; el tanque se maneja con los deslizadores (sin modelo térmico) | 2026-11-13 | Limitaciones de Wokwi |
| S4 | Compatible con núcleo ESP32 **2.x y 3.x** | 2026-11-13 | — |
| S5 | Base de datos: **Supabase** (PostgreSQL + API REST). Tablas `lotes`, `lecturas`, `eventos`; vista `ultima_lectura` para web y app. Esquema en `Simulacion/supabase/esquema.sql`. Solo clave anon/publishable, nunca `service_role` | 2026-11-13 | Plan gratuito. Firebase quedó como alternativa descartada |
| S6 | Envío a la nube en una tarea aparte (núcleo 0); **cola offline** en `/cola.txt` de la SD (o RAM), sin duplicados (`unique (lote_id, t_lote_s)`) | 2026-11-13 | El control nunca se detiene sin red |
| S7 | Trazabilidad: lote identificado por tarjeta RFID; ID `MM-AAAAMMDD-HHMMSS`; cada registro guarda `t_lote_s` y `ts_estimada` | 2026-11-13 | Consecuencia de H9 (sin RTC) |
| S8 | Estado del lote y última hora en la flash: el lote **se reanuda** tras un reinicio | 2026-11-13 | Robustez |
| S9 | Calibración de pH de 2 puntos (buffers 7 y 4) con DIP, guardada en flash y validada (pendiente 100–250 mV/pH) | 2026-11-13 | — |
| S10 | Semáforo: verde OK, amarillo aviso, rojo + buzzer crítico. Alarmas como máscara de bits (ver README) | 2026-11-13 | — |
| S11 | Frontend: **web (HTML/JS) + app** (MIT App Inventor o Flutter) leyendo `ultima_lectura` | 2026-11-05 | Pendiente de construir; falta elegir la herramienta de la app |

## Parámetros de control (prototipo real; ajustables en `config.h`)
| Parámetro | Real | Wokwi |
|---|---|---|
| Setpoint temperatura | 30 °C ± 0,5 (histéresis) | igual |
| T máxima (bloquea calentador) | 38 °C | igual |
| Nivel mínimo (bloquea calentador) / máximo (no dosifica) | 19 cm / 26 cm | igual |
| Ciclo de aire | 15 min ON / 15 min OFF | 20 s / 20 s |
| Dosis de melaza | cada 24 h, 8 s abierta | cada 90 s, 3 s |
| Registro | cada 60 s | cada 5 s |
| pH crítico / aviso | < 3,0 / > 6,5 después de 48 h | igual / tras 120 s |
| Gracia de temperatura | 2 h | 60 s |
| Índice de gas para aviso | 60 (0–100) | igual |

Enclavamientos del calentador: no enciende con nivel bajo, sin lectura de nivel o temperatura, ni por encima de 38 °C.

## Pendiente
- Compilar en Arduino IDE y reportar errores (solo se revisó con g++ y cabeceras simuladas).
- Crear el proyecto Supabase y poner URL/clave en `simbio/secrets.h`.
- Construir web + app.
