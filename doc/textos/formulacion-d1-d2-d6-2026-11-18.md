# Formulación VIP – d.1 y d.2 reconstruidos (2026-11-18)

Redactados de nuevo a partir de datos verificados (06-referencias), alineados con las fases inscritas y con los objetivos textuales del equipo (P15).

## d.1

Caldas tiene 59.058 hectáreas de café, 24.508 de plátano y 437.134 bovinos (Unidad de Planificación Rural Agropecuaria [UPRA], s. f.). Estas actividades dejan pulpa y mucílago de café, vástago y hojas de plátano y estiércol que, mal manejados, terminan en quebradas y potreros (Rodríguez-Valencia, 2023; Sánchez-Zuñiga et al., 2020; Solís et al., 2024). El estiércol puede portar patógenos que ni siquiera la digestión anaerobia elimina por completo (Lin et al., 2022), y el material vegetal de plátano enfermo disemina el Moko, sujeto a medidas fitosanitarias del Instituto Colombiano Agropecuario (ICA, 2021) y frente al cual muchos sistemas productivos de musáceas del país son vulnerables (Betancourt Vásquez et al., 2026).

Convertir esos residuos en bioinsumos dentro de la propia finca es una salida que el país está impulsando. Colombia importa cerca del 90 % de los fertilizantes que usa (Cortés Alvarado, 2026), y el Programa Nacional de Biofábricas priorizó 146 biofábricas en 21 departamentos y 279 municipios (Fernández Sosa, 2026). En Caldas, el Programa Biofábricas ya desarrolló experiencias de educación ambiental con comunidades rurales (Camacho Sánchez et al., 2024).

El problema es que multiplicar microorganismos es un proceso biológico que, en las biofábricas artesanales, se realiza sin registros. En Brasil, donde la producción en finca está más extendida, inoculantes multiplicados en finca mostraron alta contaminación con microorganismos no deseados (Bocatti et al., 2022), muchas muestras no contenían el microorganismo buscado (Kovalski et al., 2024), algunos productores controlaban sus lotes solo por color y olor, y apenas dos de 27 analizaban la causa de una contaminación (Xavier y Rodrigues, 2024). Llevar las prácticas del laboratorio a la finca es hoy uno de los principales retos de estos insumos (Goulet et al., 2025). En la activación líquida de microorganismos de montaña (MM), el tiempo, la aireación y la dosis de melaza cambian las poblaciones microbianas y el pH, y la aireación acorta la fermentación (Castro-Barquero y González-Acuña, 2021). Si esas condiciones no se miden, un lote puede desviarse sin que nadie lo note.

Además, para registrar y vender un bioinsumo, el ICA exige demostrar el control de calidad de la producción (ICA, 2020). Sin datos por lote, el productor no puede explicar por qué falló un lote, repetir las condiciones de los buenos ni mostrar evidencia ante la autoridad o el comprador. Brasil, por su parte, ya cuenta con un marco regulatorio para la producción en finca (Xavier et al., 2026).

Las herramientas disponibles no se ajustan a una biofábrica de vereda. Las estaciones que controlan bioprocesos están hechas para laboratorio (Eppendorf, s. f.), los medidores portátiles dependen de que alguien tome y anote cada lectura (Hanna Instruments, s. f.) y las plataformas de monitoreo en la nube cobran suscripciones mensuales (Ubidots, s. f.). Hay sistemas de bajo costo que monitorean biodigestores y biofertilizantes (Prasetyono et al., 2026; Ruge y Camargo, 2024), pero suponen una conexión permanente, y en 2025 solo el 56,9 % de los hogares de centros poblados y rural disperso tenía internet (Departamento Administrativo Nacional de Estadística [DANE], 2026).

**Pregunta de investigación.** ¿Cómo puede un sistema IoT de bajo costo medir, controlar y registrar el pH, la temperatura y la aireación de cada lote para dar trazabilidad a la producción artesanal de bioinsumos en las biofábricas de Caldas?

**Hipótesis.** Un sistema IoT de bajo costo basado en ESP32 registra el pH y la temperatura de cada lote con errores de hasta 0,2 unidades y 0,5 °C frente a instrumentos de laboratorio, mantiene la temperatura dentro de ±0,5 °C del punto de consigna al menos el 90 % del tiempo y conserva al menos el 99 % de los registros esperados aunque se pierda la conexión.

**Justificación.** El proyecto responde a una prioridad nacional: producir en el territorio bioinsumos de calidad documentada que reduzcan la dependencia de fertilizantes importados y den uso a residuos que hoy contaminan. También responde a una población que la Constitución reconoce como sujeto de especial protección, el campesinado (Congreso de la República de Colombia, 2023), con una herramienta que no le exige conexión permanente ni equipos de laboratorio. El equipo ya cuenta con una primera versión del prototipo en nivel de madurez TRL 4, construida y probada en laboratorio, de modo que el proyecto la desarrolla con base en el protocolo y evalúa su desempeño en entorno relevante (TRL 5).

