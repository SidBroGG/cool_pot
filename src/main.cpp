#include <Arduino.h>
#include <SettingsGyver.h>
SettingsGyver sett("Gorshok");

float temp = 0.0f;
float hum = 0.0f;
float soid = 0.0f;
int fan_slider = 0;

void build(sets::Builder& b) {
    b.LabelFloat("Температура воздуха", temp);
    b.LabelFloat("Влажность воздуха", hum);
    b.LabelFloat("Влажность почвы", soid);
    b.Slider("Температура включения вентилятора", 0, 52, 1, "", &fan_slider);
}

void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);
    WiFi.softAP("Gorshok");
    Serial.println(WiFi.softAPIP());

    sett.begin();
    sett.onBuild(build);
}

void loop() { sett.tick(); }