# Formulación VIP – Secciones d.5 a d.9 (SIMBIO), versión TRL 4 → 5

Actualizado el 2026-11-17 (noche) desde `formulacion-d5-d9-resultados-a-bibliografia.md`. Alineado con los objetivos de la formulación vigentes:

- **OG:** Desarrollar un sistema IoT de control y trazabilidad por lote para la producción artesanal de bioinsumos en biofábricas de Caldas.
- **OE1:** Ajustar un protocolo de control de calidad y trazabilidad para la activación líquida de microorganismos de montaña, construido por análisis documental y juicio de experto, al proceso real de una unidad productora de bioinsumos de Caldas, con los parámetros críticos de pH, temperatura y aireación.
- **OE2:** Adaptar el prototipo IoT de control y registro por lote (TRL 4) a las condiciones de operación de esa unidad: energía, conectividad, protección del equipo y volumen del tanque.
- **OE3:** Validar el sistema en entorno relevante (TRL 5) en tres lotes consecutivos de activación líquida de microorganismos de montaña, frente al registro tradicional y a instrumentos de referencia.

Duración: 12 meses. Según los términos de referencia de la convocatoria 2026/2027 (Tabla 4), un desarrollo tecnológico TRL ≥ 5 vale 80 puntos; el total de productos es 125.

---

## d.5 Resultados esperados

Cada resultado responde a un objetivo específico y tiene un medio para verificarlo. Los productos comprometidos suman 125 puntos de la Tabla 4 de la convocatoria, por encima del mínimo de 80.

### a. Generación de nuevo conocimiento

| Resultado | Indicador verificable | Medio de verificación | OE |
|---|---|---|---|
| Caracterización del proceso de activación líquida de microorganismos de montaña (MM) en la unidad productora: materias primas, dosis, volumen, tiempos, aireación y criterios de aceptación de un lote | Diagrama de proceso con los puntos críticos de control identificados | Diagrama de proceso y registro del lote de línea base | OE1 |
| Protocolo de control de calidad y trazabilidad por lote. Define los parámetros críticos (pH, temperatura y aireación) con sus rangos de referencia; el oxígeno disuelto se mide solo de forma puntual para validar. Define también la identificación del lote, la frecuencia de registro, los eventos que se anotan y la calibración de las sondas | Protocolo validado por juicio de experto y por el responsable de la unidad | Documento del protocolo y acta de validación firmada | OE1 |
| Conjunto de datos por lote de los tres lotes de la fase 3: series de tiempo de pH, temperatura y aireación cada 60 segundos, con los eventos de cada lote | Porcentaje de lecturas obtenidas frente a las esperadas (1.440 por día de lote) | Archivos CSV por lote en la microSD y tablas de lotes, lecturas y eventos en la base de datos | OE2, OE3 |
| Evaluación del desempeño del prototipo frente al método tradicional de registro (cuaderno, color y olor) y frente a instrumentos de referencia | Frente al método tradicional: información registrada por lote, completitud del registro, desviaciones detectadas a tiempo y tiempo dedicado a registrar. Frente a los instrumentos: error de pH ≤ 0,2 unidades y de temperatura ≤ 0,5 °C | Informe técnico de evaluación con series de tiempo, gráficos de concordancia | OE3 |
| Relación exploratoria entre las condiciones registradas y los recuentos microbianos de cada lote | Recuentos al cierre de los tres lotes | Informe de laboratorio del grupo Alimentos y Agroindustria | OE3 |
| **Artículo aceptado o publicado en revista no indexada** sobre el protocolo, el prototipo y su evaluación (10 puntos) | Un artículo aceptado o publicado | Carta de aceptación o enlace a la publicación | OE1–OE3 |

### b. Fortalecimiento de la comunidad científica colombiana

| Resultado | Indicador verificable | Medio de verificación |
|---|---|---|
| Formación de una estudiante de maestría: Karen Naghelly Moreno Ortiz (Maestría en Bioinformática y Biología Computacional) desarrolla parte de su trabajo de grado con la base de datos de trazabilidad y el análisis de las series de tiempo por lote | Una estudiante de maestría vinculada, con avance de tesis derivado del proyecto | Certificado de vinculación y constancia de avance del trabajo de grado firmada por el director |
| Cooperación entre GruCalPro (categoría B) y Alimentos y Agroindustria (categoría A1), que une la ingeniería de bioprocesos con la electrónica y el análisis de datos | Productos firmados por integrantes de ambos grupos | Productos registrados en el GrupLAC de los dos grupos |

