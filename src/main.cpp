#include <Arduino.h>
#include <DHT.h>
#include <SettingsGyver.h>

#define SOIL_MOISTURE_PIN 34
#define DHT_PIN 33
#define LDR_PIN 31

#define SOIL_MOISTURE_MIN 3632
#define SOIL_MOISTURE_MAX 2141

#define LDR_MIN 0
#define LDR_MAX 4096

TaskHandle_t wifi_task;
TaskHandle_t sensors_task;

// Libs variables
SettingsGyver sett("Gorshok");
DHT dht(DHT_PIN, DHT11);

// Variables
float temp = 0.0f;
float hum = 0.0f;
float soil_moisture = 0.0f;
float illumination_level = 0.0f;

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float getSoilMoisture() {
    float result = analogRead(SOIL_MOISTURE_PIN);
    result = mapfloat(result, SOIL_MOISTURE_MIN, SOIL_MOISTURE_MAX, 0, 100);

    return result;
}

float getIlluminationLevel() {
    float result = analogRead(LDR_PIN);
    result = mapfloat(result, LDR_MIN, LDR_MAX, 0, 100);

    return result;
}

void build(sets::Builder& b) {
    if (b.beginGroup("Датчики")) {
        b.LabelFloat(0, "Температура воздуха (°C)", temp);
        b.LabelFloat(1, "Влажность воздуха (%)", hum);
        b.LabelFloat(2, "Влажность почвы (%)", soil_moisture);
        b.LabelFloat(3, "Уровень освещенности (%)", illumination_level);

        b.endGroup();
    }
}

void update(sets::Updater& upd) {
    upd.update(0, temp);
    upd.update(1, hum);
    upd.update(2, soil_moisture);
    upd.update(3, illumination_level);
}

void wifiTaskFunc(void* pvParameters) {
    for (;;) {
        sett.tick();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void sensorsTaskFunc(void* pvParameters) {
    for (;;) {
        soil_moisture = getSoilMoisture();
        temp = dht.readTemperature();
        hum = dht.readHumidity();

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

    // Temperature / Humidity module setup
    dht.begin();

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