**Aporte al conocimiento.** El proyecto generará un protocolo de control de calidad y trazabilidad para la producción de bioinsumos, aplicado a la activación líquida de MM, diseñado mediante el mapeo operativo de sus parámetros críticos de control y validado por juicio de experto; la primera serie de registros continuos de pH, temperatura y aireación por lote en condiciones de producción en Caldas, que permitirá ver cuándo y por qué se desvía un lote; y evidencia cuantitativa de la exactitud y la completitud de un sistema de bajo costo con respaldo sin conexión en comparación con los métodos tradicionales y con instrumentos de laboratorio. El código y el manual se publicarán como código abierto para que otras biofábricas puedan replicarlo.

## d.2

### Bioinsumos y biofábricas

Los bioinsumos son productos de origen biológico que se usan para el manejo de plagas o para mejorar los cultivos y los suelos (ICA, 2020). Su uso no es nuevo: la inoculación de leguminosas con rizobios cumple más de un siglo (Piccinetti et al., 2026). Lo nuevo es la escala que ha tomado su producción fuera de la industria. En Brasil y México, los insumos microbianos producidos por los propios agricultores crecieron rápido y obligaron a llevar prácticas de laboratorio a la finca (Goulet et al., 2025). Combinados con otras enmiendas, los biofertilizantes mejoran la salud del suelo y reducen el uso de fertilizantes de síntesis (Kumar et al., 2026), y los residuos agrícolas pueden convertirse en formulaciones con microorganismos benéficos, como la obtenida con *Trichoderma viride* a partir de biomasa de fríjol para manejar la pudrición radical causada por *Rhizoctonia solani* (Saber et al., 2026).

Una biofábrica es una instalación de finca o comunitaria donde se producen bioinsumos para autoconsumo o venta local. La guía técnica de Anacafé describe sus áreas, materias primas y procedimientos para la caficultura (Navarro, 2022). En Colombia, el Programa Nacional de Biofábricas prioriza su creación en los territorios (Fernández Sosa, 2026), y en Caldas ya hay experiencias documentadas por universidades y la Gobernación (Camacho Sánchez et al., 2024).

### Activación líquida de microorganismos de montaña

Los microorganismos de montaña (MM) se recolectan en la hojarasca de bosques cercanos, se reproducen en un sustrato sólido y luego se activan en un medio líquido con agua y melaza. La activación líquida es un proceso aeróbico que puede durar hasta 15 días. Castro-Barquero y González-Acuña (2021) mostraron que el tiempo, la aireación y la dosis de melaza cambian las poblaciones microbianas y el pH del caldo, y que la aireación acorta la fermentación. Por eso SIMBIO toma como parámetros críticos de control el pH y la temperatura, y trata la aireación como una condición de operación que se controla y se registra. El oxígeno disuelto sería la medida directa de la aireación, pero un sensor apto cuesta USD 169 solo en la tienda del fabricante (DFRobot, s. f.) y exige mantenimiento; por eso se mide solo de forma puntual para validar.

### Calidad y trazabilidad en la producción en finca

La trazabilidad es la capacidad de seguir el histórico, la aplicación o la localización de un objeto (Organización Internacional de Normalización [ISO], 2015). En un bioinsumo, el objeto es el lote: saber qué materias primas lo formaron, en qué condiciones se produjo y qué pasó cuando algo se desvió. La Resolución 68370 exige a los productores demostrar el control de calidad de lo que registran y venden (ICA, 2020).

La evidencia disponible muestra por qué hace falta. En Brasil, los inoculantes de *Bradyrhizobium* y *Azospirillum* multiplicados en finca presentaron alta contaminación con microorganismos no deseados (Bocatti et al., 2022); en biofábricas de Curitibanos, muchas muestras de bioinsumos de *Bacillus* y *Trichoderma* no contenían el microorganismo buscado (Kovalski et al., 2024), y la Embrapa evaluó la calidad de inoculantes de *Bacillus* producidos en finca (Lana et al., 2022). Entre 27 productores con biofábricas, algunos controlaban sus lotes solo por inspección visual y apenas dos analizaban la causa de una contaminación (Xavier y Rodrigues, 2024). Brasil respondió con un marco regulatorio para la producción en finca (Xavier et al., 2026). En Colombia ese marco aún depende del registro ante el ICA, que una biofábrica artesanal difícilmente puede cumplir sin datos de cada lote.

### Monitoreo y control de procesos biológicos con IoT

El internet de las cosas (IoT) es una infraestructura que interconecta objetos físicos y virtuales mediante tecnologías de información y comunicación (International Telecommunication Union [ITU], 2012). En bioprocesos, permite medir de forma continua, actuar sobre el proceso y guardar los datos en una base consultable a distancia.

