# Bitácora de cambios de SIMBIO

Entradas de la más reciente a la más antigua. Formato y obligatoriedad: ver [REGLA-documentacion-de-cambios.md](REGLA-documentacion-de-cambios.md).

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
