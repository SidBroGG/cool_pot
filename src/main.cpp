#include <Arduino.h>
#include <HX710B.h>
#include <SettingsGyver.h>

#define SOIL_MOISTURE_PIN 34
#define PRESSURE_OUT 35
#define PRESSURE_SCK 32

SettingsGyver sett("Gorshok");
HX710B pressure_module;

float temp = 0.0f;
float hum = 0.0f;
float soid = 0.0f;
float pressure = 0.0f;
int fan_slider = 0;

void read_pressure() {
    if (pressure_module.is_ready()) {
        pressure = pressure_module.atm();
    }
}

void build(sets::Builder& b) {
    if (b.beginGroup("Датчики")) {
        b.LabelFloat("Температура воздуха", temp);
        b.LabelFloat("Влажность воздуха", hum);
        b.LabelFloat("Влажность почвы", soid);
        b.LabelFloat("Атмосферное Давление", pressure);
        b.endGroup();
    }

    b.Slider("Вентилятор", 0, 52, 1, "", &fan_slider);
}

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);
    WiFi.softAP("Gorshok");
    Serial.println(WiFi.softAPIP());

    sett.begin();
    sett.onBuild(build);

    pressure_module.begin(PRESSURE_OUT, PRESSURE_SCK);
}

void loop() {
    sett.tick();
}