# Formulación VIP – b, d.1 (hipótesis), d.3 y d.4, versión TRL 4 → 5

Redactado el 2026-11-17 (noche). Correcciones de buscar y reemplazar de c, d.1 y d.2 en el doc "SIMBIO – Formulación VIP (IPI): qué falta y textos para pegar" (https://claude.ai/artifact/EvznniHmLNFYiKhiwd3rj8).

## b. Resumen ejecutivo (489 palabras)

En Caldas, el café, el plátano y la ganadería generan residuos que el Programa Nacional de Biofábricas busca convertir en bioinsumos dentro de la propia finca. Sin embargo, multiplicar microorganismos es un proceso biológico que, en las biofábricas artesanales, se realiza sin registrar el pH, la temperatura ni la aireación de cada lote. La evidencia de Brasil muestra que los bioinsumos producidos en finca presentan contaminación con microorganismos no deseados (Bocatti et al., 2022; Kovalski et al., 2024) y que pocos productores analizan por qué falla un lote (Xavier y Rodrigues, 2024). Sin datos, el productor no puede corregir su procedimiento ni demostrar el control de calidad que exige la Resolución 68370 (ICA, 2020).

El proyecto responde a la pregunta: ¿cómo puede un sistema IoT de bajo costo medir, controlar y registrar el pH, la temperatura y la aireación de cada lote para dar trazabilidad a la producción artesanal de bioinsumos en las biofábricas de Caldas? Su hipótesis es que un sistema basado en ESP32 registra el pH y la temperatura con errores de hasta 0,2 unidades y 0,5 °C frente a instrumentos de laboratorio, mantiene la temperatura dentro de ±0,5 °C del punto de consigna al menos el 90 % del tiempo y conserva al menos el 99 % de los registros aunque se pierda la conexión.

El objetivo general es desarrollar un sistema IoT de control y trazabilidad por lote para la producción artesanal de bioinsumos en biofábricas de Caldas. El proyecto parte de un prototipo en nivel de madurez tecnológica TRL 4, que el equipo construyó y evaluó en laboratorio, y lo lleva a TRL 5 en tres fases. En la primera, el protocolo de control de calidad y trazabilidad, elaborado a partir de guías técnicas, literatura y normativa, se ajusta al proceso real de una unidad productora de Caldas. En la segunda, el prototipo se adapta a las condiciones de esa unidad: identifica cada lote con RFID, mide seis variables, controla la temperatura, la aireación y la dosis de melaza, guarda los datos en una tarjeta SD cuando no hay conexión y los sincroniza con una base de datos que se consulta desde una página web y una aplicación móvil. En la tercera, se producen tres lotes consecutivos de activación líquida de microorganismos de montaña y el sistema se compara con instrumentos de referencia y con el registro tradicional de la unidad.

Los resultados esperados son un protocolo validado, un prototipo en TRL 5 con código fuente y manual, y la primera serie de registros continuos por lote de este proceso en Caldas. Los productos comprometidos son el desarrollo tecnológico, una ponencia y un póster en eventos nacionales y un artículo, además de la formación de una estudiante de maestría. Con ellos, el proyecto aporta a una prioridad nacional: reducir la dependencia de fertilizantes importados, que llega a cerca del 90 % (Cortés Alvarado, 2026), con bioinsumos producidos en el territorio y de calidad documentada.

## d.1 Hipótesis (va entre la pregunta y la justificación)

**Hipótesis.** Un sistema IoT de bajo costo basado en ESP32 registra el pH y la temperatura de cada lote con errores de hasta 0,2 unidades y 0,5 °C frente a instrumentos de laboratorio, mantiene la temperatura dentro de ±0,5 °C del punto de consigna al menos el 90 % del tiempo y conserva al menos el 99 % de los registros esperados aunque se pierda la conexión.

## d.3 Objetivos

**Objetivo general.** Desarrollar un sistema IoT de control y trazabilidad por lote para la producción artesanal de bioinsumos en biofábricas de Caldas.

1. Ajustar un protocolo de control de calidad y trazabilidad para la activación líquida de microorganismos de montaña, construido por análisis documental y juicio de experto, al proceso real de una unidad productora de bioinsumos de Caldas, con los parámetros críticos de pH, temperatura y aireación.
2. Adaptar el prototipo IoT de control y registro por lote (TRL 4) a las condiciones de operación de esa unidad: energía, conectividad, protección del equipo y volumen del tanque.
3. Validar el sistema en entorno relevante (TRL 5) en tres lotes consecutivos de activación líquida de microorganismos de montaña, frente al registro tradicional y a instrumentos de referencia.

## d.4 Metodología (952 palabras)

**Punto de partida.** El proyecto parte del prototipo SIMBIO en TRL 4, que el equipo construyó y evaluó en laboratorio en noviembre de 2026 con un lote de activación líquida de microorganismos de montaña (MM) de 10 L. El prototipo usa un ESP32 con seis entradas (pH con sonda PH-4502C, temperatura del líquido con DS18B20, nivel con HC-SR04, gases con MQ-135 como indicador cualitativo, temperatura y humedad ambiente con DHT11 e identificación del lote con RFID RC522) y cuatro salidas (bomba de aire, calentador sumergible, servo que dosifica la melaza y alarma con semáforo). Guarda un archivo CSV por lote en una tarjeta SD y sincroniza los datos con una base de datos en Supabase, consultable desde una página web y una aplicación móvil. También parte de un protocolo de control de calidad y trazabilidad elaborado por análisis documental y validado por juicio de experto.

**Fase 1. Ajuste del protocolo a una unidad productora (OE1, meses 1 a 3)**

*Actividad 1.1. Actualización documental.* Se revisan la guía técnica de biofábricas de Anacafé (Navarro, 2022), los factores de la activación líquida de MM (Castro-Barquero y González-Acuña, 2021), los criterios de buenas prácticas para biofábricas (Xavier et al., 2026) y la normativa del ICA (ICA, 2020).

*Actividad 1.2. Caracterización del proceso en la unidad productora.* Con el aval de la unidad que participa (se propone la Planta de Bioprocesos y Agroindustria de la Granja Tesorito de la Universidad de Caldas), se observa un lote de línea base producido con su método habitual. Se documentan materias primas, dosis de melaza y de MM sólido, volumen y tipo de recipiente, tiempos, forma de airear y criterios con que hoy se acepta un lote. El resultado es un diagrama del proceso con sus puntos críticos.

*Actividad 1.3. Ajuste de parámetros y rangos.* Se ajustan a esa unidad los valores de referencia del protocolo: temperatura de 30 °C ± 0,5 °C, ciclos de aireación de 15 minutos, dosis diaria de melaza y alertas de pH (crítico por debajo de 3,0; aviso si supera 6,5 después de 48 horas). El oxígeno disuelto no se mide de forma continua por su costo y mantenimiento: la aireación se controla y se registra, y el oxígeno se mide de forma puntual para validar.

*Actividad 1.4. Validación del protocolo.* El protocolo define qué es un lote, cómo se identifica, qué variables y eventos se registran y con qué frecuencia, y cómo se calibran las sondas, siguiendo la definición de trazabilidad de ISO 9000 (ISO, 2015). Lo validan el experto en bioprocesos del equipo y el responsable de la unidad, con una lista de chequeo.

**Fase 2. Adaptación del prototipo (OE2, meses 3 a 6)**

*Actividad 2.1. Hardware.* Se adapta el tanque al volumen de la unidad, se protege la electrónica de la humedad en una caja cerrada, se cambia la sonda de pH de laboratorio por una apta para inmersión prolongada y se agregan un convertidor analógico-digital externo (ADS1115) y un reloj de tiempo real (DS3231) para mejorar la precisión del pH y conservar la hora sin conexión.

*Actividad 2.2. Conectividad.* Se mide la cobertura de wifi y de datos móviles en el sitio y se configura la conexión disponible. El respaldo en tarjeta SD garantiza el registro cuando la red falla.

*Actividad 2.3. Pruebas en sitio.* Con agua y durante 24 horas se verifican la calibración del pH (buffers de pH 4 y 7), el control de temperatura, cada actuador y alarma, y la recuperación ante cortes de red y de energía.

**Fase 3. Validación en entorno relevante (OE3, meses 6 a 9)**

*Actividad 3.1. Lotes.* Se producen tres lotes consecutivos de activación líquida de MM de cinco días, siguiendo el protocolo de la fase 1.

*Actividad 3.2. Comparación con instrumentos de referencia.* Dos veces al día se comparan las lecturas del prototipo con un pH-metro y un termómetro de laboratorio. Se calculan el error medio absoluto, el sesgo y los límites de concordancia (Bland y Altman, 1986). Metas: error de pH ≤ 0,2 unidades y de temperatura ≤ 0,5 °C.

*Actividad 3.3. Comparación con el registro tradicional.* En paralelo, el personal de la unidad registra cada lote como lo hace hoy. Se comparan la información que deja cada método por lote, la completitud del registro (lecturas obtenidas frente a esperadas, con meta ≥ 99 %), la detección oportuna de desviaciones y el tiempo dedicado a registrar.

*Actividad 3.4. Desempeño del control y calidad del lote.* Se miden el porcentaje del tiempo con la temperatura en ± 0,5 °C del punto de consigna (meta ≥ 90 %), las alarmas y sus causas y la evolución del pH. Al cierre de cada lote se hacen recuentos microbianos en el laboratorio del grupo Alimentos y Agroindustria para relacionar, de forma exploratoria, las condiciones registradas con la calidad del bioinsumo.

**Análisis y presentación de resultados (meses 9 a 12)**

Los datos se analizan con estadística descriptiva, series de tiempo por lote y gráficos de concordancia. Los resultados se presentan en un informe técnico, un artículo, una ponencia, un póster y una demostración del sistema en la unidad productora.

**Roles.** Óscar Julián Sánchez Toro orienta el bioproceso, el protocolo, los recuentos microbianos y la validación; Diana Rocío Varón Serna dirige la adaptación del hardware y la comunicación; Karen Naghelly Moreno Ortiz administra la base de datos, procesa las series de tiempo y analiza los resultados.

**Consideraciones éticas.** El proyecto no incluye entrevistas ni encuestas a personas; la unidad productora participa con una carta de aval. Los bioinsumos de los lotes de prueba no se aplican a cultivos ni se comercializan y se inactivan antes de desecharlos. Las conexiones de 110 V van en una caja cerrada y el calentador se bloquea si el nivel es bajo.