### c. Apropiación social del conocimiento

La estrategia de comunicación tiene dos públicos. A la comunidad académica se le presentan los resultados en eventos nacionales. A la unidad productora se le transfieren el protocolo y el uso del prototipo.

| Resultado | Indicador verificable | Medio de verificación |
|---|---|---|
| **Ponencia presencial en un evento académico nacional** sobre el sistema de trazabilidad y su evaluación (20 puntos) | Una ponencia presentada | Certificado de ponente y memorias del evento |
| **Póster en un evento académico nacional** sobre el protocolo de control de calidad y trazabilidad (15 puntos) | Un póster presentado | Certificado de presentación del póster |
| Transferencia a la unidad productora: demostración del prototipo y entrega del protocolo y del manual de uso | Una sesión de demostración realizada | Lista de asistencia y registro fotográfico |

### d. Innovación tecnológica y desarrollo tecnológico

| Resultado | Indicador verificable | Medio de verificación |
|---|---|---|
| **Prototipo IoT de control y trazabilidad por lote validado en entorno relevante (TRL 5)** (80 puntos). Se basa en un ESP32, identifica cada lote con RFID, registra pH, temperatura y aireación y controla temperatura, aireación y dosis de melaza. Guarda los datos en tarjeta SD cuando no hay conexión y los sincroniza con una base de datos en la nube que se consulta desde una página web y una aplicación móvil | Tres lotes en la unidad productora con las metas de exactitud (pH ≤ 0,2; T ≤ 0,5 °C), control (≥ 90 % del tiempo en ±0,5 °C) y completitud (≥ 99 %) | Informe de validación, video de demostración y prototipo instalado |
| Paquete técnico replicable: firmware, esquema de la base de datos, web y app, lista de materiales con costos y manual de ensamblaje y uso | Código fuente documentado y manual entregado | Repositorio del código y manual técnico |

### Resumen de productos comprometidos (Tabla 4)

| Producto | Puntos |
|---|---|
| Desarrollo tecnológico, nivel de madurez mayor o igual a TRL 5 | 80 |
| Ponencia presencial en evento académico nacional | 20 |
| Póster en evento académico nacional | 15 |
| Artículo aceptado o publicado en revista no indexada | 10 |
| **Total** | **125** |

---

## d.6 Impactos esperados a partir del uso de los resultados

**Productividad y competitividad.** El principal impacto es que una biofábrica de vereda pase de juzgar sus lotes por color y olor (Xavier y Rodrigues, 2024) a tener un registro por lote de las condiciones de producción. Con ese historial, el productor puede ver en qué momento se desvió un lote, repetir las condiciones de los lotes buenos y mostrar evidencia de control de calidad, que es lo que exige la Resolución 68370 para registrar un bioinsumo (ICA, 2020). A mediano plazo, el protocolo y el prototipo pueden apoyar a las biofábricas priorizadas por el Programa Nacional de Biofábricas en su camino hacia el registro (Fernández Sosa, 2026).

**Económicos.** El prototipo usa componentes de bajo costo y software libre. Los equipos que hoy integran control y registro están pensados para laboratorio, y las plataformas comerciales de monitoreo cobran una suscripción mensual (Tabla 1). Si se pierden menos lotes, se desperdician menos melaza, inóculo y horas de trabajo. Además, un bioinsumo con calidad documentada puede reemplazar parte de los fertilizantes que el país importa (Cortés Alvarado, 2026). La validación en la unidad productora (TRL 5) es la que permite empezar a medir estos beneficios.

**Sociales.** Las familias campesinas, reconocidas como sujeto de especial protección (Acto Legislativo 01 de 2023), reciben una herramienta para controlar un proceso que hoy depende de la experiencia de cada productor. El protocolo, el manual y las alertas de la aplicación se escriben en lenguaje sencillo. El proyecto también forma a una estudiante de maestría en el análisis de datos de bioprocesos.

**Ambientales.** Cuando los residuos de la finca se convierten en bioinsumos de calidad conocida, se reduce la quema o el vertimiento de pulpa de café, vástago de plátano y estiércol a las quebradas (Rodríguez-Valencia, 2023; Sánchez-Zuñiga et al., 2020; Solís et al., 2024). También se reduce el uso de fertilizantes de síntesis (Kumar et al., 2026).

