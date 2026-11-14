# Instrucciones del proyecto SIMBIO

## Regla obligatoria de documentación
Todo cambio que hagas en este proyecto (código, hardware, parámetros, presupuesto, proceso, entregables o referencias) **debe quedar documentado en `Doc/` en la misma tarea**, siguiendo `Doc/REGLA-documentacion-de-cambios.md`:
1. Actualiza el documento del tema en `Doc/decisiones/` (o crea uno nuevo con la plantilla y agrégalo a `Doc/README.md`). No borres decisiones reemplazadas: márcalas como reemplazadas.
2. Agrega siempre una entrada al inicio de `Doc/BITACORA-cambios.md`.
3. Si se resuelve una pregunta, actualiza `Doc/decisiones/07-pendientes-y-preguntas.md`.

No des una tarea por terminada sin esto. Antes de proponer cambios, lee `Doc/README.md` y el documento del tema para respetar las decisiones vigentes.

Idioma de la documentación: español. Fechas AAAA-MM-DD. Montos en COP.

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
