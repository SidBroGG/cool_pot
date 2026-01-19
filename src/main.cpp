#include <Arduino.h>
#include <DHT.h>
#include <SettingsGyver.h>

#define SOIL_MOISTURE_PIN 34
#define DHT_PIN 33
#define LDR_PIN 35

#define SOIL_MOISTURE_MIN 3632
#define SOIL_MOISTURE_MAX 2141

#define LDR_MIN 4096
#define LDR_MAX 0

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

float getSoilMoisture() {
    float result = analogRead(SOIL_MOISTURE_PIN);
    result = map(result, SOIL_MOISTURE_MIN, SOIL_MOISTURE_MAX, 0, 100);
    if (result < 0) result = 0;

    return result;
}

float getIlluminationLevel() {
    float result = analogRead(LDR_PIN);
    result = map(result, LDR_MIN, LDR_MAX, 0, 100);
    if (result < 0) result = 0;

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

    {
        sets::Menu menu(b, "Графики");

        b.PlotStack(10, "Температура");
        b.PlotStack(11, "Влажность");
        b.PlotStack(12, "Почва");
        b.PlotStack(13, "Свет");
    }
}

void update(sets::Updater& upd) {
    upd.update(0, temp);
    upd.update(1, hum);
    upd.update(2, soil_moisture);
    upd.update(3, illumination_level);

    float temp_plot[] = {temp};
    upd.updatePlot(10, temp_plot);

    float hum_plot[] = {hum};
    upd.updatePlot(11, hum_plot);

    float soil_plot[] = {soil_moisture};
    upd.updatePlot(12, soil_plot);

    float illum_plot[] = {illumination_level};
    upd.updatePlot(13, illum_plot);
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
        illumination_level = getIlluminationLevel();

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