**Culturales.** Los microorganismos de montaña son un saber campesino: se recolectan en el bosque cercano y se multiplican con recetas que pasan de un productor a otro. El sistema no reemplaza ese conocimiento, lo documenta. El protocolo parte de las recetas en uso, y el productor sigue decidiendo cuándo aplicar un lote, ahora con datos que le permiten explicar y repetir lo que funciona.

**Globales.** El proyecto se alinea con los Objetivos de Desarrollo Sostenible 2 (hambre cero y agricultura sostenible), 12 (producción y consumo responsables) y 13 (acción por el clima), y con la transición mundial hacia los bioinsumos, en la que Brasil ya regula la producción en finca (Xavier et al., 2026). Un método de bajo costo para documentar la calidad de cada lote se puede replicar en otras regiones cafeteras y plataneras del trópico.

**Reflexión sobre efectos en el medio natural y la salud humana.**

- *Corto plazo (ejecución).* Los lotes de prueba son activaciones de MM a la escala de la unidad productora, y los bioinsumos multiplicados en finca pueden contener microorganismos no deseados o potencialmente patógenos (Bocatti et al., 2022; Lana et al., 2022). Por eso no se aplican a cultivos ni se comercializan. Se manipulan con guantes y tapabocas, y se inactivan antes de desecharlos según el protocolo del laboratorio donde se ejecuten. El riesgo eléctrico de tener equipos de 110 V junto a líquidos se controla con una caja de paso cerrada y con el bloqueo del calentador cuando el nivel es bajo. Las soluciones de calibración se desechan según su hoja de seguridad.
- *Mediano plazo.* El prototipo genera residuos electrónicos: la sonda de pH tiene una vida útil limitada y debe reemplazarse. Los componentes se reutilizan cuando es posible y, al final de su vida útil, se entregan en un punto de recolección de residuos de aparatos eléctricos y electrónicos. El consumo de energía es bajo (una bomba de aire y un calentador de 25 W).
- *Largo plazo.* Registrar el pH, la temperatura y la aireación no garantiza que el bioinsumo sea inocuo, y un productor podría confiar en el tablero y dejar de hacer los análisis microbiológicos que exige la norma (ICA, 2020). Por eso el protocolo deja claro que el sistema complementa esos análisis y no los reemplaza. Bien usado, el sistema debería reducir los lotes defectuosos que llegan al suelo y a los cultivos.

---

## d.7 Cronograma de actividades (12 meses)

| Fase / actividad | M1 | M2 | M3 | M4 | M5 | M6 | M7 | M8 | M9 | M10 | M11 | M12 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Aval de la unidad productora y del Comité de Ética | ■ | | | | | | | | | | | |
| **Fase 1. Protocolo (OE1)** | | | | | | | | | | | | |
| 1.1 Actualización documental | ■ | ■ | | | | | | | | | | |
| 1.2 Caracterización del proceso (lote de línea base) | | ■ | | | | | | | | | | |
| 1.3 Ajuste de parámetros y rangos (pH, temperatura y aireación) | | ■ | ■ | | | | | | | | | |
| 1.4 Validación del protocolo | | | ■ | | | | | | | | | |
| **Fase 2. Adaptación del prototipo (OE2)** | | | | | | | | | | | | |
| 2.1 Hardware (tanque, caja, sonda, ADS1115 y RTC) | | | ■ | ■ | ■ | | | | | | | |
| 2.2 Conectividad en el sitio | | | | | ■ | | | | | | | |
| 2.3 Pruebas en sitio | | | | | ■ | ■ | | | | | | |
| **Fase 3. Validación TRL 5 (OE3)** | | | | | | | | | | | | |
| 3.1 Tres lotes consecutivos de MM | | | | | | | ■ | ■ | | | | |
| 3.2–3.4 Comparaciones, desempeño del control y recuentos | | | | | | | ■ | ■ | ■ | | | |
| **Análisis y productos** | | | | | | | | | | | | |
| Análisis de datos | | | | | | | | ■ | ■ | | | |
| Demostración en la unidad productora | | | | | | | | | ■ | | | |
| Póster nacional | | | | | | | | | | ■ | | |
| Ponencia nacional | | | | | | | | | | | ■ | |
| Artículo (redacción, envío y seguimiento) | | | | | | | | | ■ | ■ | ■ | ■ |
| Informe final | | | | | | | | | | | ■ | ■ |

