# 7Semi TMP102 Arduino Library

Arduino driver for the TMP102 digital temperature sensor.

The TMP102 is a high-accuracy digital temperature sensor with an I²C interface designed for applications such as environmental monitoring, industrial temperature sensing, IoT devices, and thermal protection systems.

This library provides a simple interface for configuring the sensor and reading temperature values in Celsius and Fahrenheit.

## Features

- Temperature measurement (°C)

- Temperature measurement (°F)

- Raw temperature register reading

- One-shot temperature conversion

- Shutdown power-saving mode

- High and low temperature threshold configuration

- Extended temperature resolution mode

## Supported Platforms

- Arduino UNO / Mega

- ESP32

- Any board supporting the Wire (I²C) library

## Hardware

### Supported sensor:

- 7Semi Digital Temperature Sensor I2C Breakout - TMP102

## Connection

The TMP102 communicates using I²C.

## I²C Connection
TMP102 Pin	MCU Pin	Description
| TMP102 Pin | MCU Pin | Description  |
| ---------- | ------- | ------------ |
| VCC        | 3.3V    | Sensor power |
| GND        | GND     | Ground       |
| SCL        | SCL     | I²C clock    |
| SDA        | SDA     | I²C data     |


## Default sensor address:

### 0x48

### Available addresses depending on ADD0 pin configuration:

- 0x48
- 0x49
- 0x4A
- 0x4B

## Recommended I²C speed:

- 100 kHz – 400 kHz
---
## Installation
### Arduino Library Manager

- Open Arduino IDE

- Go to Library Manager

- Search for 7Semi TMP102

- Click Install

- Manual Installation

### Download this repository as ZIP

- Arduino IDE → Sketch → Include Library → Add .ZIP Library

## Example
```cpp
#include <7Semi_TMP102.h>

TMP102_7Semi tmp;

void setup()
{
  Serial.begin(115200);

  if(!tmp.begin())
  {
    Serial.println("TMP102 not detected");
    while(1);
  }
}

void loop()
{
  float temperature;

  if(tmp.readTemperatureC(temperature))
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  }

  delay(1000);
}
```
---
# Library Overview
## Reading Temperature (Celsius)
```cpp
float temp;

tmp.readTemperatureC(temp);

Serial.print("Temperature: ");
Serial.println(temp);
```

Returns temperature in degrees Celsius.

## Reading Temperature (Fahrenheit)
```cpp
float tempF;

tmp.readTemperatureF(tempF);

Serial.print("Temperature F: ");
Serial.println(tempF);
```
Returns temperature in degrees Fahrenheit.

## Reading Raw Temperature Register
```cpp
int16_t raw;

tmp.readRawTemperature(raw);

Serial.print("Raw Temperature: ");
Serial.println(raw);
```

Returns raw temperature register value from the sensor.

## Sensor Configuration
## Conversion Rate

Controls how often the sensor performs temperature conversions.
```cpp
tmp.setConversionRate(TMP102_CONVERSION_RATE_4_HZ);
```
Available conversion rates:
```cpp
TMP102_CONVERSION_RATE_0_25_HZ
TMP102_CONVERSION_RATE_1_HZ
TMP102_CONVERSION_RATE_4_HZ
TMP102_CONVERSION_RATE_8_HZ
```
Higher conversion rates update temperature faster but consume more power.

## Shutdown Mode (Power Saving)

Shutdown mode stops continuous conversions to reduce power consumption.
```cpp
tmp.setShutdown(true);
```
This is useful for battery-powered applications.

## One-Shot Temperature Measurement

Trigger a single temperature conversion while in shutdown mode.
```cpp
tmp.startOneShot();
```
Useful for low power periodic measurements.

## Alert Configuration

The TMP102 provides a thermostat alert system.

Example:
```CPP
tmp.setHighLimit(30);
tmp.setLowLimit(20);

tmp.setAlert(true, TMP102_ALERT_ACTIVE_LOW);
```
This configures alert thresholds and enables alert mode.

## Fault Queue

Controls how many consecutive faults must occur before an alert is triggered.
```CPP
tmp.setFaultQueue(TMP102_FAULT_QUEUE_4);
```
Available options:
```CPP
TMP102_FAULT_QUEUE_1
TMP102_FAULT_QUEUE_2
TMP102_FAULT_QUEUE_4
TMP102_FAULT_QUEUE_6
```
This helps prevent false alert triggers.

## Alert Polarity

Configures the electrical behavior of the ALERT pin.
```CPP
tmp.setAlertPolarity(TMP102_ALERT_ACTIVE_HIGH);
```
Available options:
```CPP
TMP102_ALERT_ACTIVE_LOW
TMP102_ALERT_ACTIVE_HIGH
```
## Extended Temperature Mode

Enable extended temperature resolution.
```CPP
tmp.setExtendedMode(true);
```
Extended mode allows the sensor to use 13-bit temperature resolution.

---

## Example Applications

### Typical applications include:

- Environmental temperature monitoring

- Industrial temperature sensing

- IoT devices

- Thermal protection systems

- HVAC monitoring

- Data center temperature logging

- Smart home temperature sensing

## License

### MIT License

## Author

### 7Semi
