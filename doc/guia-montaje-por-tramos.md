# SIMBIO – Guía de montaje por tramos

> Copia en Markdown del doc "SIMBIO – Guía de montaje por tramos" (https://claude.ai/artifact/QbPEWdqhXDLZFjRGxTH8FU), revisión del 2026-11-18. Sketches en `Universidad/SIMBIO/Simulacion/pruebas_por_tramo/`. Si se edita el doc, volver a copiarlo aquí.

Nov 18, 2026 · @Karen

## Cómo usar esta guía

El prototipo se monta en 12 tramos (0 a 11). Cada tramo agrega una sola parte, se prueba con su propio sketch y solo se pasa al siguiente cuando cumple su criterio de aceptación. La melaza entra en el último tramo, cuando todo lo demás ya funcionó con agua.

1. **Tramo 0** – Preparación: software, librerías, herramientas y mesa.
2. **Tramo 1** – Alimentación: adaptador Techman + L7805 → riel de 5 V.
3. **Tramo 2** – ESP32 solo: carga de programas, wifi y hora NTP.
4. **Tramo 3** – Interfaz local: LEDs, buzzer, pulsadores y DIP.
5. **Tramo 4** – Temperaturas: DS18B20 (líquido) y DHT11 (ambiente).
6. **Tramo 5** – Nivel: HC-SR04 en la tapa del garrafón.
7. **Tramo 6** – Bus SPI: módulo SD y lector RFID, primero por separado y luego juntos.
8. **Tramo 7** – Analógicos: MQ-135 y pH, con divisor.
9. **Tramo 8** – Potencia: relés sin 110 V, caja de paso, bomba de aire y calentador **con agua**; prueba escalón para AyC.
10. **Tramo 9** – Servo y válvula de pinza **con agua**.
11. **Tramo 10** – Firmware completo con agua: calibración de pH, lote de prueba, cortes de red y de energía, Supabase.
12. **Tramo 11** – **Melaza** (última prueba): volumen por dosis y obstrucción de la manguera.

**Reglas que valen para todos los tramos**

- Desconectar la alimentación antes de mover un solo cable. Conectar, revisar con el multímetro y después energizar.
- Ningún pin del ESP32 recibe más de 3,3 V. El pH, el MQ-135 y el Echo del HC-SR04 llevan divisor.
- El RC522, el DHT11 y la pull-up del DS18B20 van a **3,3 V**. El resto de módulos, al riel de 5 V.
- Todos los GND unidos en un solo punto (GND común).
- Mientras el ESP32 esté conectado por USB al computador, no alimentarlo también desde el riel de 5 V: se desconecta el cable del riel al pin 5V/VIN y se deja solo el GND común.
- Los 110 V nunca tocan la protoboard. Solo existen dentro de la caja de paso cerrada (tramo 8).
- Cada tramo aprobado se anota en la hoja de registro del final, con fecha, valores medidos y una foto (sirven para el artículo y el informe de AyC).

**Presupuesto de corriente del riel de 5 V** (valores típicos de hoja de datos; se comprueban en el tramo 10)

| Carga | Corriente típica | Pico |
| --- | --- | --- |
| ESP32 con wifi | 80–160 mA | \~350 mA al transmitir |
| MQ-135 (calefactor) | 150 mA | 150 mA |
| 2 relés (bobinas) | 2 × 70 mA | 140 mA |
| Servo SG90 | 10 mA quieto | 500–700 mA al moverse o forzado |
| Módulo SD + RC522 + sensores + LEDs + buzzer | \~100 mA | \~200 mA |
| **Total** | **\~0,5 A** | **\~1,5 A** |

El L7805 da hasta 1,5 A y se calienta en proporción a (V de entrada − 5 V) × corriente. Por eso el tramo 1 mide el riel con carga y el tramo 9 lo vuelve a medir con el servo moviéndose.

**Calendario sugerido** (entrega final el 27 nov; el lote de MM dura 5 días)

| Día | Tramos |
| --- | --- |
| Mié 18 nov | 0 a 5 |
| Jue 19 nov | 6 a 8; escalón de temperatura durante la noche |
| Vie 20 nov | 9 y 10; corrida con agua durante la noche |
| Sáb 21 nov | 11 (melaza) en la mañana; inicio del lote de MM |
| 21–26 nov | Lote de MM; web, app y LabVIEW en paralelo |

Este calendario corre dos días el inicio del lote frente al plan del 17 nov (lote del 19 al 24). Hay que confirmarlo con el equipo.

## Tramo 0 – Preparación

Objetivo: tener el software, las librerías y la mesa listos antes de tocar un cable. Criterio: el sketch del tramo 2 compila sin errores.

**Software (en el Mac)**

1. Arduino IDE 2 con la placa **esp32 by Espressif Systems** (Gestor de placas). Placa: **ESP32 Dev Module**.
2. Librerías (Gestor de librerías), las mismas del firmware principal: **OneWire** (Paul Stoffregen), **DallasTemperature** (Miles Burton), **DHT sensor library for ESPx** (beegee\_tokyo), **MFRC522** (GithubCommunity / miguelbalboa) y **ArduinoJson** 7 (Benoit Blanchon).
3. Copiar la carpeta `pruebas_por_tramo` a `Universidad/SIMBIO/Simulacion/` (ya está ahí si el computador estaba conectado cuando se generó esta guía).
4. Monitor serie: 115200 baudios y fin de línea **"Nueva línea"**. Todos los sketches de prueba reciben comandos así.

**Materiales sobre la mesa**

- [ ] ESP32 + placa de expansión, cable USB de datos (no solo de carga)
- [ ] Protoboard, jumpers macho–macho y macho–hembra, cable rígido
- [ ] Multímetro con pilas buenas
- [ ] Resistencias: 330 Ω (×3), 680 Ω (×1), 820 Ω (×2), 1 kΩ (×6), 4,7 kΩ (×1), 10 kΩ (×1)
- [ ] Capacitores: 0,1 µF (×3), 0,33 µF (×1), 100 µF (×1), 470 µF (×2)
- [ ] 1N4007, 2N2222, L7805 con disipador
- [ ] Tarjeta microSD formateada en **FAT32** (32 GB o menos) + adaptador a SD
- [ ] Garrafón de 10 L limpio, regla o cinta métrica, jarra medidora, balanza de cocina
- [ ] Agua destilada para enjuagar la sonda de pH, papel absorbente, buffers pH 4 y 7

**Mesa**

Separar dos zonas: la de baja tensión (protoboard, ESP32, sensores) y la de agua (garrafón). La protoboard nunca queda debajo ni al lado del garrafón: una salpicadura la cortocircuita.

## Tramo 1 – Alimentación (adaptador + L7805)

Objetivo: un riel de 5 V estable que aguante la carga de todo el prototipo. Criterio: 4,90–5,10 V sin carga y **al menos 4,85 V con carga**, con el disipador tibio (se puede tocar 5 s).

**Montaje**

1. Con el adaptador Techman desenchufado, ponerlo en **7,5 V**. Enchufarlo y medir su plug sin carga: anotar el voltaje y cuál contacto es el positivo (centro o exterior).
2. Identificar las patas del L7805: mirándolo de frente (letras hacia usted, patas abajo) son **1 entrada, 2 GND, 3 salida**. La aleta metálica es GND; atornillar el disipador con pasta térmica si hay.
3. Positivo del adaptador → **1N4007** (la franja hacia el L7805) → pata 1. El diodo protege si el plug se conecta al revés.
4. En la pata 1: 100 µF (pata larga al +) y 0,33 µF a GND, lo más cerca posible del regulador.
5. En la pata 3: 0,1 µF y 470 µF (pata larga al +) a GND. De ahí sale el **riel de 5 V** de la protoboard.
6. Negativo del adaptador → pata 2 → riel de GND.

**Prueba**

1. Sin nada conectado al riel: medir. Debe dar 4,90–5,10 V.
2. Carga de prueba: conectar el **MQ-135** al riel (VCC y GND; la salida AO todavía sin conectar). Su calefactor consume \~150 mA, y así empieza de una vez su precalentamiento de 24–48 h. Dejarlo encendido desde hoy.
3. Con el MQ-135 conectado, medir el riel y la entrada del L7805 (pata 1). Esperar 10 min y tocar el disipador.

**Si falla**

- Riel por debajo de 4,85 V con carga: el adaptador a 7,5 V queda en \~6,7 V después del 1N4007 y el L7805 necesita unos 7 V para regular. Pasar el adaptador a **9 V** y repetir. A 9 V el regulador disipa \~1,6 W con 0,5 A: el disipador lo aguanta, pero debe estar bien atornillado.
- Disipador que quema: revisar que no haya un corto en el riel (medir resistencia entre 5 V y GND con todo apagado; debe ser mayor a unos cientos de ohmios).
- 0 V a la salida: polaridad del plug invertida (el 1N4007 la bloqueó) o patas del L7805 cambiadas.

## Tramo 2 – ESP32 solo (carga, wifi y hora)

Objetivo: comprobar que el ESP32 se programa, se conecta a la red del sitio donde estará el tanque y obtiene la hora. Criterio: wifi conectado con RSSI mejor que −70 dBm y hora NTP correcta (UTC−5).

**Montaje:** el ESP32 en la placa de expansión, conectado solo por USB. Nada más.

**Código:** `T02_esp32_wifi_ntp`. Antes de cargar, escribir el nombre y la clave de la red en `WIFI_SSID` y `WIFI_PASS` (la red debe ser de **2,4 GHz**; si se usa el celular como módem, activar la opción de compatibilidad 2,4 GHz).

**Prueba**

1. Cargar el sketch. Si se queda en `Connecting....`, mantener presionado **BOOT** hasta que empiece a escribir.
2. Abrir el monitor serie: debe mostrar el modelo del chip, `OK IP`, el RSSI y la hora.
3. Llevar el portátil con el ESP32 al sitio exacto del tanque y leer el RSSI allí. Entre −50 y −70 dBm es buena señal; peor que −80 dBm habrá cortes frecuentes.
4. Apagar el wifi del router o del celular 1 minuto: el monitor debe decir `Wifi caído: reintentando` y reconectarse solo al volver la red.

**Después de aprobar:** pasar el ESP32 al riel de 5 V del tramo 1 (cable del riel al pin 5V/VIN de la placa de expansión, GND común) y confirmar que el LED verde (GPIO 2) sigue parpadeando sin USB. A partir de aquí, cuando se conecte el USB para cargar un programa, se desconecta ese cable de 5 V.

## Tramo 3 – Interfaz local (LEDs, buzzer, pulsadores y DIP)

Objetivo: tener el semáforo, la alarma sonora y los mandos del operario funcionando, porque todos los tramos siguientes los usan para avisar. Criterio: los 3 LEDs, el pitido, los dos pulsadores y el DIP responden como dice el sketch.

**Conexiones**

| Elemento | Conexión |
| --- | --- |
| LED rojo | GPIO 16 → 330 Ω → ánodo (pata larga); cátodo a GND |
| LED amarillo | GPIO 17 → 330 Ω → LED → GND |
| LED verde | GPIO 2 → 330 Ω → LED → GND |
| Buzzer KY-012 | Pin S a 5 V; pin − al colector del 2N2222; emisor a GND; base ← 1 kΩ ← GPIO 15 |
| Pulsador LOTE | GPIO 32 ↔ GND (pull-up interno) |
| Pulsador OK | GPIO 33 ↔ GND (pull-up interno) |
| DIP calibración | 10 kΩ de GPIO 35 a 3,3 V; interruptor de GPIO 35 a GND |

El 2N2222 mirado por la cara plana, patas abajo, suele ser **E–B–C** de izquierda a derecha; confirmarlo con la hoja de datos del que tengan (hay versiones con otro orden).

**Código:** `T03_interfaz_local`.

**Prueba**

1. Al arrancar: rojo, amarillo, verde en secuencia y un pitido.
2. Mantener LOTE: LED rojo encendido, pitido corto y `Pulsador LOTE` en el monitor. Igual con OK y el amarillo.
3. DIP en ON: LED verde encendido y `DIP: MODO CALIBRACION`. DIP en OFF: `modo operación`.
4. Presionar el botón EN (reinicio) varias veces: el ESP32 debe arrancar siempre. Si con el LED en GPIO 2 o el transistor en GPIO 15 el ESP32 no arranca o no deja cargar programas, avisar: son pines de arranque y habría que moverlos.

## Tramo 4 – Temperaturas (DS18B20 y DHT11)

Objetivo: lectura confiable de la temperatura del caldo, que es la variable controlada, y del ambiente. Criterio: DS18B20 a 0,0 ± 0,5 °C en agua con hielo y a ± 0,5 °C del termómetro de referencia en agua tibia; DHT11 con lectura estable.

**Conexiones**

| Sensor | Conexión |
| --- | --- |
| DS18B20 sumergible | Rojo 3,3 V, negro GND, amarillo (DATA) GPIO 4; **4,7 kΩ entre DATA y 3,3 V** |
| DHT11 (módulo) | VCC 3,3 V, GND, DATA GPIO 13 (el módulo ya trae la pull-up) |

Si el cable del DS18B20 trae otros colores, buscar la hoja de la publicación: a veces DATA es blanco o azul.

**Código:** `T04_temperaturas` (librerías OneWire, DallasTemperature y DHT sensor library for ESPx). Imprime una línea CSV cada 2 s.

**Prueba**

1. Al arrancar debe decir `Sensores DS18B20 encontrados: 1`.
2. Ambiente: DS18B20 al aire junto al DHT11 durante 5 min. Deben coincidir en ± 2 °C (el DHT11 es de ± 2 °C).
3. **Agua con hielo** (mucho hielo picado y poca agua, revolver 2 min): el DS18B20 debe leer 0,0 ± 0,5 °C.
4. **Agua tibia** (\~30 °C) junto al termómetro de referencia del laboratorio, si ya lo tienen: anotar los dos valores. Esta es la primera evidencia para la meta del OE3 (error ≤ 0,5 °C).
5. Copiar 10 líneas del monitor a la hoja de registro.

**Si falla:** −127 = DATA suelto o sin la resistencia de 4,7 kΩ. 85 = el sensor se reinició (alimentación inestable). DHT11 con error de tiempo = cable largo o VCC flojo.

## Tramo 5 – Nivel (HC-SR04 en la tapa)

Objetivo: medir el nivel del caldo, que bloquea el calentador si baja de 19 cm y la dosis de melaza si pasa de 26 cm. Criterio: ± 0,5 cm frente a la regla en la mesa y en el garrafón, sin lecturas "sin eco" en 5 minutos.

**Conexiones:** VCC 5 V, GND, Trig GPIO 25. **Echo → 680 Ω → GPIO 34**, y de GPIO 34 → 1 kΩ → GND. Con eso los 5 V del Echo llegan como \~3,0 V. Medir ese punto con el multímetro mientras el sensor dispara (debe ser menor que 3,3 V).

**Código:** `T05_nivel_hcsr04`. Comandos: `c` (calibrar el fondo con el garrafón vacío) y `v <litros>` (calcular el área con un volumen conocido).

**Prueba en la mesa**

1. Apuntar el sensor a un libro a 10, 20 y 30 cm medidos con regla. Anotar la distancia que da el monitor en cada punto.

**Montaje en la tapa**

1. Hacer dos agujeros en la tapa para los transductores, de modo que el sensor quede mirando **perpendicular** al líquido y centrado. El haz se abre unos 15°: a 30 cm cubre un círculo de \~8 cm. Si la boca del garrafón es más angosta, el eco rebota en el cuello: hay que ampliar la boca o usar una tapa más ancha.
2. La placa del sensor queda por fuera de la tapa, cubierta con una bolsa o cinta para protegerla de la condensación y de salpicaduras de la bomba de aire.
3. Con el garrafón **vacío** y la tapa puesta, enviar `c`. Anotar el valor que da para `DIST_SENSOR_FONDO_CM`.
4. Marcar el garrafón por fuera cada 2 cm con la regla. Agregar agua con la jarra de 2 L en 2 L y, después de cada adición, enviar `v` con los litros totales (`v 2`, `v 4`, `v 6`, `v 8`). Comparar el nivel del monitor con la marca y anotar el área calculada; el promedio va a `AREA_TANQUE_CM2`.
5. Con 8 L, dejar correr 5 minutos: no debe aparecer `Sin eco`.

El sketch corrige la velocidad del sonido a 20 °C. Si el aire bajo la tapa está a 30 °C, la lectura cambia \~0,5 cm a 30 cm de distancia: es aceptable y se verá en el tramo 10.

## Tramo 6 – Bus SPI: módulo SD y lector RFID

Objetivo: guardar datos sin conexión (SD) e identificar el lote (RFID) compartiendo los mismos tres cables SPI. Criterio: cada módulo pasa su prueba solo, y los dos juntos funcionan 10 minutos sin que el RC522 se pierda.

**Conexiones**

| Señal | Módulo SD | RC522 |
| --- | --- | --- |
| Alimentación | VCC **5 V** (trae regulador) | 3.3V a **3,3 V** (a 5 V se daña) |
| GND | GND | GND |
| CS / SDA | GPIO 26 | GPIO 5 |
| SCK | GPIO 18 | GPIO 18 |
| MISO | GPIO 19 | GPIO 19 |
| MOSI | GPIO 23 | GPIO 23 |
| RST | — | GPIO 27 |
| IRQ | — | sin conectar |

Cables SPI cortos (menos de 15 cm) y del mismo largo; a 4 MHz los cables largos producen errores.

**Pruebas, en este orden**

1. **6a – SD sola** (`T06a_sd`), sin el RC522: debe mostrar el tipo y tamaño de la tarjeta y terminar en `PRUEBA SD: OK`. Sacar la tarjeta, abrirla en el Mac y confirmar que `prueba_t06.csv` tiene 11 líneas.
2. **6b – RC522 solo** (`T06b_rfid`), con la SD desconectada: debe decir `Versión del chip: 0x91` o `0x92`. Pasar las 3 tarjetas o llaveros, anotar el UID de cada uno y marcarlos con marcador (Lote 1, 2, 3). El segundo RC522 se prueba igual y se guarda de repuesto.
3. **6c – Los dos juntos** (`T06c_spi_compartido`): pasar tarjetas durante 10 minutos. El contador de lecturas guardadas debe subir y `fallos RC522` debe quedar en 0.

**Si en 6c el RC522 falla solo cuando la SD está conectada:** algunos módulos SD no sueltan la línea MISO. La salida documentada es mover la SD al segundo bus SPI del ESP32 (HSPI). Ese cambio toca el firmware (`registro.cpp`) y se hace aparte; avisar antes de seguir.

## Tramo 7 – Analógicos: MQ-135 y pH

Objetivo: leer las dos señales de 0–5 V con el ADC1 del ESP32 sin pasar de 3,3 V. Criterio: MQ-135 que sube y vuelve con alcohol o vinagre; pH con pendiente entre 100 y 250 mV/pH y lectura de 7,00 ± 0,10 al volver al buffer de pH 7.

**El divisor (igual para los dos)**

Señal del módulo → **1 kΩ** → nodo → GPIO. Del nodo a GND: **1 kΩ + 820 Ω en serie**. Entre el GPIO y GND, un capacitor de **0,1 µF**. Así 5 V del módulo llegan como 3,23 V al pin (factor 0,6454, el `DIVISOR_ANALOG` del firmware).

Antes de unir el nodo al ESP32: módulo encendido, medir el nodo con el multímetro. **Si marca más de 3,3 V, no conectar.**

**7a – MQ-135** (`T07a_mq135`)

1. AO → divisor → **GPIO 39**. DO sin conectar. El MQ-135 ya debe llevar horas encendido desde el tramo 1.
2. Esperar a que el valor se estabilice y enviar `b` (toma la línea base durante 1 minuto, en aire limpio).
3. Acercar a 5 cm un algodón con alcohol o vinagre: el cambio frente a la base debe subir claramente (decenas de %) y volver al retirarlo.
4. Anotar la línea base en mV y el índice 0–100. Es un indicador cualitativo: no se reporta como concentración de CO₂.

**7b – pH** (`T07b_ph`)

1. **Ajuste del cero, con multímetro:** módulo PH-4502C a 5 V, sonda desconectada, centro del BNC en corto con su carcasa (un clip). Girar el potenciómetro de **offset** hasta que Po marque **2,50 V**. El otro potenciómetro ajusta el umbral de la salida Do, que no se usa. Quitar el corto.
2. Po → divisor → **GPIO 36**. Conectar la sonda E201. Poner el DS18B20 en el mismo vaso que la sonda.
3. Sonda en **buffer pH 7**. Esperar a que el monitor diga `ESTABLE` (deriva menor de 3 mV en 30 s, normalmente 1–3 min) y enviar `7`.
4. Enjuagar con agua destilada, secar tocando con papel (sin frotar el bulbo) y pasar a **buffer pH 4**. Esperar `ESTABLE` y enviar `4`. Debe decir `ACEPTADA` con una pendiente entre 100 y 250 mV/pH. Como referencia, el firmware trae por defecto 2530 mV en pH 7 y 3070 mV en pH 4 (180 mV/pH).
5. Volver a pH 7: debe leer 7,00 ± 0,10. Observar 1 minuto: la lectura no debe saltar más de ± 0,05.
6. Guardar la sonda con su capuchón y la solución de almacenamiento (KCl). El bulbo nunca se deja secar.

Esta prueba solo verifica el hardware. La calibración que usará el lote se repite en el tramo 10 con el modo de calibración del firmware, que la guarda en la memoria del ESP32.

**Si falla:** pendiente menor de 100 mV/pH = sonda sucia, seca o buffers vencidos. Lectura que salta mucho = capacitor de 0,1 µF ausente o GND del módulo flojo. Lectura pegada en 0 o en \~3,2 V en el pin = divisor mal armado o sonda desconectada.

## Tramo 8 – Potencia: relés, caja de paso, bomba y calentador con agua

Objetivo: encender y apagar la bomba de aire y el calentador desde el ESP32 sin que los 110 V salgan de la caja de paso, y obtener la curva de calentamiento para AyC. Criterio: relés con la polaridad correcta y sin clic al reiniciar; caja aprobada con un bombillo; control ON-OFF dentro de ± 0,5 °C; curva de escalón guardada en CSV.

### 8a – Relés sin 110 V (`T08a_reles`)

1. Cada módulo de relé: VCC al riel de 5 V, GND, IN a **GPIO 21** (bomba de aire) o **GPIO 22** (calentador). Los bornes COM/NO/NC, vacíos.
2. Enviar `a` y `c`: cada relé debe sonar (clic) y encender su LED cuando el monitor dice ON. Si se activa al revés, enviar `p` para invertir. La polaridad que funciona va a `RELE_ACTIVO_ALTO` en `config.h` (1 = activo en alto, 0 = activo en bajo).
3. Multímetro en continuidad entre COM y NO: debe sonar solo en ON.
4. Presionar EN varias veces: los relés no deben quedar encendidos al arrancar.

**Si el relé no se apaga nunca:** en algunos módulos de 5 V con optoacoplador, los 3,3 V del ESP32 no alcanzan para apagarlo. Solución: un 2N2222 por relé (GPIO → 1 kΩ → base; emisor a GND; colector al IN del módulo) y `RELE_ACTIVO_ALTO` en 1.

### 8b – Caja de paso de 110 V (sin el ESP32 cerca)

Esta parte la arma o la revisa alguien con experiencia en instalaciones eléctricas (docente o técnico del laboratorio) **antes de energizarla por primera vez**. Todo se cablea desenchufado.

1. Los dos módulos de relé van atornillados o pegados dentro de la caja de 10 × 10. Sus tres cables de baja tensión (VCC, GND, IN) salen por un pasacables propio.
2. Cable de entrada con clavija → **portafusible con fusible de 1 A** (recomendado; bomba + calentador consumen \~0,3 A) → bornera de tornillo, borne "L" (fase).
3. Del borne L a los COM de los dos relés. Del NO de cada relé a la fase de su salida.
4. Neutro de entrada → borne "N" → neutro de las dos salidas. Nunca pasar el neutro por el relé.
5. Recomendado: que cada salida termine en una **toma hembra** (clavija aérea), para no cortar los cables de la bomba ni del calentador. Cada cable sale por su pasacables, con nudo o abrazadera interna para que un tirón no suelte el borne.
6. Identificar la fase con un destornillador buscapolo. Revisar con el multímetro, desenchufado, que no haya continuidad entre L, N y la caja.
7. Cerrar la caja. Enchufarla a una multitoma con interruptor, que queda como apagado general.
8. **Primera prueba con un bombillo** en cada salida: con `T08a_reles`, enviar `a` y `c`. El bombillo debe encender y apagar con su relé.

### 8c – Bomba de aire con agua

1. Garrafón con \~8 L de agua. Piedra difusora al fondo, lejos de la sonda de pH y del DS18B20.
2. La bomba va **por encima** del borde del garrafón, o con una válvula antirretorno en la manguera: si queda más baja que el agua, al apagarse el agua se devuelve por la manguera hacia la bomba.
3. Con `T08a_reles`, `a`: debe burbujear. Confirmar que las burbujas no mojan el HC-SR04 de la tapa.

### 8d – Calentador con agua y prueba escalón (`T08b_temperatura_agua`)

1. Calentador sumergido por encima de su marca mínima, a 1–2 cm de las paredes y lejos de las sondas. Esperar **30 min** sumergido antes de encenderlo la primera vez (el vidrio debe igualar la temperatura del agua). Perilla en **32 °C**.
2. En el sketch, poner `RELE_ACTIVO_ALTO` (de 8a) y `DIST_SENSOR_FONDO_CM` (del tramo 5).
3. **Prueba de enclavamientos, con el calentador desenchufado de su toma:** enviar `on`. Sacar el DS18B20 del agua o tapar el HC-SR04 con la mano para que no haya eco: en el CSV, la columna `bloqueo` debe dejar de ser `-`, el calentador debe pasar a 0 y el LED rojo debe encenderse con pitidos.
4. Enchufar el calentador. **Control ON-OFF**: enviar `on` (setpoint 30 °C). Con el agua por debajo de 29,5 °C, el calentador debe encender; por encima de 30,5 °C, apagarse.
5. **Escalón (para AyC):** con el agua a temperatura ambiente, enviar `esc` y dejarlo toda la noche. El calentador queda encendido sin parar y el CSV registra T cada 10 s. De esa curva salen la ganancia, la constante de tiempo y el tiempo muerto del modelo de primer orden.
6. Para que el registro no dependa del Arduino IDE ni de que el Mac se duerma: cerrar el monitor serie, ver el puerto con `ls /dev/cu.*` y correr en la Terminal `caffeinate -i screen -L /dev/cu.usbserial-XXXX 115200` (el registro queda en `screenlog.0`; salir con Ctrl-A y luego K).

**Qué esperar:** el calentador de 25 W necesita unas 3,7 h para subir 10 °C los 8 L de agua, sin contar pérdidas. Con un garrafón plástico sin aislar, las pérdidas pueden ser de unos 2,5 W por cada °C sobre el ambiente (estimación). Eso dejaría el agua estable a solo \~10 °C por encima del ambiente: con 17–18 °C en Manizales, no llegaría a 30 °C. Si el escalón se estabiliza por debajo de 30 °C, forrar el garrafón con icopor o aislante reflectivo (dejando una ventana para ver el nivel) y repetir. Además, el termostato propio del calentador corta a 32 °C como máximo: la consigna de 37 °C del plan del 17 nov no se puede alcanzar con este calentador.

## Tramo 9 – Servo y válvula de pinza, con agua

Objetivo: dejar lista la mecánica de la dosificación usando agua, para que en el tramo 11 solo cambie el líquido. Criterio: cero goteo en 30 min con la pinza cerrada; 5 dosis seguidas con una variación menor del 10 %; riel de 5 V por encima de 4,85 V mientras el servo se mueve, sin reinicios del ESP32.

**Conexiones:** SG90 café/negro a GND, rojo al **riel de 5 V** (nunca al pin 3V3 del ESP32), naranja/amarillo a **GPIO 14**. Capacitor de 470 µF entre 5 V y GND junto al conector del servo.

**Mecánica**

1. Depósito de melaza (por ahora con agua) por encima del garrafón. Mejor ancho y bajo que alto y angosto: la altura del líquido cambia menos al vaciarse y la dosis varía menos.
2. Manguera de silicona del depósito hasta la tapa del garrafón, con la salida por encima del nivel máximo (26 cm), para que nunca toque el caldo.
3. La pinza: el brazo del servo presiona la manguera contra un tope fijo (un bloque de madera o plástico pegado a la base). El servo y el tope van en una base rígida; si la base se dobla, la pinza no cierra.

**Código:** `T09_servo_valvula`. Comandos: `g <grados>`, `c`, `o`, `sc <grados>`, `so <grados>`, `d <ms>`, `r <n>`.

**Prueba**

1. Con la manguera vacía, mover con `g` de 10 en 10 grados y encontrar el ángulo donde la pinza **apenas** aplasta la manguera. Ese, más unos 5–8°, es el CERRADO (`sc`). Más allá, el servo queda forzado: zumba, se calienta y consume hasta 0,7 A.
2. Buscar el ABIERTO (`so`): el ángulo donde la manguera queda totalmente libre.
3. Llenar el depósito con agua. Pinza cerrada durante **30 min**: cero gotas. Tocar el servo al final: tibio está bien, caliente no.
4. Mientras el servo abre y cierra, medir el riel de 5 V con el multímetro. Debe quedar por encima de 4,85 V y el ESP32 no debe reiniciarse.
5. Con un vaso sobre la balanza (1 g de agua = 1 mL): `d 8000` (8 s, el valor actual del firmware). Anotar los gramos.
6. `r 5`: cinco dosis con 30 s entre ellas para pesar. Calcular el promedio y la variación (máximo − mínimo, dividido por el promedio).
7. Anotar en la hoja de registro: ángulos, mL por dosis con agua y variación. Los ángulos van a `SERVO_CERRADO_GRAD` y `SERVO_ABIERTO_GRAD` en `config.h`.

## Tramo 10 – Firmware completo, con agua

Objetivo: todo el prototipo corriendo con el firmware principal (`Simulacion/simbio`) en un lote de prueba con agua, antes de usar melaza o microorganismos. Criterio: calibración de pH aceptada; 12 h con al menos 99 % de los registros esperados en la SD y en Supabase; temperatura en ± 0,5 °C la mayor parte del tiempo una vez alcanzado el setpoint; todas las fallas provocadas detectadas.

**Preparar el código** (detalle en "Instrucciones del código")

1. En `config.h`: `SIMBIO_SIM 0` y los valores medidos en los tramos 5, 8a y 9.
2. En `secrets.h`: wifi del sitio y, si ya está creado el proyecto, la URL y la clave pública de Supabase.
3. Cargar. En el monitor, escribir `ayuda` y luego `estado`: deben aparecer todas las lecturas, `WiFi OK`, `SD OK` y `RFID OK`.

**Calibración de pH en el firmware**

1. DIP en ON: el monitor dice `MODO CALIBRACIÓN`.
2. Sonda y DS18B20 en buffer pH 7, esperar estabilidad, presionar **OK**. Enjuagar, buffer pH 4, presionar **OK**.
3. Debe decir `[CAL] Calibración guardada` con la pendiente. DIP en OFF. La calibración queda en la memoria del ESP32 aunque se apague.

**Lote de prueba con agua** (garrafón con 8 L, depósito con agua)

1. Acercar la tarjeta "Lote 1" al lector: amarillo rápido (ARMADO). Presionar **LOTE** antes de 15 s: verde.
2. Revisar con `estado` cada tanto: ciclos de aire de 15 min, calentador según el setpoint, nivel y volumen.
3. Forzar una dosis con el comando `dosis` (el intervalo real es de 24 h): el servo abre 8 s y aparece el evento `dosis_melaza`.
4. `csv` muestra el archivo del lote en la SD. En Supabase, la tabla `lecturas` y la vista `ultima_lectura` deben recibir filas.

**Fallas provocadas** (una a la vez; anotar qué alarma sale y cuánto tarda)

| Falla | Qué se hace | Qué debe pasar |
| --- | --- | --- |
| Sin red | Apagar el wifi 10 min | Alarma `sin_red`; los registros siguen en la SD y en la cola; al volver, los pendientes bajan a 0 sin duplicados |
| Corte de energía | Apagar la multitoma 5 min | Al volver, el lote se reanuda solo; si no hay red al arrancar, los registros salen como hora estimada |
| Nivel bajo | Sacar agua hasta menos de 19 cm | Alarma `nivel_bajo`, calentador apagado, rojo y buzzer; OK silencia |
| Sensor de T | Desconectar el DATA del DS18B20 | Alarma `sensor_temp`, calentador apagado |
| Reinicio | Comando `reiniciar` | El lote continúa con el mismo identificador |

**Interferencia entre la sonda y las cargas:** con la sonda de pH en el agua del garrafón, comparar la lectura con la bomba y el calentador apagados y encendidos. Si cambia más de 0,1 pH, anotarlo: el firmware tendría que leer el pH con las cargas apagadas un instante (cambio pendiente).

**Corrida nocturna:** dejar el lote 12 h. A la mañana, contar las filas del CSV (a un registro por minuto, 12 h = 720) y el porcentaje del tiempo con T dentro de 30 ± 0,5 °C. Son las mismas métricas del OE3. Cerrar el lote sosteniendo **LOTE** 2,5 s.

## Tramo 11 – Melaza (última prueba)

Objetivo: saber cuántos mL de melaza diluida entrega cada dosis y fijar el tiempo de apertura, con todo lo demás ya probado. Criterio: 5 dosis con variación menor del 10 %; la manguera fluye después de 24 h cerrada; el tiempo de apertura queda en `config.h`.

**Preparación**

1. Diluir la melaza con agua tibia en la proporción que defina el protocolo del lote (Óscar). Para la prueba se puede partir de 1 parte de melaza por 1–2 de agua: sin diluir, no baja por gravedad por una manguera delgada.
2. Pesar 100 mL de la dilución para saber su densidad (g/mL). Con eso se pasan los gramos de la balanza a mL.
3. Anotar la temperatura ambiente: la melaza es más espesa en frío y la dosis de la madrugada puede salir menor que la de la tarde.

**Calibración de la dosis** (`T09_servo_valvula`, el mismo sketch del tramo 9)

1. Depósito con la dilución, a la misma altura que tendrá en el lote. Vaso sobre la balanza bajo la salida.
2. `d 8000`: anotar gramos y pasarlos a mL. Caudal = mL / 8 s.
3. Tiempo de apertura = dosis que pide el protocolo (mL) ÷ caudal (mL/s). Probarlo con `d <ms>`.
4. `r 5` con ese tiempo: calcular la variación.
5. Pinza cerrada durante 30 min con la melaza en la manguera: cero gotas.

**Prueba de 24 h**

Dejar la manguera llena y la pinza cerrada 24 h; después hacer una dosis. Si sale menos o no sale, la melaza se espesó o cristalizó en la zona aplastada: aumentar la dilución o mover la manguera unos milímetros en la pinza. Tapar el depósito: la melaza diluida fermenta sola, así que durante el lote se prepara fresca cada 2–3 días.

**Prueba en el tanque, con el firmware**

1. Poner el tiempo de apertura medido en `DOSIS_ABIERTA_MS` (el segundo valor de `SIM_O_REAL`) y cargar el firmware.
2. Con el garrafón con agua y un lote de prueba activo, enviar `dosis`. Confirmar que la melaza cae en el centro sin salpicar el HC-SR04, la sonda de pH ni el DS18B20.
3. Cerrar el lote, vaciar y lavar el garrafón, la manguera de aire y las sondas antes del lote de MM.

Con este tramo aprobado, el prototipo está listo para el lote único de MM según el protocolo de pruebas (fase 3).

## Instrucciones del código

Hay dos niveles de código. Los **sketches de prueba** (carpeta `Simulacion/pruebas_por_tramo/`) ya están escritos: uno por tramo, independiente y con los pines del 12 nov. El **firmware principal** (`Simulacion/simbio/`) ya existe y solo necesita los valores medidos en `config.h`. Lo que falta escribir es el control PI, el modo escalón y el servidor Modbus para AyC.

### Sketches de prueba

| Tramo | Sketch | Qué entrega |
| --- | --- | --- |
| 2 | `T02_esp32_wifi_ntp` | Wifi, RSSI y hora NTP |
| 3 | `T03_interfaz_local` | LEDs, buzzer, pulsadores y DIP |
| 4 | `T04_temperaturas` | CSV de T del líquido, T y HR ambiente |
| 5 | `T05_nivel_hcsr04` | `DIST_SENSOR_FONDO_CM` y `AREA_TANQUE_CM2` |
| 6 | `T06a_sd`, `T06b_rfid`, `T06c_spi_compartido` | SD, UID de las tarjetas, bus compartido |
| 7 | `T07a_mq135`, `T07b_ph` | Línea base del MQ-135; pendiente del pH |
| 8 | `T08a_reles`, `T08b_temperatura_agua` | `RELE_ACTIVO_ALTO`; CSV del ON-OFF y del escalón |
| 9 y 11 | `T09_servo_valvula` | Ángulos de la pinza; mL por dosis con agua y con melaza |

Todos siguen el mismo patrón, útil si quieren escribir otra prueba: (1) constantes de pines arriba, copiadas de `config.h`; (2) en `setup()`, iniciar el módulo e imprimir un autodiagnóstico (encontrado / no encontrado); (3) en `loop()`, leer, imprimir una línea CSV y atender comandos de una letra por el monitor serie. Las salidas de potencia se ponen en su nivel de apagado **antes** de `pinMode(..., OUTPUT)` para que no cliquen al arrancar.

Los sketches pasaron una revisión de sintaxis, pero todavía no se han compilado en el Arduino IDE: el primer compilado real es el de cada tramo.

### Cambios en `config.h` antes del tramo 10

| Constante | Valor actual | De dónde sale el nuevo |
| --- | --- | --- |
| `SIMBIO_SIM` | 1 | Poner 0 (prototipo real) |
| `RELE_ACTIVO_ALTO` | 1 | Tramo 8a |
| `DIST_SENSOR_FONDO_CM` | 30.0 | Tramo 5, comando `c` |
| `AREA_TANQUE_CM2` | 348.0 | Tramo 5, promedio de los comandos `v` |
| `SERVO_CERRADO_GRAD` / `SERVO_ABIERTO_GRAD` | 0 / 90 | Tramo 9 |
| `DOSIS_ABIERTA_MS` | `SIM_O_REAL(3000, 8000)` | Tramo 11: cambiar solo el 8000 |

En `secrets.h` (rama `#else`): `WIFI_SSID`, `WIFI_PASS`, `SUPABASE_URL` y `SUPABASE_KEY` (la clave pública, nunca la `service_role`). Para volver a Wokwi basta con `SIMBIO_SIM 1`.

### Código que falta escribir

**Solo si una prueba lo pide**

- Tramo 6c falla: mover la SD al segundo bus SPI (`SPIClass hspi(HSPI)` con sus propios pines) en `registro.cpp`.
- Tramo 10 muestra interferencia mayor de 0,1 pH: en `sensores.cpp`, apagar bomba y calentador \~2 s antes de leer el pH y volver a encenderlos después, sin que eso cuente en el control.

**Para AyC (decisiones M4 y M9), en este orden**

1. **Modo escalón en el firmware** (`modo = 2`, sin lote): calentador al 100 % respetando los enclavamientos, registro cada 10 s en la SD. Sirve para repetir el escalón del tramo 8 con el sistema completo.
2. **Modelo:** de la curva del escalón sacar la ganancia K (°C de subida final por 100 % de calentador), la constante de tiempo τ (tiempo hasta el 63 % de la subida, contado después del tiempo muerto) y el tiempo muerto θ.
3. **PI por ventana de tiempo** en `control.cpp`, como alternativa a `controlTemperatura()`: cada ventana de 60 s se calcula u = Kp · (e + ∫e·dt / Ti), con e = setpoint − T, u limitado a 0–1. El calentador se enciende u · 60 s al inicio de la ventana; si eso da menos de 5 s, no enciende. **Anti-windup:** la integral no acumula mientras u esté saturado en 0 o en 1. Los enclavamientos (nivel, T máxima, sensores) siguen mandando sobre el PI. Valores iniciales con la regla SIMC: τc = θ, Kp = τ / (K · (τc + θ)), Ti = el menor entre τ y 4 · (τc + θ).
4. **Servidor Modbus TCP** con la librería *modbus-esp8266* (Alexander Emelianov), solo en la red local: `ModbusIP mb; mb.server();`, `mb.task()` en cada `loop()`. Registros de entrada 30001–30010 según M9 (T×100, pH×100, nivel×10, gas×10, T y HR ambiente ×10, alarmas en dos registros, minutos de aire, dosis), actualizados cada segundo. Registros de retención 40001–40004: consigna ×100 (aceptar solo 2500–3500), modo (0 ON-OFF, 1 PI, 2 escalón sin lote), Kp×100 y Ti en s. Los estados de bomba, calentador, válvula y alarma conviene publicarlos como entradas discretas (10001–10004), que el SCADA no puede escribir; si se dejan como coils, el firmware los reescribe en cada ciclo.
5. **Prueba comparativa** ON-OFF frente a PI con agua: misma consigna, 3 h cada uno, mismo nivel inicial. Comparar el % del tiempo dentro de ± 0,5 °C y el número de conmutaciones del relé.

Cada cambio al firmware se documenta en `doc/decisiones/04-software-firmware-y-datos.md` y en la bitácora, según la regla del proyecto.

## Riesgos detectados y hoja de registro

**Riesgos que conviene vigilar desde ya**

- **Calentador corto de potencia:** con el garrafón sin aislar, 25 W podrían no llevar 8 L a 30 °C en Manizales (estimación del tramo 8). El escalón lo confirma; la salida es aislar el garrafón.
- **Consigna de 37 °C para el escalón (plan del 17 nov):** no alcanzable, porque el termostato del calentador corta a 32 °C. El escalón se hace de 0 a 100 % de calentador desde la temperatura ambiente.
- **L7805 a 7,5 V:** puede quedarse sin margen con el servo y el wifi a la vez. Se revisa en los tramos 1 y 9; la salida es pasar a 9 V.
- **Relés de 5 V con 3,3 V:** algunos no se apagan. Se revisa en 8a; la salida es un 2N2222 por relé.
- **SD y RC522 en el mismo SPI:** se revisa en 6c; la salida es el segundo bus.
- **Calendario:** con esta secuencia el lote de MM empieza el 21 nov y termina el 26, un día antes de la entrega.
- **Sonda E201:** no está hecha para inmersión continua de días. Ya está reportada como limitación; revisar su lectura en buffer pH 7 al terminar el lote.

**Hoja de registro** (llenarla al aprobar cada tramo; los valores y fotos van al artículo y al informe de AyC)

| Tramo | Estado | Fecha | Valores medidos | Notas y foto |
| --- | --- | --- | --- | --- |
| 0 – Preparación | Pendiente |  | Compila T02: sí/no |  |
| 1 – Alimentación | Pendiente |  | V adaptador; V riel sin carga; V riel con MQ-135 |  |
| 2 – ESP32 | Pendiente |  | RSSI en el sitio; hora NTP |  |
| 3 – Interfaz | Pendiente |  | LEDs, buzzer, LOTE, OK, DIP |  |
| 4 – Temperaturas | Pendiente |  | T en hielo; T tibia vs referencia |  |
| 5 – Nivel | Pendiente |  | DIST\_SENSOR\_FONDO\_CM; área; error vs regla |  |
| 6 – SD y RFID | Pendiente |  | Versión RC522; UIDs de los lotes 1–3; fallos en 10 min |  |
| 7 – MQ-135 y pH | Pendiente |  | Línea base MQ-135; mV pH 7 y pH 4; pendiente |  |
| 8 – Potencia | Pendiente |  | Polaridad relés; prueba bombillo; T final del escalón; K, τ, θ |  |
| 9 – Servo con agua | Pendiente |  | Ángulos; mL por dosis; variación; V riel con servo |  |
| 10 – Firmware con agua | Pendiente |  | Pendiente pH; % registros en 12 h; % tiempo en ± 0,5 °C; fallas detectadas |  |
| 11 – Melaza | Pendiente |  | Dilución; densidad; caudal; DOSIS\_ABIERTA\_MS; prueba de 24 h |  |
