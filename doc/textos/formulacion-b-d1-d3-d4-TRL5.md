# Formulación VIP – b, d.1 (hipótesis), d.3 y d.4, versión TRL 4 → 5

Redactado el 2026-11-17 (noche). Correcciones de buscar y reemplazar de c, d.1 y d.2 en el doc "SIMBIO – Formulación VIP (IPI): qué falta y textos para pegar" (https://claude.ai/artifact/EvznniHmLNFYiKhiwd3rj8).

**Actualizado el 2026-11-18:** b, d.3 y d.4 se alinearon con las tres fases inscritas en EAiFi 13.0 (resumen de IPI): fase 1 = protocolo por análisis documental + juicio de experto; fase 2 = prototipo (TRL 4, construido en AyC e IoT) adaptado a la unidad productora; fase 3 = validación TRL 5 con tres lotes. Se quitó el lote de línea base de la fase 1. Versión anterior: `_respaldo/formulacion-b-d1-d3-d4-TRL5_2026-11-17.md`.

**Actualizado el 2026-11-18 (2):** d.3 usa textualmente los objetivos ya escritos por el equipo (P15: OE1 mapeo operativo de pH y temperatura; OE2 desarrollar el prototipo TRL 4 con almacenamiento offline; OE3 evaluar frente a los métodos tradicionales). b y d.4 se ajustaron a esa redacción sin salirse de las tres fases inscritas.

## b. Resumen ejecutivo (495 palabras)

En Caldas, el café, el plátano y la ganadería generan residuos que el Programa Nacional de Biofábricas busca convertir en bioinsumos dentro de la propia finca. Sin embargo, multiplicar microorganismos es un proceso biológico que, en las biofábricas artesanales, se realiza sin registrar el pH, la temperatura ni la aireación de cada lote. La evidencia de Brasil muestra que los bioinsumos producidos en finca presentan contaminación con microorganismos no deseados (Bocatti et al., 2022; Kovalski et al., 2024) y que pocos productores analizan por qué falla un lote (Xavier y Rodrigues, 2024). Sin datos, el productor no puede corregir su procedimiento ni demostrar el control de calidad que exige la Resolución 68370 (ICA, 2020).

El proyecto responde a la pregunta: ¿cómo puede un sistema IoT de bajo costo medir, controlar y registrar el pH, la temperatura y la aireación de cada lote para dar trazabilidad a la producción artesanal de bioinsumos en las biofábricas de Caldas? Su hipótesis es que un sistema basado en ESP32 registra el pH y la temperatura con errores de hasta 0,2 unidades y 0,5 °C frente a instrumentos de laboratorio, mantiene la temperatura dentro de ±0,5 °C del punto de consigna al menos el 90 % del tiempo y conserva al menos el 99 % de los registros aunque se pierda la conexión.

El objetivo general es desarrollar un sistema IoT de control y trazabilidad por lote para la producción artesanal de bioinsumos en biofábricas de Caldas. Es una investigación aplicada en tres fases. En la primera, se diseña el protocolo de control de calidad y trazabilidad mediante el mapeo operativo de los parámetros críticos de control (pH y temperatura), por análisis documental de guías técnicas, literatura y normativa del ICA, validado por juicio de experto. En la segunda, se desarrolla el prototipo IoT (TRL 4) con base en el protocolo, a partir del que el equipo ya construyó: ESP32, seis variables, identificación del lote por RFID, almacenamiento sin conexión en tarjeta SD, base de datos en la nube, web y aplicación. En la tercera, se evalúa su desempeño en entorno relevante (TRL 5) con tres lotes consecutivos de activación líquida de microorganismos de montaña en una unidad productora de Caldas, en comparación con los métodos tradicionales y con instrumentos de laboratorio.

Los resultados esperados son un protocolo de control de calidad y trazabilidad para la activación líquida de microorganismos de montaña, un prototipo validado en TRL 5 con manual y código abierto, y la primera serie de registros continuos de pH, temperatura y aireación por lote en condiciones de producción en Caldas. Con ellos, el productor podrá identificar cuándo y por qué se desvía un lote y documentar el control de calidad que exige la norma. Los productos comprometidos son el desarrollo tecnológico, una ponencia y un póster en eventos nacionales y un artículo, además de la formación de una estudiante de maestría. Así, aporta a reducir la dependencia de fertilizantes importados, cercana al 90 % (Cortés Alvarado, 2026).

## d.1 Hipótesis (va entre la pregunta y la justificación)

**Hipótesis.** Un sistema IoT de bajo costo basado en ESP32 registra el pH y la temperatura de cada lote con errores de hasta 0,2 unidades y 0,5 °C frente a instrumentos de laboratorio, mantiene la temperatura dentro de ±0,5 °C del punto de consigna al menos el 90 % del tiempo y conserva al menos el 99 % de los registros esperados aunque se pierda la conexión.

## d.3 Objetivos

**Objetivo general.** Desarrollar un sistema IoT de control y trazabilidad por lote para la producción artesanal de bioinsumos en biofábricas de Caldas.

**Objetivos específicos.**

1. Diseñar un protocolo de control de calidad y trazabilidad para la producción de bioinsumos, mediante el mapeo operativo de los parámetros críticos de control (pH y temperatura) en las biofábricas de Caldas.
2. Desarrollar un prototipo IoT (TRL 4), basado en el protocolo, para el registro por lote con capacidad de almacenamiento offline.
3. Evaluar el desempeño del prototipo implementado en comparación con los métodos tradicionales.

## d.4 Metodología (991 palabras)

Es una investigación aplicada en tres fases, una por objetivo específico.

**Punto de partida.** El proyecto parte del prototipo SIMBIO en TRL 4, construido en noviembre de 2026 en las asignaturas de Automatización e IoT, y evaluado en laboratorio con un lote de activación líquida de microorganismos de montaña (MM) de 10 L. Un ESP32 mide seis variables (pH con sonda PH-4502C, temperatura del líquido con DS18B20, nivel con HC-SR04, gases con MQ-135 como indicador cualitativo, y temperatura y humedad ambiente con DHT11), identifica el lote con RFID RC522 y maneja cuatro actuadores (bomba de aire, calentador sumergible, servo que dosifica la melaza y alarma con semáforo). Guarda un archivo CSV por lote en una tarjeta SD como respaldo sin conexión y sincroniza los datos con una base de datos en la nube, consultable desde una página web y una aplicación móvil.

**Fase 1. Diseño del protocolo de control de calidad y trazabilidad (OE1, meses 1 a 3)**

*Actividad 1.1. Mapeo operativo.* El mapeo se hace por análisis documental de la guía técnica de biofábricas de Anacafé (Navarro, 2022), la literatura sobre la activación líquida de MM (Castro-Barquero y González-Acuña, 2021) y sobre la producción de bioinsumos en finca (Xavier et al., 2026) y la normativa del ICA (ICA, 2020). Se extraen materias primas, dosis, tiempos, condiciones de operación y controles de calidad, y se construye el diagrama del proceso con sus puntos críticos.

*Actividad 1.2. Parámetros críticos de control y rangos.* Se fijan los parámetros críticos de control y sus valores de referencia: temperatura de 30 °C ± 0,5 °C y alertas de pH (crítico por debajo de 3,0; aviso si supera 6,5 después de 48 horas). La aireación (ciclos de 15 minutos) y la dosis diaria de melaza se definen como condiciones de operación que se controlan y se registran; el oxígeno disuelto se mide solo de forma puntual para validar.

*Actividad 1.3. Trazabilidad del lote.* El protocolo define qué es un lote, cómo se identifica, qué variables y eventos se registran y con qué frecuencia, y cómo se calibran las sondas, siguiendo la definición de trazabilidad de ISO 9000 (ISO, 2015).

*Actividad 1.4. Validación por juicio de experto.* El experto en bioprocesos del equipo y el responsable de la unidad productora revisan los parámetros críticos de control y sus rangos con una lista de chequeo. Sus observaciones se incorporan y la versión final queda en un acta.

**Fase 2. Desarrollo del prototipo IoT (OE2, meses 3 a 6)**

*Actividad 2.1. Condiciones del sitio.* Con el aval de la unidad productora (se propone la Planta de Bioprocesos y Agroindustria de la Granja Tesorito), se registran el volumen y tipo de recipiente, la forma de airear, la energía disponible, la cobertura de wifi y de datos móviles y la forma en que hoy se registran los lotes.

*Actividad 2.2. Hardware.* Se adapta el tanque al volumen de la unidad, se protege la electrónica de la humedad en una caja cerrada, se cambia la sonda de pH de laboratorio por una apta para inmersión prolongada y se agregan un convertidor analógico-digital externo (ADS1115) y un reloj de tiempo real (DS3231) para mejorar la precisión del pH y conservar la hora sin conexión.

*Actividad 2.3. Software y conectividad.* Se ajustan la base de datos, la web y la aplicación a los campos y alertas del protocolo, y el registro por lote con almacenamiento offline en la tarjeta SD.

*Actividad 2.4. Pruebas en sitio.* Con agua y durante 24 horas se verifican la calibración del pH (buffers de pH 4 y 7), el control de temperatura, cada actuador y alarma, y la recuperación ante cortes de red y de energía.

**Fase 3. Evaluación del desempeño en entorno relevante, TRL 5 (OE3, meses 6 a 9)**

*Actividad 3.1. Lotes.* En la unidad productora se producen tres lotes consecutivos de activación líquida de MM de cinco días, siguiendo el protocolo de la fase 1.

*Actividad 3.2. Comparación con instrumentos de laboratorio.* Dos veces al día se comparan las lecturas del prototipo con un pH-metro y un termómetro de laboratorio. Se calculan el error medio absoluto, el sesgo y los límites de concordancia (Bland y Altman, 1986). Metas: error de pH ≤ 0,2 unidades y de temperatura ≤ 0,5 °C.

*Actividad 3.3. Comparación con los métodos tradicionales.* En paralelo, el personal de la unidad registra cada lote como lo hace hoy. Se comparan la completitud de los registros (lecturas obtenidas frente a esperadas, con meta ≥ 99 % para el prototipo) y la detección oportuna de desviaciones (cuándo detecta cada método que un parámetro sale del rango del protocolo), así como el tiempo dedicado a registrar.

*Actividad 3.4. Desempeño del control y calidad del lote.* Se miden el porcentaje del tiempo con la temperatura en ± 0,5 °C del punto de consigna (meta ≥ 90 %), las alarmas y sus causas y la evolución del pH. Al cierre de cada lote se hacen recuentos microbianos en el laboratorio del grupo Alimentos y Agroindustria para relacionar, de forma exploratoria, las condiciones registradas con la calidad del bioinsumo.

**Análisis y presentación de resultados (meses 9 a 12)**

Los datos se analizan con estadística descriptiva, series de tiempo por lote y gráficos de concordancia. Los resultados se presentan en un informe técnico, un artículo, una ponencia, un póster y una demostración del sistema en la unidad productora. El código se publica como código abierto.

**Roles.** Óscar Julián Sánchez Toro orienta el bioproceso, el protocolo, los recuentos microbianos y la validación; Diana Rocío Varón Serna dirige la adaptación del hardware y la comunicación; Karen Naghelly Moreno Ortiz administra la base de datos, procesa las series de tiempo y analiza los resultados.

**Consideraciones éticas.** El proyecto no incluye entrevistas ni encuestas; la unidad productora participa con una carta de aval. Los bioinsumos de los lotes de prueba no se aplican a cultivos ni se comercializan y se inactivan antes de desecharlos. Las conexiones de 110 V van en una caja cerrada.
