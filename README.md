<p align="center">
  <kbd><a href="#-русский">🇷🇺 Русский</a></kbd>
  <kbd><a href="#-english">🇬🇧 English</a></kbd>
</p>

---

## 🇷🇺 Русский

**Cool Pot / «Горшок»** — проект на **ESP32 (Arduino + PlatformIO)** для мониторинга состояния растения: влажность почвы, температура и влажность воздуха (и заготовка под освещённость) с простым веб-интерфейсом.

### Фичи
- 🌱 **Влажность почвы** (аналоговый датчик) → проценты (0–100) с калибровкой.
- 🌡️ **Температура / влажность воздуха** через **DHT11**.
- 📶 ESP32 поднимает **Wi-Fi точку доступа** `Gorshok` и показывает данные в браузере.
- 🖥️ Веб-панель построена на **GyverLibs/Settings**.

### Железо
- ESP32 DevKit (env `esp32dev`)
- DHT11
- Датчик влажности почвы (аналоговый)
- (опционально) фоторезистор/LDR + делитель напряжения

**Пины (по умолчанию в коде):**
- Soil moisture: `GPIO34` (ADC)
- DHT11: `GPIO33`
- LDR: `GPIO35`

### Как запустить
1. Установите **PlatformIO** (VS Code или CLI).
2. Откройте проект и прошейте:
   - `pio run -t upload`
3. Откройте монитор порта (опционально):
   - `pio device monitor -b 115200`
4. Подключитесь к Wi-Fi сети **`Gorshok`**.
5. Откройте адрес, который выводится в Serial Monitor (обычно это `192.168.4.1`).

### Настройка калибровки
Значения `SOIL_MOISTURE_MIN` и `SOIL_MOISTURE_MAX` зависят от конкретного датчика/почвы. Подберите их под свои условия (сухо/в воде), чтобы проценты были корректными.

---

## 🇬🇧 English

**Cool Pot / “Gorshok”** is an **ESP32 (Arduino + PlatformIO)** project for plant monitoring: soil moisture, air temperature & humidity (plus a stub for light level) with a simple web UI.

### Features
- 🌱 **Soil moisture** (analog sensor) → 0–100% with calibration.
- 🌡️ **Air temperature & humidity** via **DHT11**.
- 📶 ESP32 starts a **Wi-Fi access point** `Gorshok` and serves a browser dashboard.
- 🖥️ Web UI is built with **GyverLibs/Settings**.

### Hardware
- ESP32 DevKit (env `esp32dev`)
- DHT11
- Analog soil moisture sensor
- (optional) photoresistor/LDR + voltage divider

**Pins (defaults in code):**
- Soil moisture: `GPIO34` (ADC)
- DHT11: `GPIO33`
- LDR: `GPIO35`

### Usage
1. Install **PlatformIO** (VS Code or CLI).
2. Build & flash:
   - `pio run -t upload`
3. Serial monitor (optional):
   - `pio device monitor -b 115200`
4. Connect to the Wi-Fi network **`Gorshok`**.
5. Open the IP printed in Serial Monitor (usually `192.168.4.1`).

### Calibration
`SOIL_MOISTURE_MIN` and `SOIL_MOISTURE_MAX` depend on your sensor/soil. Measure “dry” vs “wet” readings and adjust these constants for accurate percentages.

---

## License
Apache-2.0 (see `LICENSE`).
