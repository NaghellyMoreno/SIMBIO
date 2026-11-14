#pragma once
// =====================================================================
//  Credenciales (NO subir este archivo a un repositorio público)
// =====================================================================

#if SIMBIO_SIM
// En Wokwi la red virtual se llama Wokwi-GUEST y no tiene clave
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""
#else
#define WIFI_SSID "NOMBRE_DE_TU_RED"
#define WIFI_PASS "CLAVE_DE_TU_RED"
#endif

// Supabase -> Project Settings -> API (o "Connect")
//   SUPABASE_URL: https://xxxxxxxxxxxx.supabase.co   (sin "/" al final)
//   SUPABASE_KEY: la clave pública (anon / publishable), NUNCA la service_role
// Si se dejan vacías, el prototipo funciona igual y guarda los datos en la SD.
#define SUPABASE_URL ""
#define SUPABASE_KEY ""
