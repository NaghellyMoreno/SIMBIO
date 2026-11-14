#pragma once
// =====================================================================
//  SIMBIO – Configuración: pines y parámetros del proceso
//  SIMBIO_SIM = 1 -> simulación en Wokwi (tiempos acelerados)
//  SIMBIO_SIM = 0 -> prototipo real
// =====================================================================
#include <Arduino.h>

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//  ELIJA EL MODO AQUÍ:
//    1 = simulación en Wokwi (tiempos acelerados, DHT22, red Wokwi-GUEST)
//    0 = ESP32 real del montaje (tiempos reales, DHT11, su WiFi)
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef SIMBIO_SIM
#define SIMBIO_SIM 1
#endif

#if SIMBIO_SIM
#define SIM_O_REAL(sim, real) (sim)
#else
#define SIM_O_REAL(sim, real) (real)
#endif

#define DISPOSITIVO_ID "SIMBIO-01"
#define VERSION_FW     "1.0.0"

// ---------------------------------------------------------------------
//  Pines del ESP32 (plan del 12 nov 2026, sin I2C)
// ---------------------------------------------------------------------
#define PIN_PH              36   // Po del PH-4502C (con divisor + 0,1 uF)
#define PIN_MQ135           39   // AO del MQ-135 (con divisor + 0,1 uF)
#define PIN_RELE_AIRE       21   // Relé bomba de aire
#define PIN_RELE_CALENTADOR 22   // Relé calentador 25 W
#define PIN_BTN_LOTE        32   // Pulsador LOTE (pull-up interno)
#define PIN_BTN_OK          33   // Pulsador OK / silenciar (pull-up interno)
#define PIN_DIP_CAL         35   // DIP modo calibración (pull-up externo 10 k)
#define PIN_DS18B20          4   // 1-Wire (pull-up 4,7 k)
#define PIN_DHT             13   // DHT11 (real) / DHT22 (Wokwi)
#define PIN_SERVO           14   // Servo SG90, válvula de pinza de melaza
#define PIN_US_TRIG         25   // HC-SR04 Trig
#define PIN_US_ECHO         34   // HC-SR04 Echo (con divisor en el real)
#define PIN_RFID_SS          5   // RC522 SDA/SS
#define PIN_RFID_RST        27   // RC522 RST
#define PIN_SD_CS           26   // Módulo SD CS
#define PIN_SPI_SCK         18
#define PIN_SPI_MISO        19
#define PIN_SPI_MOSI        23
#define PIN_LED_ROJO        16
#define PIN_LED_AMARILLO    17
#define PIN_LED_VERDE        2
#define PIN_BUZZER          15   // Buzzer (vía 2N2222 en el real)

// ---------------------------------------------------------------------
//  Hardware
// ---------------------------------------------------------------------
#define RELE_ACTIVO_ALTO   1                  // 1: IN en HIGH energiza el relé. Si tus relés son activos en bajo, pon 0
#define BUZZER_ACTIVO      SIM_O_REAL(0, 1)   // KY-012 real = buzzer activo; Wokwi = pasivo (se le da un tono)
#define DHT_ES_DHT22       SIM_O_REAL(1, 0)   // Wokwi no tiene DHT11
#define CANAL_LEDC_SERVO   2                  // timer 1
#define CANAL_LEDC_BUZZER  4                  // timer 2
#define SERVO_CERRADO_GRAD 0                  // pinza cerrando la manguera
#define SERVO_ABIERTO_GRAD 90

// Divisor analógico: 1 k arriba / (1 k + 820) abajo -> Vpin = Vmodulo * 0,6454
#define DIVISOR_ANALOG     (1820.0f / 2820.0f)
#define ADC_PROMEDIO       64

// ---------------------------------------------------------------------
//  pH (PH-4502C): calibración de 2 puntos con buffers 7 y 4
//  Valores por defecto (voltaje del módulo, antes del divisor)
// ---------------------------------------------------------------------
#define PH_V7_DEF_MV       2530.0f
#define PH_V4_DEF_MV       3070.0f
#define PH_PEND_MIN        100.0f   // |mV/pH| aceptable al calibrar
#define PH_PEND_MAX        250.0f
#define CAL_DEFECTO_VALIDA SIM_O_REAL(1, 0)   // en el real, hasta calibrar se avisa en amarillo

// ---------------------------------------------------------------------
//  Tanque (garrafón de 10 L, ~8 L de caldo, ~23 cm de profundidad)
// ---------------------------------------------------------------------
#define DIST_SENSOR_FONDO_CM 30.0f  // del HC-SR04 (en la tapa) al fondo
#define AREA_TANQUE_CM2      348.0f // 8 L / 23 cm
#define NIVEL_MIN_CM         19.0f  // debajo: calentador descubierto -> se bloquea
#define NIVEL_MAX_CM         26.0f  // encima: riesgo de rebose -> no dosificar

// ---------------------------------------------------------------------
//  Control de temperatura (activación líquida de MM)
// ---------------------------------------------------------------------
#define SETPOINT_DEF_C     30.0f
#define HISTERESIS_C       0.5f
#define T_MAX_C            38.0f
#define T_TOLERANCIA_C     2.0f
#define T_GRACIA_S         SIM_O_REAL(60, 7200)      // tiempo para llegar al setpoint

// pH del proceso
#define PH_MIN_CRITICO     3.0f
#define PH_ALERTA_ALTO     6.5f
#define PH_GRACIA_S        SIM_O_REAL(120, 172800)   // 48 h para que acidifique

// Gases (MQ-135, indicador cualitativo 0–100)
#define GAS_ALERTA_IDX     60.0f
#define GAS_PRECAL_S       SIM_O_REAL(5, 180)

// Aireación por ciclos
#define AIRE_ON_S          SIM_O_REAL(20, 900)
#define AIRE_OFF_S         SIM_O_REAL(20, 900)

// Dosificación de melaza (servo)
#define DOSIS_INTERVALO_S  SIM_O_REAL(90, 86400)
#define DOSIS_ABIERTA_MS   SIM_O_REAL(3000, 8000)
#define DOSIS_REINTENTO_S  SIM_O_REAL(20, 600)

// ---------------------------------------------------------------------
//  Registro, tiempo y nube
// ---------------------------------------------------------------------
#define REG_INTERVALO_S    SIM_O_REAL(5, 60)
#define ESTADO_INTERVALO_S 10
#define LOTE_GUARDAR_S     SIM_O_REAL(15, 120)
#define TIEMPO_GUARDAR_S   SIM_O_REAL(30, 300)
#define ARMADO_TIMEOUT_MS  15000
#define BOTON_LARGO_MS     2500
#define NUBE_LOTE_MAX      20
#define COLA_RAM_MAX       80
#define TZ_POSIX           "<-05>5"          // Colombia, UTC-5 sin horario de verano
#define NTP_1              "pool.ntp.org"
#define NTP_2              "time.google.com"
#define EPOCH_VALIDO       1700000000UL