Hay antecedentes cercanos. Al Mamun et al. (2024) monitorearon y controlaron en tiempo real un cultivo acuícola de biofloc. Prasetyono et al. (2026) monitorearon la producción y purificación de biogás y enviaron los datos a una plataforma en la nube con visualización en tiempo real. Díaz Andrade et al. (2025) construyeron una minicolumna de burbujeo con monitoreo y control por IoT para cultivos de levaduras. Ruge y Camargo (2024) presentaron un registrador de datos de bajo costo para la valorización de biofertilizantes, con una electrónica muy parecida a la de SIMBIO, y Nava Cuevas y Ballesteros Rincón (2025) un prototipo IoT para la fermentación del café en el sur del Huila. En la misma línea, Ruiz Rubiano y Góngora Herrera (2023), en la Universidad Distrital, montaron un sistema de telemetría con MQTT que registró el pH, la temperatura y el nivel de un tanque de biofertilizante de 2.500 L durante 15 días, sin controlar el proceso, y Champa et al. (2026) aplicaron control adaptativo a la fermentación doméstica de un fertilizante orgánico líquido en tanques de 30 L.

### Vigilancia tecnológica

La búsqueda incluyó productos comerciales, bases de patentes y literatura gris. La Tabla 1 resume las alternativas y por qué no resuelven el problema de una biofábrica de vereda.

[[TABLA1]]

### Vacío identificado

La literatura documenta que los bioinsumos de finca fallan y que las condiciones del proceso determinan su calidad. Ya existen sistemas que miden el pH y la temperatura en compost, biodigestores, cultivos de levaduras y plantas de biofertilizante, y uno que controla una fermentación doméstica. Sin embargo, no se encontró un trabajo que controle la temperatura y la aireación de cada lote, lo identifique y conserve su registro aunque falle la conexión, ni que se haya validado en biofábricas artesanales que transforman residuos de café, plátano y ganado. SIMBIO busca llenar ese vacío con un protocolo, un prototipo de bajo costo y una validación en una unidad productora de Caldas.

## d.6 (versión de 1 página)

**Productividad y competitividad.** Una biofábrica de vereda pasa de juzgar sus lotes por color y olor (Xavier y Rodrigues, 2024) a tener un registro por lote: ve cuándo y por qué se desvió un lote, repite las condiciones de los buenos y muestra el control de calidad que exige la Resolución 68370 (ICA, 2020). El protocolo y el prototipo pueden apoyar a las biofábricas priorizadas por el Programa Nacional de Biofábricas (Fernández Sosa, 2026).

**Económicos.** El sistema usa componentes de bajo costo y software libre, frente a equipos de laboratorio y plataformas con suscripción mensual (Tabla 1). Menos lotes perdidos significa menos melaza, inóculo y horas desperdiciados, y un bioinsumo de calidad documentada puede sustituir parte de los fertilizantes importados (Cortés Alvarado, 2026).

**Sociales.** Las familias campesinas, sujeto de especial protección (Congreso de la República de Colombia, 2023), reciben una herramienta en lenguaje sencillo para controlar un proceso que hoy depende de la experiencia de cada productor. El proyecto forma, además, a una estudiante de maestría.

**Ambientales.** Convertir residuos en bioinsumos de calidad conocida reduce la quema o el vertimiento de pulpa de café, vástago de plátano y estiércol (Rodríguez-Valencia, 2023; Solís et al., 2024) y el uso de fertilizantes de síntesis (Kumar et al., 2026).

**Culturales.** Los MM son un saber campesino. El sistema no reemplaza ese conocimiento, lo documenta: el productor sigue decidiendo, ahora con datos para explicar y repetir lo que funciona.

**Globales.** El proyecto aporta a los Objetivos de Desarrollo Sostenible 2, 12 y 13 y a la transición hacia los bioinsumos que Brasil ya regula (Xavier et al., 2026); el método se puede replicar en otras regiones cafeteras y plataneras.

**Efectos sobre el medio natural y la salud humana.** *Corto plazo:* los bioinsumos de finca pueden contener microorganismos no deseados (Bocatti et al., 2022; Lana et al., 2022), así que los lotes de prueba no se aplican ni se venden, se manipulan con guantes y tapabocas y se inactivan antes de desecharlos; el riesgo eléctrico se controla con una caja cerrada y el bloqueo del calentador por nivel bajo. *Mediano plazo:* la sonda de pH y la electrónica terminan como residuos y se entregarán en un punto de recolección de aparatos eléctricos y electrónicos. *Largo plazo:* registrar el pH, la temperatura y la aireación no garantiza la inocuidad; el protocolo deja claro que el sistema complementa los análisis microbiológicos que exige la norma y no los reemplaza.
