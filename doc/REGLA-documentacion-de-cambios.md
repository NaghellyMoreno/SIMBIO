# REGLA OBLIGATORIA · Documentación de cambios

**Vigente desde:** 2026-11-14 · Aplica a **todas** las personas del equipo y a Claude.

## La regla
**Ningún cambio en el proyecto SIMBIO se da por terminado hasta que esté documentado en `Doc/`.**

Se considera *cambio* cualquier modificación de:
- hardware (componentes, pines, conexiones, alimentación, tanque),
- firmware, simulación, base de datos, web o app (`Simulacion/` y futuras carpetas de código),
- parámetros de control (`config.h`),
- presupuesto o lista de compras,
- proceso piloto, objetivos, metodología, cronograma o alcance,
- entregables (talleres, artículo, informe, formulación, Canvas) cuando cambia su contenido técnico,
- referencias o datos citados.

## Qué hacer con cada cambio
1. **Si el tema ya tiene documento** en `Doc/decisiones/` → **actualizarlo**:
   - modificar la fila de la decisión afectada (o agregar una nueva con el siguiente número: H14, S12…),
   - si una decisión se reemplaza, **no borrarla**: marcarla como `~~reemplazada~~ por Xn (fecha)` y agregar la nueva,
   - cambiar la fecha de *Última actualización*.
2. **Si el tema no tiene documento** → **crear uno nuevo** `Doc/decisiones/NN-tema.md` con la plantilla de abajo y agregarlo al índice (`Doc/README.md`).
3. **Siempre** → agregar una entrada al **inicio** de `Doc/BITACORA-cambios.md`.
4. Si el cambio resuelve una pregunta → actualizar `07-pendientes-y-preguntas.md`.
5. Si el cambio afecta un entregable de Office → anotarlo en la columna de pendientes de `01-alcance-y-entregables.md`.

Un cambio de código y su documentación van **juntos** (mismo día, mismo commit si se usa git).

## Plantilla de entrada de bitácora
```markdown
## AAAA-MM-DD · Título corto del cambio
- **Quién:** nombre (o Claude)
- **Qué cambió:** …
- **Por qué:** …
- **Archivos afectados:** `ruta/archivo`, …
- **Documentos actualizados:** `decisiones/NN-tema.md` (decisión Xn)
- **Pendiente derivado:** … (o "ninguno")
```

## Plantilla de documento nuevo
```markdown
# NN · Tema

**Estado:** vigente · **Última actualización:** AAAA-MM-DD

## Contexto
…

## Decisiones
| # | Decisión | Fecha | Motivo / alternativa descartada |
|---|---|---|---|
| X1 | … | AAAA-MM-DD | … |

## Pendiente
- …
```

## Convenciones
- Fechas en formato **AAAA-MM-DD**. Montos en **COP**.
- Prefijos de decisión: A alcance, P proceso, H hardware, S software, B presupuesto, R referencias. Un tema nuevo elige una letra libre.
- Los documentos son Markdown (`.md`) para que se puedan versionar y comparar.

## Convenciones para commits
Todo commit del proyecto debe usar la siguiente estructura:

```text
tipo: descripción breve
```

| Tipo | Cuándo usarlo |
|---|---|
| `feat` | Cuando se agregue una operación matemática real |
| `fix` | Corrección de un bug |
| `docs` | Cambios solo en documentación, como README o comentarios |
| `style` | Cambios de formato o estilos que no afectan la lógica, como ajustes de Tailwind |
| `refactor` | Reestructuración de código sin cambiar su comportamiento |
| `test` | Agregar o corregir pruebas |
| `chore` | Configuración, dependencias o tareas de mantenimiento |
