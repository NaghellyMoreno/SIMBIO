# Formulación VIP · d.4 Metodología propuesta (995 palabras)

**Última actualización:** 2026-11-15 · Texto listo para pegar en el formato VIP.

Enfoque. El proyecto es una investigación aplicada de desarrollo tecnológico con enfoque cuantitativo. Se organiza en tres fases, una por objetivo específico, y cada fase entrega el insumo de la siguiente. Cada componente se diseña, se prueba en simulación y luego en el prototipo, y los resultados ajustan el diseño.

### Fase 1. Protocolo de control de calidad y trazabilidad (OE1)

Actividad 1.1. Revisión de literatura sobre calidad de bioinsumos de finca y monitoreo de procesos biológicos con sensores (Bocatti et al., 2022; Castro-Barquero y González-Acuña, 2021; Ruge y Camargo, 2024).

Actividad 1.2. Mapeo operativo en una biofábrica o finca de Caldas. Mediante una visita de campo y una entrevista semiestructurada al productor, se documenta el procedimiento actual de activación líquida de microorganismos de montaña (MM): materias primas, dosis de melaza, tiempos, forma de airear, recipientes y criterios con que hoy se juzga un lote, que en la literatura suelen ser el color y el olor (Xavier y Rodrigues, 2024). El resultado es un diagrama del proceso con sus puntos críticos.

Actividad 1.3. Definición de los parámetros críticos y sus rangos. Se fijan el pH, la temperatura y la aireación como variables de control (Castro-Barquero y González-Acuña, 2021). El oxígeno disuelto no se mide de forma continua, por su costo y mantenimiento; la aireación se controla y se registra (estado de la bomba y minutos de operación), y el oxígeno disuelto se mide de forma puntual para validar. Se definen los valores de referencia: temperatura de 30 °C ± 0,5 °C, ciclos de aire de 15 minutos, dosis diaria de melaza y alertas de pH (crítico por debajo de 3,0; aviso si supera 6,5 después de 48 horas).

Actividad 1.4. Redacción del protocolo. El protocolo define qué es un lote, cómo se identifica (tarjeta RFID), qué variables se registran y con qué frecuencia (cada 60 segundos), qué eventos se anotan (inicio y cierre del lote, dosis, alarmas, calibraciones) y cómo se calibran las sondas. Sigue la definición de trazabilidad de ISO 9000: poder reconstruir el histórico de cada lote (ISO, 2015). Producto: protocolo validado por el equipo y el productor.

### Fase 2. Desarrollo del prototipo IoT (OE2)

Actividad 2.1. Diseño de la arquitectura. Se definen cuatro capas: sensores, control, almacenamiento y visualización, siguiendo prototipos similares con ESP32 y Arduino (Al Mamun et al., 2024; Prasetyono et al., 2026; Ruge y Camargo, 2024). Entradas: pH (PH-4502C con sonda E201), temperatura del líquido (DS18B20), nivel (HC-SR04), gases de fermentación como indicador cualitativo (MQ-135), temperatura y humedad ambiente (DHT11) e identificación del lote (RFID RC522). Salidas: bomba de aire, calentador sumergible de 25 W, servo como válvula de dosificación de melaza y alarma con buzzer y LED. El controlador es un ESP32 en un tanque de 10 L con unos 8 L de caldo.

Actividad 2.2. Firmware y simulación. Se programa en Arduino IDE el control de temperatura con histéresis y enclavamientos por nivel bajo, los ciclos de aireación, la dosificación de melaza, las alarmas, la calibración del pH en dos puntos (buffers de pH 4 y 7) con compensación por temperatura y el registro por lote. El firmware se prueba primero en Wokwi y luego en el hardware.

Actividad 2.3. Almacenamiento y visualización. Cada lectura se guarda en un archivo CSV por lote en una tarjeta microSD y se envía a una base de datos en Supabase (PostgreSQL), con tablas de lotes, lecturas y eventos. Si se pierde la conexión, los datos quedan en una cola local y se envían al recuperarla. Se desarrollan una página web y una aplicación móvil que muestran la última lectura, el histórico del lote y las alertas.

Actividad 2.4. Pruebas de laboratorio (TRL 4). Con agua y luego con un lote corto se prueban: calibración y repetibilidad del pH, respuesta del control de temperatura, funcionamiento de cada actuador y alarma, y cortes de wifi simulados para verificar que no se pierden registros. Producto: prototipo, código fuente y manual.

### Fase 3. Evaluación del prototipo (OE3)

Actividad 3.1. Piloto. Se producen tres lotes consecutivos de activación líquida de MM de cinco días cada uno, siguiendo el protocolo de la fase 1.

Actividad 3.2. Comparación con instrumentos de referencia. Dos veces al día se toman muestras y se comparan las lecturas del prototipo con un pH-metro y un termómetro de laboratorio; el oxígeno disuelto se mide con un oxímetro prestado para relacionarlo con los ciclos de aireación. Se calculan el error medio absoluto, el sesgo y los límites de concordancia; la meta es un error de pH menor o igual a 0,2 unidades y de temperatura menor o igual a 0,5 °C.

Actividad 3.3. Comparación con el método tradicional. En paralelo, el productor registra cada lote como lo hace hoy. Se comparan ambos métodos por la cantidad de información que dejan por lote, la completitud de los registros (lecturas obtenidas frente a esperadas), la capacidad de detectar desviaciones a tiempo y el tiempo que el productor dedica a registrar. Al cierre, el productor evalúa la utilidad del sistema en una encuesta corta.

Actividad 3.4. Desempeño del control. Se mide el porcentaje del tiempo en que la temperatura permanece dentro de ± 0,5 °C del punto de consigna, el número de alarmas y su causa, y el comportamiento del pH en los tres lotes.

### Análisis y presentación de resultados

Los datos se analizan con estadística descriptiva, gráficos de series de tiempo por lote y gráficos de concordancia con los instrumentos de referencia. Los resultados se presentan en un informe técnico, un artículo científico y una demostración del prototipo al productor.

Roles. Óscar Julián Sánchez Toro orienta el bioproceso, el protocolo y la validación; Diana Rocío Varón Serna dirige el hardware y la comunicación; Karen Naghelly Moreno Ortiz desarrolla la base de datos, procesa las series de tiempo y analiza los resultados.

Consideraciones éticas. La entrevista y la encuesta se hacen con consentimiento informado. Los bioinsumos producidos en el piloto no se comercializan, y las conexiones de 110 V van en una caja de paso cerrada.

