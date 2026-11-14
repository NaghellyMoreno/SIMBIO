# 02 · Proceso piloto, objetivos y variables

**Estado:** vigente · **Última actualización:** 2026-11-14

## Decisiones
| # | Decisión | Fecha | Motivo / alternativa descartada |
|---|---|---|---|
| P1 | Proceso piloto: **activación líquida de microorganismos de montaña (MM)** | 2026-11-05 | Es aeróbico (la aireación tiene sentido), se controla con melaza y aire y dura días. Se descartó el **biol** (anaerobio, OD ≈ 0, dura semanas o meses) |
| P2 | Piloto en **3 lotes consecutivos de 5 días (semanas 5–7)** | 2026-11-05 | Una activación puede durar hasta 15 días; la aireación la acorta (Castro-Barquero y González-Acuña, 2021) |
| P3 | **Oxígeno disuelto: opción B.** No se mide de forma continua; se controla y registra la **aireación** (ON/OFF y minutos). El OD se mide solo para validar, con un oxímetro prestado | 2026-11-05 | El sensor SEN0237-A costaba $813.000–1.071.000 (más que todo lo demás) y requiere NaOH y cambio de membrana |
| P4 | Variables del proceso: **pH, temperatura y aireación** | 2026-11-05 | Consecuencia de P3 |
| P5 | Tanque del prototipo: garrafón de **10 L** (~8 L de caldo, ~23 cm de profundidad). Menos de 5 L no se recomienda | 2026-11-08 | Antes 20 L. La planta de 4 tanques del informe AyC no cambia |
| P6 | Validación (OE3) con **pH-metro y oxímetro de laboratorio prestados**; muestras ≤ 50 mL | 2026-11-05 | OE3 es un piloto en campo, no *testing* |

## Objetivos
- **OG:** Desarrollar un sistema IoT de monitoreo, control y trazabilidad por lote de las variables fisicoquímicas en la producción artesanal de bioinsumos en biofábricas de Caldas.
- **OE1:** Diseñar la arquitectura del sistema (sensores, control, almacenamiento y visualización) a partir de los requerimientos de operación de las biofábricas.
- **OE2:** Implementar un prototipo IoT que mida las variables de cada lote, controle temperatura, aireación y dosificación, y almacene los registros en una base de datos consultable desde web y app, conservándolos ante pérdidas de conectividad.
- **OE3:** Evaluar el prototipo en un piloto de producción (activación líquida de MM) en una biofábrica o finca de Caldas: exactitud frente a instrumentos de referencia, estabilidad del control y completitud de los registros por lote.

Ajustes que motivaron esta redacción (5 nov): OE1 debe entregar el diseño (no solo requerimientos); OE3 debe ser piloto en campo; alinear el TRL (piloto en campo = TRL 5).

## Limitaciones que se reportan
- Precisión del pH esperada **±0,2** (sin ADS1115).
- El prototipo depende de energía y señal (sin RTC).
- La sonda E201 es de laboratorio, no para inmersión continua de días.
- El MQ-135 es un **indicador cualitativo**, no un medidor de CO₂.
