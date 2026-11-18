# SIMBIO – Entradas y salidas del prototipo

Nov 18, 2026 · @Karen

SIMBIO tiene **6 entradas** (sensores) y **4 salidas** (actuadores), más un agitador opcional. Un ESP32 lee las entradas, decide y mueve las salidas para mantener un lote de activación líquida de microorganismos de montaña (MM) en el garrafón de 10 L, con unos 8 L de caldo.

| Requisito | AyC | IoT | SIMBIO |
| --- | --- | --- | --- |
| Entradas | 4 o más | 5 o más | 6 |
| Salidas (luz y sonido cuentan como una) | 4 o más | 4 o más | 4 + 1 opcional |

Las variables controladas son la **temperatura** del caldo, la **aireación** y la **dosificación de melaza**. El pH, el nivel y los gases se miden para vigilar el lote y para bloquear acciones peligrosas. El oxígeno disuelto no se mide de forma continua: se registran los minutos de aireación y el OD se mide solo para validar, con un oxímetro prestado.

Configuración vigente: plan de pines del 12 nov 2026, sin ADS1115 ni RTC (decisiones H1–H18 de `doc/decisiones/03-hardware-prototipo.md`).

## Mapa del sistema

&#91;embedded content: SIMBIO · 6 entradas, control en el ESP32, 4 salidas + 1 opcional y salidas de datos\]

Cada sensor entra a un pin del ESP32; el ESP32 decide y mueve cada actuador por su propio pin, y guarda los datos en la SD y en Supabase. Los bordes punteados son lo opcional o lo que aún no está implementado.

## Entradas: 6 sensores

Dos entradas son analógicas (pH y gases) y pasan por un divisor de voltaje, porque los módulos entregan hasta 5 V y el ESP32 solo admite 3,3 V. Las otras cuatro son digitales.

| # | Variable | Sensor | Señal y pin del ESP32 | Rango y precisión | Para qué se usa |
| --- | --- | --- | --- | --- | --- |
| E1 | Temperatura del caldo | DS18B20 sumergible | Digital 1-Wire, GPIO 4 (pull-up de 4,7 kΩ) | −55 a 125 °C; ± 0,5 °C | Es la variable controlada: decide el calentador. También compensa la lectura de pH |
| E2 | pH del caldo | PH-4502C + sonda E201 | Analógica 0–5 V → divisor → GPIO 36 (ADC1) | 0–14; ± 0,2 pH esperado sin ADS1115 | Sigue la acidificación del lote. Alarma crítica si baja de 3,0; aviso si sigue sobre 6,5 después de 48 h |
| E3 | Nivel del caldo | HC-SR04 en la tapa | Pulso digital: Trig GPIO 25, Echo GPIO 34 (con divisor) | 2–400 cm; \~0,3 cm | Calcula el volumen. Bloquea el calentador bajo 19 cm y la dosis de melaza sobre 25 cm |
| E4 | Gases de fermentación | MQ-135 en la tapa | Analógica 0–5 V → divisor → GPIO 39 (ADC1) | Índice cualitativo 0–100 | Indica actividad fermentativa o anomalías. Aviso sobre 60. No mide concentración de CO₂ |
| E5 | Identificación del lote | Lector RFID RC522 + tarjetas | SPI: SS GPIO 5, RST GPIO 27, bus 18/19/23 | UID de la tarjeta | Arranca el lote y le da su identificador. Cada registro queda ligado a ese lote (trazabilidad) |
| E6 | Temperatura y humedad ambiente | DHT11 | Digital, GPIO 13 | 0–50 °C ± 2 °C; 20–90 % HR ± 5 % | Contexto del lote: explica las pérdidas de calor y sirve para compensar el MQ-135 |

La precisión del pH es la principal limitación de esta fase: con el ADC del ESP32 se espera ± 0,2 pH, y con un ADS1115 (fase con patrocinio) ± 0,1. Se calibra con buffers de pH 4 y 7 y se compensa con la temperatura del DS18B20.

## Salidas: 4 actuadores + 1 opcional

Las dos cargas de 110 V (bomba y calentador) se conmutan con relés dentro de la caja de paso; el servo y la alarma trabajan a 5 V desde el riel del L7805.