Las fechas de la ponencia y del póster dependen del calendario de los eventos. Si un evento cae en otro mes, se mueve la actividad sin afectar las demás.

---

## d.8 Compromisos

| Compromiso | Descripción | Plazo | Medio de verificación |
|---|---|---|---|
| Informe final | Informe técnico en el formato de la VIP con los resultados de los tres objetivos | Mes 12 | Informe radicado en la VIP |
| Producto de desarrollo tecnológico (80 puntos) | Prototipo IoT de control y trazabilidad por lote validado en TRL 5, con código fuente y manual. Es también el producto de apropiación (prototipo de un producto tecnológico) con el que se cumple el requisito de finalización del numeral 4 | Mes 9 | Informe de validación, video y prototipo instalado |
| Ponencia nacional (20 puntos) | Ponencia presencial en un evento académico nacional | Mes 11 | Certificado de ponente |
| Póster nacional (15 puntos) | Póster en un evento académico nacional | Mes 10 | Certificado de presentación |
| Artículo (10 puntos) | Artículo aceptado o publicado en revista no indexada | Mes 12 | Carta de aceptación o publicación |
| Formación de recurso humano | Vinculación de una estudiante de maestría que desarrolla parte de su trabajo de grado en el proyecto (requisito de la Tabla 3) | Meses 1–12 | Certificado de vinculación y constancia de avance |
| Ética e integridad | Carta de aval de la unidad productora y aval del Comité de Ética si lo exige | Mes 1 | Carta firmada y acta del Comité |
| Propiedad intelectual | Firma del acta de propiedad intelectual con la Universidad, si se determina necesaria | Según la VIP | Acta firmada |

Total de productos comprometidos: 125 puntos (mínimo exigido: 80).

---

## d.9 Bibliografía (APA 7)

Al Mamun, M. R., Ashik-E-Rabbani, M., Haque, M. M. y Upoma, S. M. (2024). IoT-based real-time biofloc monitoring and controlling system. *Smart Agricultural Technology, 9*, 100598. https://doi.org/10.1016/j.atech.2024.100598

Atlas Scientific. (s.f.). *pH Kit (KIT-101P)*. https://atlas-scientific.com

Betancourt Vásquez, J. A., Silva Escobar, O., Luque Sanabria, N. Y., Florez Yepes, G. Y., Betancourt Vásquez, M. y Rodríguez Yzquierdo, G. A. (2026). Edaphoclimatic factors related to vulnerability to Moko (*Ralstonia solanacearum* race 2) in Musaceae production systems in Colombia. *Bioagro, 38*(3), 335–350. https://doi.org/10.51372/bioagro383.3

Bland, J. M. y Altman, D. G. (1986). Statistical methods for assessing agreement between two methods of clinical measurement. *The Lancet, 327*(8476), 307–310. https://doi.org/10.1016/S0140-6736(86)90837-8

Bocatti, C. R., Ferreira, E., Ribeiro, R. A., Chueire, L. M. de O., Delamuta, J. R. M., Kobayashi, R. K. T., Hungria, M. y Nogueira, M. A. (2022). Microbiological quality analysis of inoculants based on *Bradyrhizobium* spp. and *Azospirillum brasilense* produced "on farm" reveals high contamination with non-target microorganisms. *Brazilian Journal of Microbiology, 53*, 267–280. https://doi.org/10.1007/s42770-021-00649-2

Burapha University. (2026). *Sistema de control de condiciones de producción en biorreactores anaerobios controlado por IoT* (Patente de modelo de utilidad TH28567U). https://eureka.patsnap.com/patent-TH28567U

Camacho Sánchez, O. A., Álvarez Vargas, C., Gallego López, J. P. y Enríquez Sandoval, M. F. (2024). *De la conciencia a la acción: sistematización de experiencias de educación ambiental en el marco del Programa Biofábricas en Caldas*. Universidad de Manizales; CINDE; Gobernación de Caldas; Minciencias.

Castro-Barquero, L. y González-Acuña, J. (2021). Factores relacionados con la activación líquida de microorganismos de montaña (MM). *Agronomía Costarricense, 45*(1), 81–92. https://doi.org/10.15517/rac.v45i1.45703

Champa, L., Hadthamard, N. y Thongpan, N. (2026). An IoT-enabled cyber-physical system architecture with adaptive control: A case study in household bio-fermentation. *Journal of Current Science and Technology, 16*(2), 186. https://doi.org/10.59796/jcst.V16N2.2026.186

