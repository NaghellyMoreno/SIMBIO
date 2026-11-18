# 02 · Proceso piloto, objetivos y variables

**Estado:** vigente · **Última actualización:** 2026-11-18

## Decisiones
| # | Decisión | Fecha | Motivo / alternativa descartada |
|---|---|---|---|
| P1 | Proceso piloto: **activación líquida de microorganismos de montaña (MM)** | 2026-11-05 | Es aeróbico (la aireación tiene sentido), se controla con melaza y aire y dura días. Se descartó el **biol** (anaerobio, OD ≈ 0, dura semanas o meses) |
| P2 | Piloto en **3 lotes consecutivos de 5 días (semanas 5–7)** | 2026-11-05 | Una activación puede durar hasta 15 días; la aireación la acorta (Castro-Barquero y González-Acuña, 2021) |
| P3 | **Oxígeno disuelto: opción B.** No se mide de forma continua; se controla y registra la **aireación** (ON/OFF y minutos). El OD se mide solo para validar, con un oxímetro prestado | 2026-11-05 | El sensor SEN0237-A costaba $813.000–1.071.000 (más que todo lo demás) y requiere NaOH y cambio de membrana |
| P4 | Variables del proceso: **pH, temperatura y aireación** | 2026-11-05 | Consecuencia de P3 |
| P5 | Tanque del prototipo: garrafón de **10 L** (~8 L de caldo, ~23 cm de profundidad). Menos de 5 L no se recomienda | 2026-11-08 | Antes 20 L. La planta de 4 tanques del informe AyC no cambia |
| P6 | Validación (OE3) con **pH-metro y oxímetro de laboratorio prestados**; muestras ≤ 50 mL | 2026-11-05 | OE3 es un piloto en campo, no *testing* |
| P7 | Metodología de la formulación VIP en **3 fases, una por objetivo**: (1) protocolo de control de calidad y trazabilidad con mapeo operativo en finca; (2) prototipo IoT con pruebas de laboratorio (TRL 4); (3) evaluación en el piloto frente a instrumentos de referencia y frente al registro tradicional del productor. Texto completo en [../textos/formulacion-d4-metodologia.md](../textos/formulacion-d4-metodologia.md) (995 palabras) | 2026-11-15 | Sección d.4 del formato VIP (máx. 1000 palabras) |
| P8 | Metas de validación del OE3: error de pH ≤ 0,2 unidades y de temperatura ≤ 0,5 °C frente a los instrumentos de laboratorio; se reportan error medio absoluto, sesgo, límites de concordancia, % del tiempo con T dentro de ±0,5 °C del setpoint y completitud de registros (lecturas obtenidas frente a esperadas) | 2026-11-15 | Coherente con la precisión esperada del pH (±0,2) y con el setpoint de `config.h` |
| P9 | Muestreo de validación: **dos veces al día** durante los 3 lotes; el OD se mide con el oxímetro prestado para relacionarlo con los ciclos de aireación | 2026-11-15 | Complementa P6 |
| P11 | **OE1 sin entrevista:** el mapeo operativo en finca se reemplaza por **análisis documental** (Guía técnica de biofábricas de Anacafé, Navarro 2022; Castro-Barquero y González-Acuña 2021; criterios de buenas prácticas de Xavier, Martins do Vale y de Almeida 2026; proyecto de resolución ICA de biopreparados 2024; Res. ICA 68370) + **juicio de experto** (Dr. Óscar Julián Sánchez Toro, lista de chequeo). *Reemplaza la visita y la entrevista de P7 y la encuesta al productor de P10 en las materias* | 2026-11-17 | No hay productor disponible para entrevistar |
| P12 | **Dos versiones de objetivos:** materias = TRL 4 con un lote de 10 L en condiciones controladas; formulación VIP = siguiente fase, **TRL 4 → 5**, validación en entorno relevante con **tres lotes**. Entorno propuesto: Planta de Bioprocesos y Agroindustria de la Granja Tesorito (U. de Caldas), que ya produce bioinsumos con cascarilla de café, cáscara de plátano y bovinaza (por confirmar con Óscar) | 2026-11-17 | Las materias ya dejan el prototipo en TRL 4; la propuesta no debe prometer lo ya hecho |
| P13 | En las materias, el método tradicional se simula: un integrante registra el lote como lo haría un productor (cuaderno, color y olor, dos veces al día) en paralelo al prototipo | 2026-11-17 | Consecuencia de P11 |
| P14 | **Las tres fases inscritas en EAiFi 13.0 (resumen de IPI, enviado) son la referencia y no se cambian.** Fase 1: protocolo de control de calidad y trazabilidad por análisis documental (guías técnicas de biofábricas, literatura —Xavier et al., 2026— y normativa del ICA, 2020), con pH, temperatura y aireación validados por juicio de experto. Fase 2: prototipo ESP32 con seis sensores, cuatro actuadores, RFID, SD sin conexión, nube, web y app (TRL 4, construido en AyC e IoT). Fase 3: validación TRL 5 con tres lotes consecutivos de MM en una unidad productora de Caldas, frente a instrumentos de laboratorio (error medio absoluto, sesgo, límites de concordancia) y al registro tradicional (completitud y detección oportuna de desviaciones). Resultados inscritos: protocolo; prototipo TRL 5 con manual y código abierto; primera serie de registros continuos de pH, temperatura y aireación por lote en Caldas. En el VIP el OE1 vuelve a ser *construir* el protocolo (documental + experto), sale el lote de línea base de la fase 1 y la visita a la unidad pasa a la fase 2 (2.1 Condiciones del sitio). *Reemplaza el OE1 y el OE2 VIP de P12.* **Sus objetivos fueron reemplazados por P15; las fases siguen vigentes** | 2026-11-18 | Pedido de Karen: respetar lo inscrito en el evento |
| P15 | **Objetivos textuales del equipo en todos los entregables (VIP, pptx de IPI y doc de objetivos de las materias):** OG "Desarrollar un sistema IoT de control y trazabilidad por lote para la producción artesanal de bioinsumos en biofábricas de Caldas." OE1 "Diseñar un protocolo de control de calidad y trazabilidad para la producción de bioinsumos, mediante el mapeo operativo de los parámetros críticos de control (pH y temperatura) en las biofábricas de Caldas." OE2 "Desarrollar un prototipo IoT (TRL 4), basado en el protocolo, para el registro por lote con capacidad de almacenamiento offline." OE3 "Evaluar el desempeño del prototipo implementado en comparación con los métodos tradicionales." La metodología los cumple con las fases inscritas (P14): el mapeo operativo se hace por análisis documental + juicio de experto; la aireación pasa a ser condición de operación que se controla y se registra (no parámetro crítico de control); el OE2 desarrolla el prototipo a partir del TRL 4 de las materias; el OE3 evalúa en TRL 5 con tres lotes frente a los métodos tradicionales y a instrumentos de laboratorio | 2026-11-18 | Pedido de Karen: respetar los objetivos ya escritos |
| P10 | Comparación con el método tradicional: el productor registra cada lote como lo hace hoy y al cierre responde una encuesta corta de utilidad. Entrevista y encuesta con consentimiento informado | 2026-11-15 | El OE3 de la formulación pide comparar con los métodos tradicionales |