| # | Actuador | Interfaz y pin del ESP32 | Variable que mueve | Regla que lo enciende o lo apaga |
| --- | --- | --- | --- | --- |
| S1 | Bomba de aire Xilong XL-1187 (110 V) con piedra difusora | Relé de 1 canal, GPIO 21 | Aireación | Ciclos de 15 min encendida y 15 min apagada mientras hay lote activo. Los minutos de aire se acumulan y se registran, en lugar de medir el OD |
| S2 | Calentador sumergible de 25 W (110 V) con termostato | Relé de 1 canal, GPIO 22 | Temperatura | ON-OFF con consigna de 30 °C e histéresis de ± 0,5 °C. Se apaga siempre si T ≥ 38 °C, si el nivel baja de 19 cm o si falta la lectura de T o de nivel. Su termostato propio (perilla en 32 °C) queda como respaldo |
| S3 | Válvula de melaza: servo SG90 que aprieta la manguera (pinza) | PWM de 50 Hz, GPIO 14 | Dosificación de melaza | Abre una vez cada 24 h durante 8 s (tiempo a recalibrar con melaza en el tramo 11). Si el nivel pasa de 25 cm o no hay lectura de nivel, pospone la dosis 10 min |
| S4 | Alarma: semáforo de LEDs + buzzer KY-012 | LEDs: rojo GPIO 16, amarillo 17, verde 2. Buzzer por 2N2222, GPIO 15 | Aviso al operario | Verde: lote sin novedades. Amarillo: aviso. Rojo + buzzer: alarma crítica. El pulsador OK silencia el buzzer |
| S5 | Motor agitador (opcional) | Relé o transistor con diodo de rueda libre; pin por definir | Mezcla del caldo | Por decidir: el equipo aún no define si se incluye |

Para AyC está previsto un control **PI por ventana de tiempo** del calentador como alternativa al ON-OFF, y un servidor **Modbus TCP** para leer entradas y salidas desde LabVIEW (decisiones M4, M9 y M10; aún no implementados).

## Qué entrada decide cada salida

La temperatura es la única entrada que regula una salida de forma continua (el calentador). El nivel funciona como enclavamiento de seguridad, y la tarjeta RFID habilita todo: sin lote activo, o con el DIP de calibración en ON, el firmware deja las tres salidas de proceso apagadas.

| Salida | E1 Temperatura | E2 pH | E3 Nivel | E4 Gases | E5 RFID | E6 Ambiente |
| --- | --- | --- | --- | --- | --- | --- |
| S1 Bomba de aire | — | — | — | — | Habilita | — |
| S2 Calentador | **Regula** (30 ± 0,5 °C); apaga si ≥ 38 °C o sin lectura | — | **Bloquea** bajo 19 cm o sin lectura | — | Habilita | — |
| S3 Válvula de melaza | — | — | **Pospone** sobre 25 cm o sin lectura | — | Habilita | — |
| S4 Alarma | Crítica / aviso | Crítica / aviso | Crítica | Aviso | Aviso | Aviso |

El pH, los gases y el ambiente no mueven actuadores: se registran y avisan al operario, que decide (por ejemplo, revisar contaminación si el pH no baja).

**Alarmas críticas** (LED rojo + buzzer): nivel bajo (< 19 cm), nivel alto (> 26 cm), sin lectura de nivel, temperatura alta (≥ 38 °C), DS18B20 sin respuesta y pH demasiado ácido (< 3,0).

**Avisos** (LED amarillo): temperatura a más de 2 °C de la consigna después de 2 h, pH sobre 6,5 después de 48 h, sonda de pH desconectada o sin calibrar, gases sobre 60, DHT11 o lector RFID sin respuesta, SD no disponible y sin wifi.

## Mandos del operario y salidas de datos

Estos elementos completan el sistema, pero no se cuentan como entradas ni salidas para el requisito de los cursos: no miden una variable del proceso ni actúan sobre ella.

**Mandos del operario (entradas digitales)**

| Mando | Pin | Función |
| --- | --- | --- |
| Pulsador LOTE | GPIO 32 (pull-up interno) | Con la tarjeta recién leída, inicia el lote; sostenido 2,5 s, lo cierra |
| Pulsador OK | GPIO 33 (pull-up interno) | Silencia el buzzer; confirma cada punto en la calibración de pH |
| DIP de calibración | GPIO 35 (pull-up externo de 10 kΩ) | En ON pone el sistema en modo calibración de pH, con las salidas de proceso apagadas |

**Salidas de datos**

| Destino | Qué recibe | Para qué |
| --- | --- | --- |
| Tarjeta SD (módulo SPI, CS GPIO 26) | Un CSV por lote, un registro por minuto, y la cola de envíos pendientes | Respaldo sin conexión |
| Supabase (base de datos en la nube) | Tablas `lotes`, `lecturas` y `eventos`; vista `ultima_lectura` | Consulta del lote desde la web y la app |
| Web y app | La vista `ultima_lectura` y el histórico del lote | Seguimiento remoto (por construir) |
| Monitor serie | Estado y comandos (`estado`, `dosis`, `csv`…) | Montaje y pruebas |
| Modbus TCP → LabVIEW | Registros 30001–30010 y 40001–40004 (decisión M9) | SCADA de AyC (por implementar) |

Cada registro lleva la hora de NTP (o la marca "hora estimada" tras un corte sin red) y el tiempo transcurrido desde el inicio del lote.
