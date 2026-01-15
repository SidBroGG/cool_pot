#include <Arduino.h>
#include <HX710B.h>
#include <SettingsGyver.h>

#define SOIL_MOISTURE_PIN 34
#define PRESSURE_OUT 35
#define PRESSURE_SCK 32

#define SOIL_MOISTURE_MIN 3632
#define SOIL_MOISTURE_MAX 2141

TaskHandle_t wifi_task;
TaskHandle_t sensors_task;

// Libs variables
SettingsGyver sett("Gorshok");
// HX710B pressure_module;

// Variables
float temp = 0.0f;
float hum = 0.0f;
float soil_moisture = 0.0f;
// float pressure = 0.0f;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// float getPressure() {
//     float result = pressure_module.atm();
//     result += 1.5;
//     return result;
// }

float getSoilMoisture() {
    float result = analogRead(SOIL_MOISTURE_PIN);
    map(result, SOIL_MOISTURE_MIN, SOIL_MOISTURE_MAX, 0, 100);

    return result;
}

void build(sets::Builder& b) {
    if (b.beginGroup("Датчики")) {
        b.LabelFloat(0, "Температура воздуха", temp);
        b.LabelFloat(1, "Влажность воздуха", hum);
        b.LabelFloat(2, "Влажность почвы", soil_moisture);
        // b.LabelFloat(3, "Атмосферное Давление", pressure);
        b.endGroup();
    }
}

void update(sets::Updater& upd) {
    upd.update(0, temp);
    upd.update(1, hum);
    upd.update(2, soil_moisture);
    // upd.update(3, pressure);
}

void wifiTaskFunc(void* pvParameters) {
    for (;;) {
        sett.tick();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void sensorsTaskFunc(void* pvParameters) {
    for (;;) {
        // pressure = getPressure();
        soil_moisture = getSoilMoisture();

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void setup() {
    Serial.begin(115200);

    // WiFI hotspot setup
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Gorshok");
    Serial.println(WiFi.softAPIP());

    // Settings setup
    sett.begin();
    sett.onBuild(build);
    sett.onUpdate(update);

    // Pressure module setup
    // analogReadResolution(12);
    // analogSetPinAttenuation(SOIL_MOISTURE_PIN, ADC_11db);
    // pressure_module.begin(PRESSURE_OUT, PRESSURE_SCK);
    // while (!pressure_module.is_ready()) {
    //     delay(100);
    // }

    // Pin wifi handle to core 1
    xTaskCreatePinnedToCore(
        wifiTaskFunc,
        "Wifi",
        10000,
        NULL,
        1,
        &wifi_task,
        0);

    // Pin sensors handle to core 2
    xTaskCreatePinnedToCore(
        sensorsTaskFunc,
        "Sensors",
        10000,
        NULL,
        1,
        &sensors_task,
        1);
}

void loop() {}