## Objetivos
- **OG:** Desarrollar un sistema IoT de monitoreo, control y trazabilidad por lote de las variables fisicoquímicas en la producción artesanal de bioinsumos en biofábricas de Caldas.
- **OE1:** Diseñar la arquitectura del sistema (sensores, control, almacenamiento y visualización) a partir de los requerimientos de operación de las biofábricas.
- **OE2:** Implementar un prototipo IoT que mida las variables de cada lote, controle temperatura, aireación y dosificación, y almacene los registros en una base de datos consultable desde web y app, conservándolos ante pérdidas de conectividad.
- **OE3:** Evaluar el prototipo en un piloto de producción (activación líquida de MM) en una biofábrica o finca de Caldas: exactitud frente a instrumentos de referencia, estabilidad del control y completitud de los registros por lote.

Ajustes que motivaron esta redacción (5 nov): OE1 debe entregar el diseño (no solo requerimientos); OE3 debe ser piloto en campo; alinear el TRL (piloto en campo = TRL 5).

## Objetivos tal como están en la formulación VIP (2026-11-15)
La formulación usa una redacción distinta a la de arriba. La metodología P7 sigue esta:
- **OG:** Desarrollar un sistema de trazabilidad para la producción artesanal de bioinsumos en biofábricas de Caldas.
- **OE1:** Diseñar un protocolo de control de calidad y trazabilidad para la producción de bioinsumos, mediante el mapeo operativo de los parámetros críticos de control (pH, temperatura y ~~oxígeno disuelto~~ **aireación**, según P3) en las biofábricas de Caldas.
- **OE2:** Desarrollar un prototipo IoT (TRL 4), basado en el protocolo, para el registro por lote con capacidad de almacenamiento offline.
- **OE3:** Evaluar el desempeño del prototipo implementado en comparación con los métodos tradicionales.

