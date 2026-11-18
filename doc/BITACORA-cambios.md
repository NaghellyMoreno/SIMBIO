# Bitácora de cambios de SIMBIO

Entradas de la más reciente a la más antigua. Formato y obligatoriedad: ver [REGLA-documentacion-de-cambios.md](REGLA-documentacion-de-cambios.md).

## 2026-11-18 · Documento de entradas y salidas de SIMBIO
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se creó el doc "SIMBIO – Entradas y salidas del prototipo" (https://claude.ai/artifact/SYEPxaJfCsfg6nCDRTSwox): diagrama del sistema, tabla de las 6 entradas (E1–E6) y de las 4 salidas + 1 opcional (S1–S5) con pin, señal, rango y regla de control, matriz de qué entrada decide cada salida, alarmas críticas y avisos, mandos del operario y salidas de datos. Sin cambios de diseño: refleja el plan del 12 nov y el firmware actual.
- **Por qué:** Karen pidió un documento que explique las entradas y salidas del prototipo.
- **Archivos afectados:** ninguno local; copia en Markdown en los docs del proyecto (`claude/entradas-y-salidas.md`)
- **Documentos actualizados:** `decisiones/03-hardware-prototipo.md` (enlace en "Configuración resultante")
- **Pendiente derivado:** definir pin y conexión del agitador si se incluye (07).

## 2026-11-18 · Guía de montaje por tramos y sketches de prueba
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se creó la guía de montaje del prototipo en 12 tramos (0 a 11), con un criterio de aceptación por tramo y la prueba con melaza al final (doc "SIMBIO – Guía de montaje por tramos": https://claude.ai/artifact/QbPEWdqhXDLZFjRGxTH8FU). Se agregaron 12 sketches de prueba independientes, uno por tramo, con los pines del 12 nov y las mismas librerías del firmware (DHTesp, MFRC522, OneWire, DallasTemperature; servo por LEDC sin librería). La guía incluye las instrucciones para el código pendiente de AyC (modo escalón, PI por ventana de tiempo con anti-windup y servidor Modbus TCP según M4 y M9).
- **Por qué:** Karen pidió montar el prototipo paso a paso, comprobando cada tramo, y dejar la melaza para el final.
- **Archivos afectados:** `Simulacion/pruebas_por_tramo/` (nuevo: README y sketches T02 a T09)
- **Documentos actualizados:** `decisiones/03-hardware-prototipo.md` (H14–H18 y reglas de conexión), `decisiones/04-software-firmware-y-datos.md` (S12 y pendientes), `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** confirmar el calendario (lote de MM del 21 al 26 nov en vez del 19 al 24); conseguir portafusible de 1 A, dos tomas hembra aéreas y una válvula antirretorno (o montar la bomba por encima del garrafón); compilar cada sketch en el Arduino IDE (solo tuvieron revisión de sintaxis).

## 2026-11-18 · Biblioteca completa y depurada para Mendeley
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se generó `Doc/mendeley/SIMBIO_biblioteca_completa.bib` con las 63 referencias únicas del proyecto (las 41 de la formulación VIP + 22 de hojas de datos, normas y guías usadas en el artículo y el informe de AyC), sin duplicados, con las correcciones de la revisión (Saber, patente US 11655444 B2, Kumar) y los autores institucionales protegidos con doble llave para que Mendeley no los parta. Todas llevan la palabra clave SIMBIO.
- **Por qué:** reimportar la colección limpia; el RIS anterior generó duplicados y Mendeley partió los nombres de entidades.
- **Archivos afectados:** `Doc/mendeley/SIMBIO_biblioteca_completa.bib` (nuevo)
- **Documentos actualizados:** ninguno
- **Pendiente derivado:** vaciar la colección en Mendeley e importar el .bib; Vu et al. (2026) sigue sin verificar.

## 2026-11-18 · Revisión de la biblioteca de Mendeley: duplicados y verificación
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se revisó la exportación de la colección SIMBIO (86 entradas): 25 pares duplicados (21 con sufijo a/b y 4 de normas o entidades con el autor mal leído). Se verificaron en la web las referencias 2024–2026 y las patentes. Correcciones aplicadas en la bibliografía d.9 del .docx y en los archivos de Mendeley: autor de Saber et al. (2026) → "Saber, W. I. A."; título real de la patente US 11655444 B2 (Precision Fermentation, 2023); Kumar et al. (2026) con volumen 2(2) y DOI.
- **Por qué:** Karen preguntó si había referencias duplicadas o que no se pudieran encontrar.
- **Archivos afectados:** `IPI/SIMBIO_Formulacion_VIP.docx`, `Doc/textos/formulacion-d5-d9-TRL5.md`, `Doc/mendeley/SIMBIO_referencias.ris` y `.bib`
- **Documentos actualizados:** ninguno
- **Pendiente derivado:** fusionar duplicados en Mendeley; corregir en la diapositiva 13 del pptx de IPI las páginas de Prasetyono et al. (29–38, no 28–37); Vu et al. (2026) sin confirmar (no se cita en la formulación).

## 2026-11-18 · Archivos de importación para Mendeley
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se generaron `Doc/mendeley/SIMBIO_referencias.ris` y `.bib` con las 41 referencias de la bibliografía (d.9) de la formulación VIP, con la palabra clave SIMBIO.
- **Por qué:** agregar las referencias a la colección SIMBIO de Mendeley.
- **Archivos afectados:** `Doc/mendeley/SIMBIO_referencias.ris`, `Doc/mendeley/SIMBIO_referencias.bib` (nuevos)
- **Documentos actualizados:** ninguno
- **Pendiente derivado:** fusionar en Mendeley (vista Duplicates) las referencias que ya estaban en la colección.

## 2026-11-18 · Objetivos ya escritos, textuales en todos los entregables
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** el OG y los OE1–OE3 del equipo van textuales (02, P15). Formulación VIP: d.3 literal; b (495 palabras), d.2, d.4 (991), d.5 y d.7 ajustados (OE1 = mapeo operativo de pH y temperatura por análisis documental + juicio de experto; aireación como condición de operación; fase 2 = desarrollo del prototipo; fase 3 = evaluación frente a los métodos tradicionales). pptx de IPI: diapositiva 10 con los objetivos textuales. Docs de la formulación y de objetivos de las materias actualizados.
- **Por qué:** Karen pidió respetar los objetivos ya escritos.
- **Archivos afectados:** `IPI/SIMBIO_Formulacion_VIP.docx`, `IPI/MORENO_KAREN.pptx`, `Doc/textos/formulacion-b-d1-d3-d4-TRL5.md`, `Doc/textos/formulacion-d5-d9-TRL5.md`, `Doc/textos/formulacion-d1-d2-d6-2026-11-18.md`, docs https://claude.ai/artifact/EvznniHmLNFYiKhiwd3rj8 y https://claude.ai/artifact/VmBQf8keFHNugZEcZyStBR
- **Documentos actualizados:** `decisiones/02-proceso-y-objetivos.md` (P15; objetivos de P14 marcados como reemplazados), `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** pasar los mismos objetivos a los Talleres 1 y 2, el artículo y el informe de AyC (este último usa 4 objetivos por su esquema).

## 2026-11-18 · Anexo 1 en el formato de presupuesto y d.10 ajustada
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se diligenció `2026-11-13_Formato_Presupuesto_General.xlsx` (AÑO 1) y se repararon sus fórmulas (05, B-VIP2). Con la guía de formulación presupuestal que entregó Karen, la distribución se ajustó para cumplir los términos y la hoja % RUBROS: servicios personales 20 %, gastos generales 32,5 %, inversión 47,5 % (05, B-VIP1). En el .docx de la formulación se actualizaron d.10 y el desglose del Anexo 1 (apoyo a la estudiante $7.000.000; equipos $16.631.788 con medidor multiparamétrico de referencia; viajes con montos).
- **Por qué:** Karen entregó el formato oficial del presupuesto y la guía de orientación.
- **Archivos afectados:** `IPI/SIMBIO_Presupuesto_VIP.xlsx` (nuevo), `IPI/SIMBIO_Formulacion_VIP.docx`
- **Documentos actualizados:** `decisiones/05-presupuesto.md`, `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** horas por semana y valor hora de los tres investigadores (celda B8 de AÑO 1); cotizaciones de equipos, insumos, recuentos y viaje.

## 2026-11-18 · Formulación VIP completa en el formato oficial (.docx)
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se diligenció el formato en blanco `2026-09-04 FORMATO-FORMULACION-PROYECTO.docx` con todo el proyecto alineado a las fases inscritas: encabezado (título inscrito, grupos, tipo, área, integrantes con Diana como responsable propuesta, lugar, presupuesto $35.000.000), b, c, d.1–d.10 y el desglose del Anexo 1. Como no apareció la versión llena anterior, **d.1 y d.2 se redactaron de nuevo** (828 y 945 palabras + Tabla 1 de vigilancia) solo con datos ya verificados; d.6 se recortó a una página. Interlineado 1,5 (con doble, la sección d pasaba de 15 páginas). Validado sin errores de esquema; 29 páginas.
- **Por qué:** Karen pidió el entregable completo y alineado.
- **Archivos afectados:** `IPI/SIMBIO_Formulacion_VIP.docx` (nuevo), `Doc/textos/formulacion-d1-d2-d6-2026-11-18.md` (nuevo)
- **Documentos actualizados:** `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** horas por semana y valores en especie del Anexo 1; cotizaciones de equipos y viaje; confirmar a Diana como responsable y su vinculación; revisar d.1 y d.2 frente a la versión anterior si aparece.

## 2026-11-18 · Todo alineado con las tres fases inscritas en EAiFi 13.0
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** las fases y los resultados del resumen de IPI ya inscrito pasan a ser la referencia fija. Formulación VIP: OE1 = construir el protocolo por análisis documental + juicio de experto (antes "ajustar" a la unidad); OE2 = adaptar el prototipo TRL 4 (RFID, SD, nube, web y app); fase 1 sin lote de línea base (actividades: análisis documental, parámetros y rangos, trazabilidad, juicio de experto); fase 2 con 2.1 Condiciones del sitio; fase 3 con error medio absoluto, sesgo, límites de concordancia, completitud y detección oportuna de desviaciones; resultados con "código abierto" y "primera serie de registros continuos". Resumen ejecutivo 498 palabras, d.4 995 palabras. pptx de IPI: pregunta, objetivo general y objetivos específicos (diapositivas 8–10) con la redacción vigente. Doc de objetivos de las materias: OE1 y su metodología sin visita ni entrevista. Doc de inscripciones: resultados de IPI iguales al texto enviado (sin la frase de la estudiante de maestría).
- **Por qué:** Karen pidió respetar las fases ya inscritas en el evento y alinear todo con ellas.
- **Archivos afectados:** `Doc/textos/formulacion-b-d1-d3-d4-TRL5.md`, `Doc/textos/formulacion-d5-d9-TRL5.md` (respaldos en `Doc/textos/_respaldo/`), `IPI/MORENO_KAREN.pptx` (respaldo `IPI/MORENO_KAREN_respaldo_2026-11-18.pptx`), docs https://claude.ai/artifact/EvznniHmLNFYiKhiwd3rj8, https://claude.ai/artifact/Pa3dxmysofsYHLgepFAxUp y https://claude.ai/artifact/VmBQf8keFHNugZEcZyStBR
- **Documentos actualizados:** `decisiones/02-proceso-y-objetivos.md` (P14; OE1 y OE2 VIP de P12 marcados como reemplazados), `decisiones/09-alineacion-cuatro-materias.md` (M15), `decisiones/07-pendientes-y-preguntas.md`, `README.md`
- **Pendiente derivado:** pasar los textos al .docx; decidir si se iguala el título; terminar web y app antes del evento; reponer en el Anexo 1 el transporte y los insumos de los tres lotes.

## 2026-11-17 · Términos de referencia de la convocatoria 2026/2027 aplicados a la formulación (ejercicio)
- **Quién:** Claude (a pedido de Karen; es un ejercicio de estudio, no se postulará)
- **Qué cambió:** desarrollo tecnológico TRL ≥ 5 = 80 puntos (Tabla 4) → productos comprometidos 125 puntos (TRL 5 80 + ponencia nacional 20 + póster nacional 15 + artículo no indexado 10). Monto único $35.000.000; inversión y dotación ≥ 30 %; apoyo a estudiante de posgrado ≤ 1,5 SMMLV/mes (SMMLV 2026 $1.750.905); tiempo de investigadores como recurrente. Presupuesto de ejercicio: equipos 10.500.000 (30 %), apoyo a la estudiante 13.131.788 (1,5 SMMLV × 5 meses), materiales 4.368.212, servicios técnicos 3.000.000, viajes y eventos 4.000.000. Área estratégica: Biotecnología y agroindustria. Requisito habilitante: modalidad A (mujeres: Diana y Karen). Responsable propuesta: Diana (Óscar firma los términos como Vicerrector (E)). +5 puntos adicionales posibles por TRL ≥ 3 acreditado.
- **Por qué:** Karen entregó los términos de referencia.
- **Archivos afectados:** `Doc/textos/formulacion-d5-d9-TRL5.md` (puntajes), doc https://claude.ai/artifact/EvznniHmLNFYiKhiwd3rj8 (sección "Lo que dicen los términos de referencia")
- **Documentos actualizados:** `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** vinculación de Diana (planta u ocasional); autorreconocimiento para la modalidad A; cotizaciones para los equipos.

## 2026-11-17 · Formulación VIP (IPI): diagnóstico y textos TRL 4 → 5
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se revisó el .docx de la formulación contra las decisiones vigentes y se prepararon: título propuesto (14 palabras), áreas de conocimiento por la lista Colciencias, resumen ejecutivo (489 palabras), hipótesis, correcciones puntuales de c, d.1 y d.2 (dato "siete de 27", "mínima pérdida de datos", cifras de Saber et al., frases de Rodríguez-Valencia y Vu et al. quitadas para dar espacio a la hipótesis), párrafo de antecedentes y vacío nuevos, 2 patentes en la vigilancia, objetivos TRL 4 → 5, d.4 reescrita (952 palabras, sin entrevista, validación en tres lotes en una unidad productora; se propone la Planta de Bioprocesos de la Granja Tesorito), d.5–d.9 actualizados (40 referencias; sin Vu et al. ni Sigma Electrónica; precio del sensor de OD con DFRobot, USD 169).
- **Por qué:** terminar el formato de formulación, que es el entregable de Investigación para Ingeniería.
- **Archivos afectados:** `Doc/textos/formulacion-b-d1-d3-d4-TRL5.md` (nuevo), `Doc/textos/formulacion-d5-d9-TRL5.md` (nuevo; el de d5–d9 anterior se conserva), doc "SIMBIO – Formulación VIP (IPI): qué falta y textos para pegar" (https://claude.ai/artifact/EvznniHmLNFYiKhiwd3rj8)
- **Documentos actualizados:** `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** investigador responsable; puntaje TRL 5 en la Tabla 4; confirmar Tesorito y recuentos con Óscar; metas de la hipótesis; montos del Anexo 1; aplicar los cambios en el .docx.

## 2026-11-17 · Textos de inscripción a EAiFi 13.0 (cuatro materias)
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se redactaron los cuatro resúmenes para el formulario de EAiFi 13.0 (AyC, IoT, Emprendimiento e IPI): título, institución, programa, autores, introducción, objetivo, metodología, resultados, palabras clave y 7–8 referencias APA por materia. Cada resumen tiene entre 460 y 488 palabras (límite 500). AyC e IoT con resultados preliminares; Emprendimiento con el modelo de negocio; IPI con la propuesta VIP (TRL 4 → 5) y resultados esperados.
- **Por qué:** inscribir el mismo proyecto en el evento por las cuatro materias.
- **Archivos afectados:** doc "SIMBIO – Inscripciones EAiFi 13.0" (https://claude.ai/artifact/Pa3dxmysofsYHLgepFAxUp)
- **Documentos actualizados:** `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** confirmar con la organización si se aceptan cuatro inscripciones del mismo proyecto; apellidos de Yonier; metas de la hipótesis; verificar Park et al. (2020) y el año de Kalamaras et al.

## 2026-11-17 · Respuestas de Karen: entrega 27 nov, OE1 documental, VIP a TRL 5, LabVIEW, cita de Xavier
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** plan rehecho para entregar el 2026-11-27 (lote de MM 19–24 nov); OE1 por análisis documental + juicio de experto (sin entrevista); dos versiones de objetivos (materias TRL 4, VIP TRL 4 → 5 con tres lotes, entorno propuesto: Planta de Bioprocesos de la Granja Tesorito); SCADA en LabVIEW en lugar de FUXA; cita de Xavier corregida en el pptx de IPI (2026 → Xavier y Rodrigues 2024, referencia y enlace); autores por entregable confirmados.
- **Por qué:** respuestas de Karen a las 8 preguntas abiertas.
- **Archivos afectados:** `IPI/MORENO_KAREN.pptx` (diapositiva 11; respaldo `IPI/MORENO_KAREN_respaldo_2026-11-17.pptx`), doc "SIMBIO – Alineación con las cuatro materias" (sección "Respuestas del 17 nov" y plan)
- **Documentos actualizados:** `decisiones/02-proceso-y-objetivos.md` (P11–P13, objetivos vigentes), `decisiones/09-alineacion-cuatro-materias.md` (M10–M14; M4 y M6 marcadas como reemplazadas), `decisiones/06-referencias.md`, `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** confirmar metas de la hipótesis; confirmar con Óscar la planta de Tesorito y el puntaje TRL 5; préstamo de pH-metro y termómetro; redactar la ficha de emprendimiento; firmware PI + Modbus; VI de LabVIEW.

## 2026-11-17 · Alineación de SIMBIO con las cuatro materias
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se analizaron los PIAA y las guías de AyC, IoT, Emprendimiento e IPI frente al estado del proyecto; se armó la matriz de brechas (A/B/C), el plan de dos semanas (lote de MM desde el 2026-11-21), las especificaciones de AyC (identificación de la planta, PI por ventana, Modbus TCP, FUXA), la hipótesis propuesta y 27 referencias nuevas (7 blancas, 18 grises, 2 patentes). Decisiones M1–M9.
- **Por qué:** que un solo prototipo y un solo texto base cumplan las cuatro materias, con los materiales ya comprados.
- **Archivos afectados:** doc "SIMBIO – Alineación con las cuatro materias" (https://claude.ai/artifact/VDSKLMHXbpAA7DP74ZwbrR), `Doc/README.md`
- **Documentos actualizados:** `decisiones/09-alineacion-cuatro-materias.md` (nuevo), `decisiones/01-alcance-y-entregables.md` (A5), `decisiones/06-referencias.md` (R4), `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** implementar M4 en el firmware; corregir el Canvas EQ_3; agregar hipótesis e impactos cultural y global al formato VIP; responder las 8 preguntas abiertas.

## 2026-11-17 · Secciones d.5 a d.9 de la formulación VIP
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se redactaron los resultados esperados (d.5), los impactos (d.6), el cronograma de 12 meses (d.7), los compromisos (d.8) y la bibliografía APA de 34 referencias (d.9). Productos comprometidos: TRL 3–4 (50) + ponencia nacional (20) + póster nacional (15) + artículo en revista no indexada (10) = 95 puntos (mínimo 80). Todo alineado con los objetivos de la formulación (OE1 con pH, temperatura y oxígeno disuelto; OE3 frente a métodos tradicionales); el número de lotes se dejó neutro.
- **Por qué:** completar el formato de formulación.
- **Archivos afectados:** `Doc/textos/formulacion-d5-d9-resultados-a-bibliografia.md`, `Doc/README.md`
- **Documentos actualizados:** `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** completar Vu et al. (2026) y la URL de Sigma Electrónica; confirmar Kumar et al. (2026) y Lana et al. (2022); confirmar la duración de 12 meses; decidir uno o tres lotes y ajustar d.4; prever en el Anexo 1 el viaje a la ponencia.

## 2026-11-15 · Equipo investigador y metodología de la formulación VIP
- **Quién:** Claude (a pedido de Karen)
- **Qué cambió:** se registraron los integrantes, los grupos, el tipo de proyecto y los roles; se redactaron la sección c (trayectoria del equipo, 493 palabras) y la d.4 (metodología en 3 fases, 995 palabras) de la formulación VIP; se fijaron las metas de validación del OE3 y el muestreo; se anotaron las inconsistencias del .docx (OE1 con OD, correcciones de referencias sin aplicar).
- **Por qué:** completar las secciones c y d.4 del formato de formulación.
- **Archivos afectados:** `Doc/textos/formulacion-c-equipo-investigadores.md`, `Doc/textos/formulacion-d4-metodologia.md`, `Doc/README.md`
- **Documentos actualizados:** `decisiones/08-equipo-investigador.md` (nuevo, E1–E5), `decisiones/02-proceso-y-objetivos.md` (P7–P10 y objetivos de la formulación), `decisiones/01-alcance-y-entregables.md` (estado de la formulación), `decisiones/06-referencias.md` (R3), `decisiones/07-pendientes-y-preguntas.md`
- **Pendiente derivado:** pegar la d.4 en el .docx; corregir el OE1; aplicar las correcciones de 06; confirmar roles e investigador responsable; verificar las referencias de R3; unificar los objetivos.

## 2026-11-14 · Convenciones generales para commits
- **Quién:** Claude
- **Qué cambió:** se establecieron los tipos y usos permitidos para los mensajes de commit (`feat`, `fix`, `docs`, `style`, `refactor`, `test` y `chore`).
- **Por qué:** mantener un historial de cambios consistente y entendible en el proyecto.
- **Archivos afectados:** `CLAUDE.md`, `doc/REGLA-documentacion-de-cambios.md`
- **Documentos actualizados:** regla general de trabajo y regla de documentación
- **Pendiente derivado:** ninguno

## 2026-11-14 · Creación de la documentación de decisiones y de la regla
- **Quién:** Claude
- **Qué cambió:** se crearon `Doc/README.md`, la regla de documentación, esta bitácora, `Doc/decisiones/01…07` y `CLAUDE.md` en la raíz.
- **Por qué:** dejar por escrito las decisiones tomadas y obligar a documentar todo cambio futuro.
- **Archivos afectados:** `Doc/`, `CLAUDE.md`
- **Documentos actualizados:** todos (creación)
- **Pendiente derivado:** ninguno

## 2026-11-13 · Simulación Wokwi, firmware y base de datos
- **Qué cambió:** carpeta `Simulacion/` con sketch de Arduino IDE, `diagram.json`, `wokwi.toml` y esquema Supabase.
- **Documentos:** 04 (S1–S10)
- **Pendiente derivado:** compilar en Arduino IDE; crear el proyecto Supabase; web + app.

## 2026-11-12 · Configuración de bajo costo para fases iniciales
- **Qué cambió:** sin ADS1115 ni RTC; pH y MQ-135 al ADC1; hora NTP + flash; módulo SD; HC-SR04 para nivel; servo como válvula de melaza; caja de paso; nuevo plan de pines; presupuesto $233.184.
- **Documentos:** 03 (H2, H3, H8–H11), 05 (B1, B2)
- **Pendiente derivado:** actualizar formulación, artículo, informe AyC y Canvas.

## 2026-11-11 · Kit de pH, préstamos, alimentación, gases y calentador
- **Qué cambió:** kit PH-4502C + E201 + buffers; componentes prestados; adaptador Techman + L7805 (sin 12 V ni LM2596); MQ-135 en vez de MQ-5; calentador de acuario de 25 W.
- **Documentos:** 03 (H4–H7, H12)

## 2026-11-08 · Tanque de 20 L a 10 L
- **Documentos:** 02 (P5)

## 2026-11-05 · Alcance, proceso piloto, OD y referencias
- **Qué cambió:** requisitos de ambos cursos; título; objetivos; activación líquida de MM; opción B para el OD; piloto de 3 lotes de 5 días; verificación de referencias; Talleres 1 y 2.
- **Documentos:** 01 (A1–A4), 02 (P1–P4, P6), 06 (R1, R2)
