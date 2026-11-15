# 01 · Alcance del proyecto y entregables

**Estado:** vigente · **Última actualización:** 2026-11-15

## Contexto
SIMBIO es un prototipo mínimo viable (maqueta HW/SW) para los cursos *Automatización y Control de Procesos* (AyC) y *Fundamentos de IoT*, Ingeniería de Sistemas y Computación, Universidad de Caldas. Trabajo en parejas.

## Decisiones
| # | Decisión | Fecha | Motivo |
|---|---|---|---|
| A1 | El proyecto cumple los requisitos **de los dos cursos**, tomando lo más exigente de cada uno | 2026-11-05 | Un solo prototipo para AyC e IoT |
| A2 | Mínimos: **≥ 5 entradas**, control, **≥ 4 salidas distintas** (luz y sonido cuentan como una), **web + app**, base de datos | 2026-11-05 | Guías de ambos cursos |
| A3 | Título vigente (15 palabras, límite VIP): *Sistema de trazabilidad por lote para la producción artesanal de bioinsumos en biofábricas de Caldas*. Alternativo: *Sistema IoT de control y trazabilidad por lote para biofábricas artesanales de bioinsumos en Caldas* | 2026-11-05 | Cumple el formato VIP |
| A4 | Objetivos específicos: **3** en formulación, talleres y artículo; **4** (uno por nivel de Bloom) en el informe de AyC | 2026-11-05 | Lo exige cada esquema |

## Entregables (en orden)
1. Taller 1 – Problema y título
2. Taller 2 – Objetivos y metodología
3. Prototipo (Sprint 1 y final)
4. Pruebas (datos y registros)
5. Taller 3 – Artículo científico (≥ 30 referencias)
6. Informe AyC (esquema UCaldas, APA 7)
7. Formato de formulación de proyecto (VIP UCaldas, presupuesto ≤ 15 smmlv)
8. Modelo Canvas (Excel)
9. Cierre: video demo, sustentación (EAiFi), artículo final, repositorio

## Estado de cada entregable (actualizado 15 nov 2026)
| Entregable | Archivo | Estado |
|---|---|---|
| Talleres 1 y 2 | Doc "SIMBIO – Talleres 1 y 2" (Claude) | Actualizado 12 nov. Faltan nombres del equipo |
| Formulación VIP | `SIMBIO_Formulacion_Proyecto.docx` / `Karen_Naghelly_Moreno.docx` | **15 nov:** encabezado con integrantes, grupos y tipo de proyecto (ver [08](08-equipo-investigador.md)); sección c lista en el .docx; **d.4 redactada** en `Doc/textos/` y pendiente de pegar. Falta: corregir OE1 (OD → aireación), aplicar las correcciones de 06 en d.1 y d.2, título, resumen ejecutivo (b), d.5–d.8, presupuesto, pasar a 10 L, quitar ADS1115/RTC/12 V/LM2596, calentador 25 W, Anexo 1 |
| Presupuesto | `Doc/SIMBIO – Presupuesto del proyecto.xlsx` | Ver [05-presupuesto](05-presupuesto.md) |
| Artículo | `SIMBIO_Articulo_Borrador.docx` | Borrador con 34 refs. Falta actualizar hardware y resultados del piloto |
| Informe AyC | `SIMBIO_Informe_AyC.docx` | Completo; falta actualizar a 10 L e instrumentación nueva |
| Canvas | `SIMBIO_Modelo_Canvas.xlsx` | Falta alinear bidón de 20 L y presupuesto |
| Maqueta 3D | Página "SIMBIO – Maqueta 3D del prototipo" (Claude) | Actualizada 12 nov |
| Simulación + firmware | `Simulacion/` | Completa; aún no compilada en Arduino IDE |