Congreso de la República de Colombia. (2023). *Acto Legislativo 01 de 2023, por medio del cual se reconoce al campesinado como sujeto de especial protección constitucional*.

Cortés Alvarado, V. (2 de septiembre de 2026). Colombia quiere producir sus propios fertilizantes y reducir la dependencia externa. *AgroLatam*.

DFRobot. (s.f.). *Gravity: Analog dissolved oxygen / DO sensor meter kit for Arduino (SEN0237-A)*. https://www.dfrobot.com/product-1628.html

Díaz Andrade, C. A., Giraldo Rodríguez, I. y Caicedo Ortega, N. H. (2025). *Implementación de una mini-columna de burbujeo con sistemas de monitoreo y control apoyado en IoT para el estudio de cultivos de levaduras*. ACOFI. https://doi.org/10.26507/paper.4760

Eppendorf. (s.f.). *BioFlo® 120 bioprocess control station*. https://bioflo120.eppendorf.com

Fernández Sosa, I. (12 de mayo de 2026). Gobierno de Colombia impulsa Programa Nacional de Biofábricas. *Prensa Latina*.

Goulet, F., Fonteyne, S., López-Ridaura, S., Niederle, P., Odjo, S., Schneider, S., Verhulst, N. y Van Loon, J. (2025). The emergence of microbiological inputs and the challenging laboratorisation of agriculture: Lessons from Brazil and Mexico. *Agriculture and Human Values, 42*, 369–381. https://doi.org/10.1007/s10460-024-10614-y

Hanna Instruments. (s.f.). *Multiparameter pH/ORP/EC/TDS/salinity/DO/pressure/temperature waterproof meter – HI98194*. https://hannainst.com/multiparameter-ph-orp-ec-tds-salinity-do-pressure-temperature-waterproof-meter

Instituto Colombiano Agropecuario. (2020). *Resolución 68370 de 2020*.

Instituto Colombiano Agropecuario. (2021). *Resolución 92770 de 2021, por medio de la cual se establecen las medidas fitosanitarias para prevenir la diseminación en el territorio nacional de la enfermedad conocida como Moko del plátano y banano, ocasionada por la bacteria Ralstonia solanacearum Filotipo II secuevares 3, 4 y 6*.

International Telecommunication Union. (2012). *Overview of the Internet of things* (Recomendación ITU-T Y.2060). https://www.itu.int/rec/T-REC-Y.2060-201206-I

Kovalski, E., Passos, E. G. C., Ganen, J. S., Nogueira, M. A., Hungria, M. y Cruz, S. P. (2024). Análises microbiológicas de bioinsumos multiplicados a partir de *Bacillus* e *Trichoderma* em biofábricas na região de Curitibanos – SC. *Saúde e Meio Ambiente, 13*, 131–149. https://doi.org/10.24302/sma.v.13.5135

Kumar, S., Kumar, R. y Sindhu, S. S. (2026). Biochar and biofertilizers combined fertilization technology for modulating soil health, plant growth and environmental sustainability. *Total Environment Microbiology*, 100083.

Lana, U. G. de P., Oliveira-Paiva, C. A. de, Vilela, A. E., Gomes, E. A., Pires, V. L. M., Sousa, S. M. de y Valicente, F. H. (2022). *Avaliação da qualidade de inoculantes à base de Bacillus para promoção de crescimento de plantas produzidos em sistema on farm* (Boletim de Pesquisa e Desenvolvimento 238). Embrapa Milho e Sorgo.

Lin, M., Wang, A., Ren, L., Qiao, W., Wandera, S. M. y Dong, R. (2022). Challenges of pathogen inactivation in animal manure through anaerobic digestion: A short review. *Bioengineered, 13*(1), 1149–1161. https://doi.org/10.1080/21655979.2021.2017717

Nava Cuevas, D. F. y Ballesteros Rincón, J. A. (2025). Desarrollo de un prototipo automatizado para mejorar la fermentación del café mediante IoT en el sur del Huila. *Publicaciones e Investigación, 19*(3). https://hemeroteca.unad.edu.co/index.php/publicaciones-e-investigacion/article/view/10447

Navarro, Y. (2022). *Guía técnica: biofábricas. Producción de insumos biológicos para uso en la caficultura*. Asociación Nacional del Café (Anacafé).

