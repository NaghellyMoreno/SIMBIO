-- =====================================================================
--  SIMBIO – Esquema de base de datos en Supabase (PostgreSQL)
--  Cómo usarlo: Supabase -> SQL Editor -> New query -> pegar todo -> Run
--  Se puede ejecutar varias veces sin dañar datos (usa "if not exists").
-- =====================================================================

-- ---------------------------------------------------------------------
-- 1. Lotes: un registro por lote (trazabilidad)
-- ---------------------------------------------------------------------
create table if not exists public.lotes (
  id               text primary key,                 -- ej. MM-20261113-144205
  uid_rfid         text not null,                    -- tarjeta/llavero del lote
  proceso          text not null default 'Activación líquida de MM',
  dispositivo      text,
  inicio           timestamptz,
  inicio_estimado  boolean default false,            -- true si no había hora NTP
  fin              timestamptz,
  estado           text not null default 'activo' check (estado in ('activo', 'cerrado')),
  duracion_s       integer,
  aire_min_total   real,
  dosis_total      integer,
  observaciones    text,
  creado_en        timestamptz not null default now()
);

-- ---------------------------------------------------------------------
-- 2. Lecturas: una fila cada REG_INTERVALO_S (5 s en Wokwi, 60 s en el real)
-- ---------------------------------------------------------------------
create table if not exists public.lecturas (
  id               bigint generated always as identity primary key,
  lote_id          text not null references public.lotes (id) on delete cascade,
  t_lote_s         integer not null,                 -- segundos desde el inicio del lote
  ts               timestamptz,
  ts_estimada      boolean default false,
  ph               real,
  ph_mv            real,
  temp_liquido     real,
  temp_ambiente    real,
  humedad_ambiente real,
  nivel_cm         real,
  volumen_l        real,
  gas_indice       real,                             -- MQ-135, cualitativo 0–100
  bomba_aire       boolean,
  calentador       boolean,
  valvula_melaza   boolean,
  aire_min_acum    real,
  dosis_acum       integer,
  setpoint         real,
  alarmas          integer,                          -- máscara de bits (ver README)
  semaforo         text,
  recibido_en      timestamptz not null default now(),
  unique (lote_id, t_lote_s)                         -- evita duplicados al reenviar
);
create index if not exists lecturas_lote_t_idx on public.lecturas (lote_id, t_lote_s);

-- ---------------------------------------------------------------------
-- 3. Eventos: inicio/cierre, dosis, alarmas, calibraciones, reinicios
-- ---------------------------------------------------------------------
create table if not exists public.eventos (
  id               bigint generated always as identity primary key,
  uid_evento       text not null unique,             -- SIMBIO-01-<arranque>-<n>
  lote_id          text references public.lotes (id) on delete set null,
  t_lote_s         integer,
  ts               timestamptz,
  ts_estimada      boolean default false,
  tipo             text not null,
  detalle          text,
  dispositivo      text,
  recibido_en      timestamptz not null default now()
);
create index if not exists eventos_lote_idx on public.eventos (lote_id, id);

-- ---------------------------------------------------------------------
-- 4. Vista para la web/app: última lectura de cada lote
-- ---------------------------------------------------------------------
create or replace view public.ultima_lectura
with (security_invoker = true) as
select distinct on (l.lote_id) l.*
from public.lecturas l
order by l.lote_id, l.t_lote_s desc;

-- ---------------------------------------------------------------------
-- 5. Permisos (Row Level Security)
--    PROTOTIPO ACADÉMICO: el ESP32 usa la clave pública (anon/publishable)
--    y puede insertar. Cualquiera con esa clave puede leer y escribir.
--    Para producción: autenticar el dispositivo o usar una Edge Function.
-- ---------------------------------------------------------------------
alter table public.lotes    enable row level security;
alter table public.lecturas enable row level security;
alter table public.eventos  enable row level security;

grant select, insert, update on public.lotes    to anon, authenticated;
grant select, insert         on public.lecturas to anon, authenticated;
grant select, insert         on public.eventos  to anon, authenticated;
grant select                 on public.ultima_lectura to anon, authenticated;

drop policy if exists "lotes_leer"        on public.lotes;
drop policy if exists "lotes_insertar"    on public.lotes;
drop policy if exists "lotes_actualizar"  on public.lotes;
drop policy if exists "lecturas_leer"     on public.lecturas;
drop policy if exists "lecturas_insertar" on public.lecturas;
drop policy if exists "eventos_leer"      on public.eventos;
drop policy if exists "eventos_insertar"  on public.eventos;

create policy "lotes_leer"        on public.lotes    for select to anon, authenticated using (true);
create policy "lotes_insertar"    on public.lotes    for insert to anon, authenticated with check (true);
create policy "lotes_actualizar"  on public.lotes    for update to anon, authenticated using (true) with check (true);
create policy "lecturas_leer"     on public.lecturas for select to anon, authenticated using (true);
create policy "lecturas_insertar" on public.lecturas for insert to anon, authenticated with check (true);
create policy "eventos_leer"      on public.eventos  for select to anon, authenticated using (true);
create policy "eventos_insertar"  on public.eventos  for insert to anon, authenticated with check (true);

-- ---------------------------------------------------------------------
-- 6. Tiempo real (para que la web/app se actualice sola)
-- ---------------------------------------------------------------------
do $$
begin
  begin
    alter publication supabase_realtime add table public.lecturas;
  exception when duplicate_object then null;
  end;
  begin
    alter publication supabase_realtime add table public.eventos;
  exception when duplicate_object then null;
  end;
end $$;

-- ---------------------------------------------------------------------
-- Consultas útiles
-- ---------------------------------------------------------------------
-- select * from lotes order by creado_en desc;
-- select * from ultima_lectura;
-- select t_lote_s, ph, temp_liquido, nivel_cm, bomba_aire, calentador
--   from lecturas where lote_id = 'MM-...' order by t_lote_s;
-- select ts, tipo, detalle from eventos where lote_id = 'MM-...' order by id;
