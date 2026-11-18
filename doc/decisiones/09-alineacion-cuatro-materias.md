# 09 · Alineación con las cuatro materias

**Estado:** vigente · **Última actualización:** 2026-11-18

## Contexto
SIMBIO debe servir a cuatro materias de Ingeniería de Sistemas y Computación (Universidad de Caldas): Automatización y Control de Procesos (AyC, G8F0223), Fundamentos de IoT, Emprendimiento y Creación de Empresas de Base Tecnológica (25G8F) e Investigación para Ingeniería (IPI, 34G8F). Análisis completo, matriz de brechas, plan de dos semanas y 27 referencias nuevas en el doc "SIMBIO – Alineación con las cuatro materias": https://claude.ai/artifact/VDSKLMHXbpAA7DP74ZwbrR

## Decisiones
| # | Decisión | Fecha | Motivo / alternativa descartada |
|---|---|---|---|
| M1 | IPI se entrega con el **formato de formulación VIP**; debe responder a los RA R1–R5 de 34G8F (hipótesis, impactos en contextos globales, económicos, ambientales, culturales y sociales) | 2026-11-17 | Respuesta de Karen |
| M2 | Un solo nombre: **SIMBIO**. El Canvas EQ_3 ("SIMBA") se renombra | 2026-11-17 | Respuesta de Karen. Descartado: SIMBA como nombre de empresa |
| M3 | Quedan **≤ 2 semanas**: solo lo marcado A (obligatorio) en la matriz; B si sobra tiempo; C a trabajo futuro | 2026-11-17 | Respuesta de Karen |
| M4 | *(SCADA reemplazado por M10)* AyC: **firmware + SCADA gratis**: prueba escalón (subida y bajada) para la función de transferencia de primer orden con tiempo muerto, **PI por ventana de tiempo** (60 s, encendido mínimo 5 s, anti-windup) comparado con el ON-OFF, **servidor Modbus TCP** en el ESP32 (librería modbus-esp8266, solo red local) y **FUXA** como SCADA en el portátil | 2026-11-17 | Respuesta de Karen. Descartados: solo documentarlo; integrar el PLC LOGO!. Alternativa si el profesor la pide: LabVIEW con la librería Modbus de NI |
| M5 | **Materiales ya comprados** para el prototipo TRL 4: no hay compras; en el Anexo 1 y en el Canvas el prototipo entra como recurso propio (especie) | 2026-11-17 | Indicación de Karen |
| M6 | Se mantiene **un solo lote de 10 L** para las materias (P del 15 nov). *Fecha reemplazada por M11* — ruta crítica: el lote debía arrancar el 2026-11-21 (supuesto) | 2026-11-17 | Plazo de 2 semanas |
| M7 | El "vacío" se precisa: lo nuevo de SIMBIO es **controlar y trazar cada lote, con respaldo sin conexión, en una biofábrica artesanal**. Motivo: Ruiz Rubiano y Góngora Herrera (2023, U. Distrital) ya monitorean pH, T y nivel de un biofertilizante (sin control ni trazabilidad por lote) y Champa et al. (2026) controlan una fermentación doméstica de fertilizante líquido | 2026-11-17 | Búsqueda de literatura gris y blanca |
| M8 | Hipótesis propuesta (metas 90 % y 99 % por confirmar): el sistema registra pH y T con error ≤ 0,2 pH y ≤ 0,5 °C, mantiene T en ±0,5 °C ≥ 90 % del tiempo y conserva ≥ 99 % de los registros ante cortes de red, superando al registro tradicional en completitud y detección de desviaciones | 2026-11-17 | RA R2 de IPI |
| M9 | Mapa de registros Modbus: IR 30001 T×100, 30002 pH×100, 30003 nivel cm×10, 30004 gas×10, 30005/30006 T y HR amb ×10, 30007/30008 alarmas, 30009 min de aire, 30010 dosis; coils 1–4 bomba/calentador/válvula/alarma (solo lectura); HR 40001 consigna °C×100 (25–35), 40002 modo (0 ON-OFF, 1 PI, 2 escalón sin lote), 40003 Kp×100, 40004 Ti s | 2026-11-17 | Diseño; aún no implementado |

| M10 | **SCADA en LabVIEW** (instalado en el Mac, /Applications/National Instruments) como maestro Modbus TCP: librería Modbus Master de Plasmionique o NI Modbus Library (gratuitas); si el gestor de paquetes no corre en macOS, copiar a `user.lib` o armar la trama FC04 con los VI de TCP. Panel: mímico, gráficas T/pH/nivel, alarmas, consigna y selector ON-OFF/PI; histórico CSV/TDMS y reporte del lote. *Reemplaza FUXA de M4* | 2026-11-17 | Preferencia de Karen; ya lo usaron en AyC |
| M11 | **Entrega final 2026-11-27.** Plan: montaje + Supabase + calibración 17 nov; escalón de subida la noche del 17 (firmware actual, lote de prueba con agua, consigna 37 °C) y bajada el 18; firmware PI + Modbus 17–18; ON-OFF vs PI y cortes de wifi el 19 en la mañana; **lote de MM 19–24 nov**; web, app, LabVIEW, Canvas y ficha 18–23; VIP y pptx 18–24; análisis 24–25; artículo, informe y video 24–26 | 2026-11-17 | Respuesta de Karen |
| M12 | OE1 por análisis documental + juicio de experto (ver 02, P11) y objetivos VIP a TRL 5 (02, P12) | 2026-11-17 | Respuestas de Karen |
| M13 | Cita de Xavier: ambas fuentes existen. El dato de los productores es de **Xavier y Rodrigues (2024)**, Vigil. Sanit. Debate 12, e02272 (ya en la bibliografía). En el pptx de IPI se reemplazó la referencia de Xavier, Martins do Vale y de Almeida (2026) y su enlace por la de 2024 (respaldo `IPI/MORENO_KAREN_respaldo_2026-11-17.pptx`). La de 2026 (Braz. J. Microbiol. 57, 141) se conserva como fuente del OE1 | 2026-11-17 | Pedido de Karen |
| M14 | No hay ficha oficial de la Unidad de Emprendimiento: se redacta una con campos usuales y se ajusta al formato oficial cuando lo dé el profesor. Autores confirmados: AyC e IoT Karen y Camila; Canvas Camila, Karen y Yonier; VIP Karen, Diana y Óscar | 2026-11-17 | Respuestas de Karen |
| M15 | **Inscripciones EAiFi 13.0 enviadas.** El resumen de IPI fija las fases y los resultados esperados del proyecto; todo entregable se alinea con él (ver 02, P14). Materias = fases 1 y 2 (TRL 4); formulación VIP = fases 1–3 con validación TRL 5. El doc de inscripciones refleja el texto enviado: en los resultados de IPI ya no figura "además de la formación de una estudiante de maestría" (93 palabras; total 451) | 2026-11-18 | Pedido de Karen |

## Pendiente
- Implementar M4 en el firmware (modo escalón, PI, Modbus) y documentarlo en 04.
- Preguntas abiertas de M1–M8: ver 07.