Organización Internacional de Normalización. (2015). *Sistemas de gestión de la calidad — Fundamentos y vocabulario* (ISO 9000:2015).

Piccinetti, C. F., Rossi, A. O., Bortolato, M. A., Gallace, M. E. y Perticari, A. (2026). Ciento treinta años de inoculación de leguminosas con rizobios ¿cómo estamos y cómo seguimos? *Semiárida, 36*(1), 91–93. https://doi.org/10.19137/semiarida.2026(1).91-93

Prasetyono, S., Sarwono, C. S., Setyawan, D. L., Saleh, A., Kaloko, B. S. y An Nafi, M. N. (2026). IoT-based monitoring system for biodigester production and purification. *Journal of Mechatronics, Electrical Power, and Vehicular Technology, 17*(1), 29–38. https://doi.org/10.55981/j.mev.2026.1352

Precision Fermentation. (2023). *BrewIQ* (Patente US 11655444 B2). Oficina de Patentes y Marcas de Estados Unidos.

Rodríguez-Valencia, N. (2023). *Aplicación de la bioeconomía circular en el proceso de beneficio de café con cero residuos*. Cenicafé. https://doi.org/10.38141/cenbook-0032

Ruge, I. A. y Camargo, G. L. (2024). *Low-cost datalogger for biofertilizer valorization as a strategy for mitigating climate variability* [Ponencia]. LACCEI 2024, San José, Costa Rica. https://doi.org/10.18687/LACCEI2024.1.1.1835

Ruiz Rubiano, B. D. y Góngora Herrera, R. A. (2023). *Sistema de telemetría para procesos industriales utilizando tecnologías de internet de las cosas (IoT). Caso de estudio: planta de producción de biofertilizante* [Trabajo de grado, Universidad Distrital Francisco José de Caldas]. https://hdl.handle.net/11349/37520

Saber, W. E. I. A., Menaa, F., Ghoneem, K. M., Abdelkhalek, A. y Al-Askar, A. A. (2026). Decision tree-optimized biodegradation of bean biomass by *Trichoderma viride*: Bioactive formulation for rhizoctonia root rot management. *European Journal of Plant Pathology*. https://doi.org/10.1007/s10658-026-03317-z

Sánchez-Zuñiga, J. V., Sánchez-Molina, J. y Florez-Vargas, A. O. (2020). Caracterización de los residuos de la cosecha del plátano hartón para un potencial uso industrial. *Aibi Revista de Investigación, Administración e Ingeniería, 8*(3), 13–16. https://doi.org/10.15649/2346030X.821

Solís, H., Moreno, L. y García-Noguera, L. (2024). La contaminación del recurso hídrico en Colombia y sus mecanismos sociales de intervención y recuperación: una revisión de la literatura. *Ciencia Latina Revista Científica Multidisciplinar, 8*(6). https://doi.org/10.37811/cl_rcm.v8i6.15338

Ubidots. (s.f.). *Pricing & plan*. https://ubidots.com/pricing

Unidad de Planificación Rural Agropecuaria. (s.f.). *Caldas: microanálisis evaluaciones agropecuarias – EVAs*. https://upra.gov.co/Kit_Territorial/2-%20Informaci%C3%B3n%20por%20Departamentos/CALDAS/3-%20Microan%C3%A1lisis%20Evaluaciones%20agropecuarias%202023-Caldas.pdf

Xavier, V. L. y Rodrigues, R. W. da S. (2024). Perfil das biofábricas on farm e levantamento dos perigos biológicos potenciais na produção de bioinsumos. *Vigilância Sanitária em Debate, 12*, e02272. https://doi.org/10.22239/2317-269X.02272

Xavier, V. L., Martins do Vale, H. M. y de Almeida, J. R. M. (2026). Commercial and on-farm microbial bioinputs production systems: Challenges, opportunities and current Brazilian regulatory framework. *Brazilian Journal of Microbiology, 57*, 141. https://doi.org/10.1007/s42770-026-01955-3

### Notas sobre la bibliografía
- Son las referencias citadas en d.1, d.2, d.4 y d.6 con las correcciones del 2026-11-17 (noche): se quitaron Vu et al. (2026) y Sigma Electrónica (s.f.), porque sus frases salen del texto, y se agregaron las 8 nuevas de d.2, d.4 y d.6.
- En la Tabla 1 de d.2, el precio del sensor de oxígeno disuelto queda como "USD 169 en la tienda del fabricante (DFRobot, s. f.)".
