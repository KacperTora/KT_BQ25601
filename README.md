# KT_BQ25601

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-00979D?logo=arduino)](https://github.com/KacperTora/KT_BQ25601)
[![PlatformIO Registry](https://img.shields.io/badge/PlatformIO-Registry-F58225?logo=platformio)](https://registry.platformio.org/)
[![License: GPL-3.0](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Arduino and PlatformIO C++ library for the Texas Instruments BQ25601 3A single-cell Li-Ion / LiPo battery charger, power-path management IC, and OTG boost converter.

<p align="center">
  <img src="extras/bq25601_pic.png" alt="KT_BQ25601 Board" width="600">
</p>

Tested and validated on custom hardware powered by the Texas Instruments BQ25601. (Contact me on github if you are interested in this module)

---

## Key Features

* **Complete Hardware Coverage:** Full register (REG00 through REG0B).
* **NVDC Power-Path Management:** Regulate system voltage dynamically while charging single-cell batteries.
* **Configurable Charge Parameters:** Fast charge current (up to 3000 mA), pre-charge, termination, and battery regulation voltage.
* **Dynamic Power Management (DPM):** Input voltage (VINDPM) and input current (IINDPM) limiting to prevent host port or weak adapter collapse.
* **OTG Boost Converter:** Step up battery voltage to power external USB accessories.
* **Watchdog Protection:** Configurable watchdog timer control.
* **Low-Power Shipping Mode:** Disconnect BATFET to prevent self-discharge during long storage or transit.
* **Cross-Platform Compatibility:** Works with ESP32, ESP8266, STM32, RP2040, and Arduino AVR architectures.

---

## Wiring & Pinout

The BQ25601 communicates over standard I2C (7-bit address: `0x6B`).

| BQ25601 Pin | MCU / Connection | Notes |
| :--- | :--- | :--- |
| **SDA** | MCU SDA | Requires 4.7kΩ – 10kΩ pull-up |
| **SCL** | MCU SCL | Requires 4.7kΩ – 10kΩ pull-up|
| **GND** | System GND | Ground |
| **INT** | Digital GPIO | Active-low interrupt |
| **STAT** | Status LED | Open-drain charging status indicator |
| **PG** | Status LED | Open-drain power good indicator |

---

## Basic Configuration Example

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <KT_BQ25601.h>

#define BATTERY_CAPACITY_MAH 800

KT_BQ25601 Charger;

void setup(){
  Serial.begin(115200);
  delay(1000);

  Wire.begin();

  while (!Charger.begin(&Wire)) {
    Serial.println("Couldnt find BQ25601.");
    delay(2000);
  }
  Serial.println("BQ25601 initialized successfully.");
  Serial.print("Part Number: 0x");
  Serial.println(Charger.getPartNumber(), HEX);
  Serial.print("Device Revision: 0x");
  Serial.println(Charger.getDeviceRevision(), HEX);

  // 0.5C = 400 mA
  Charger.setChargeCurrent(BATTERY_CAPACITY_MAH / 2);
  Charger.setPrechargeCurrent(60);
  Charger.setTerminationCurrent(60);
  Charger.setInputCurrentLimit(500);
  Charger.setChargeVoltage(4208);

  uint16_t inputCurrentLimit = Charger.getInputCurrentLimit();
  uint16_t chargeVoltage = Charger.getChargeVoltage();
  uint16_t chargeCurrent = Charger.getChargeCurrent();
  uint16_t preChargeCurrent = Charger.getPrechargeCurrent();
  uint16_t terminationCurrent = Charger.getTerminationCurrent();

  Serial.print(F("Input Current Limit: "));
  Serial.print(inputCurrentLimit);
  Serial.println(F(" mA"));

  Serial.print(F("Charge Voltage: "));
  Serial.print(chargeVoltage);
  Serial.println(F(" mV"));

  Serial.print(F("Charge Current: "));
  Serial.print(chargeCurrent);
  Serial.println(F(" mA"));

  Serial.print(F("Precharge Current: "));
  Serial.print(preChargeCurrent);
  Serial.println(F(" mA"));

  Serial.print(F("Termination Current: "));
  Serial.print(terminationCurrent);
  Serial.println(F(" mA"));
}

void loop(){}
```

---

## API Reference

### Initialization & Watchdog
* `bool begin(TwoWire *wire = nullptr)`: Initializes I2C and disables the internal watchdog timer.
* `bool isConnected()`: Pings the I2C address (`0x6B`) to verify device presence.
* `bool setWatchdog(kt_bq25601_watchdog_enum watchdog)`: Set timer (`DISABLE`, `40S`, `80S`, `160S`).
* `bool resetWatchdog()`: Reset the hardware watchdog timer.
* `bool reset()`: Reset all registers to factory defaults.

### Charging Configuration
* `bool enableCharging(bool enable)`: Software enable/disable charging.
* `bool setChargeCurrent(uint16_t current_mA)`: Fast-charge current (0 to 3000 mA, 60 mA step).
* `bool setChargeVoltage(uint16_t voltage_mV)`: Target battery regulation voltage (3856 to 4624 mV, 32 mV step).
* `bool setPrechargeCurrent(uint16_t current_mA)`: Pre-charge current (60 to 780 mA, 60 mA step).
* `bool setTerminationCurrent(uint16_t current_mA)`: Termination current (60 to 780 mA, 60 mA step).
* `bool enableTermination(bool enable)`: Enable/disable automatic charge termination.
* `bool setRechargeThreshold(kt_bq25601_recharge_threshold_enum threshold)`: 100 mV or 200 mV recharge drop.

### Power Management & Dynamic Limits
* `bool setInputCurrentLimit(uint16_t current_mA)`: Set IINDPM limit (100 to 3200 mA, 100 mA step).
* `bool setInputVoltageThreshold(uint16_t voltage_mV)`: Set VINDPM threshold (3900 to 5400 mV, 100 mV step).
* `uint16_t getInputVoltageThreshold()`: Read back currently programmed VINDPM threshold in mV.
* `bool setMinVoltage(kt_bq25601_sys_min_voltage_enum voltage)`: Minimum NVDC system voltage (2.6V to 3.7V).
* `bool enableHIZ(bool enable)`: Put device into High-Impedance mode.
* `bool enableShippingMode(bool enable)`: Turn off BATFET to isolate battery from the system.

### OTG (5V Boost Mode / Power Bank)
* `bool enableOTG(bool enable)`: Turn on 5.15V boost converter output on VBUS.
* `bool setCurrentLimitOTG(kt_bq25601_current_limit_otg_enum limit)`: Limit OTG current (0.5A or 1.2A).
* `bool setRegulationVoltageOTG(kt_bq25601_regulation_voltage_otg_enum v)`: 4.85V, 5.0V, 5.15V, or 5.3V.
* `bool minVoltageOTG(kt_bq25601_min_voltage_otg_enum voltage)`: Battery cutoff voltage for OTG (2.5V or 2.8V).

### Status & Diagnostics
* `bool isCharging()`: Returns `true` if currently in pre-charge or fast-charge mode.
* `bool isCharged()`: Returns `true` when charge cycle completes (termination reached).
* `bool isPowerPresent()`: Returns `true` if external VBUS input is attached.
* `bool isPowerGood()`: Returns `true` if input source voltage is valid and qualified.
* `kt_bq25601_power_status_enum getPowerStatus()`: Reports input type (`NO_INPUT`, `USB_HOST`, `ADAPTER`, `OTG`).
* `kt_bq25601_faults getFaults()`: Reads all fault flags from `REG09`.

---

## Installation

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps =
    KacperTora/KT_BQ25601 @ ^1.1.0
```

### Arduino IDE
1. Open Arduino IDE -> **Sketch** -> **Include Library** -> **Manage Libraries...**
2. Search for `KT_BQ25601`
3. Click **Install**

---

## Documentation & References

* [Texas Instruments BQ25601 Official Datasheet](https://www.ti.com/document-viewer/bq25601/datasheet)

## License

This library is licensed under the **GNU General Public License v3.0**.