**Pendiente:** el .docx de la formulación todavía dice "oxígeno disuelto" en el OE1; hay que cambiarlo por "aireación". Falta decidir si los talleres, el artículo y la formulación usan la misma redacción de objetivos.

## Objetivos vigentes (2026-11-17; VIP actualizado el 2026-11-18, P14)
**Materias (TRL 4, un lote):**
- OG: Desarrollar un sistema IoT de control y trazabilidad por lote para la producción artesanal de bioinsumos en biofábricas de Caldas.
- OE1: Diseñar un protocolo de control de calidad y trazabilidad para la activación líquida de microorganismos de montaña, mediante el análisis documental de guías técnicas y normativa, con los parámetros críticos (pH, temperatura y aireación) validados por juicio de experto.
- OE2: Desarrollar un prototipo IoT (TRL 4), basado en el protocolo, que mida y controle los parámetros de cada lote identificado por RFID y almacene los registros, con respaldo sin conexión, en una base de datos consultable desde web y app.
- OE3: Evaluar el desempeño del prototipo en un lote de MM de 10 L en condiciones controladas, frente al registro tradicional y a instrumentos de referencia.

**Objetivos vigentes en todos los entregables (P15, 2026-11-18):** OG y OE1–OE3 textuales de P15.

*Reemplazados por P15:* **Formulación VIP (TRL 4 → 5), alineada con las fases inscritas en EAiFi 13.0 (P14, 2026-11-18):**
- OE1: Construir un protocolo de control de calidad y trazabilidad para la activación líquida de MM, mediante el análisis documental de guías técnicas de biofábricas, literatura y normativa del ICA, con los parámetros críticos de pH, temperatura y aireación validados por juicio de experto.
- OE2: Adaptar el prototipo IoT de control y trazabilidad por lote (TRL 4), con RFID, respaldo sin conexión en tarjeta SD, base de datos en la nube, web y aplicación, a las condiciones de operación de una unidad productora de bioinsumos de Caldas.
- OE3: Validar el sistema en entorno relevante (TRL 5) con tres lotes consecutivos de activación líquida de MM en esa unidad, frente a instrumentos de laboratorio y al registro tradicional.

*Reemplazados por P14* (VIP, 2026-11-17): ~~OE1: Ajustar el protocolo documental al proceso real de una unidad productora de bioinsumos de Caldas.~~ ~~OE2: Adaptar el prototipo TRL 4 a las condiciones de esa unidad (energía, conectividad, protección del equipo, volumen del tanque).~~

*Reemplazan* los "objetivos unificados del 15 nov" para el OE1 (ya no hay mapeo en finca) y, en el VIP, el OE3 en TRL 4.

## Limitaciones que se reportan
- Precisión del pH esperada **±0,2** (sin ADS1115).
- El prototipo depende de energía y señal (sin RTC).
- La sonda E201 es de laboratorio, no para inmersión continua de días.
- El MQ-135 es un **indicador cualitativo**, no un medidor de CO₂.
