/**
 * 7Semi TMP102 Arduino Library
 *
 * Basic Temperature Reading Example
 *
 * Description
 * - Demonstrates the simplest way to read temperature
 * - Uses default I2C address and default I2C pins
 * - Reads temperature in Celsius and Fahrenheit
 *
 * Library Features Used
 * - begin()
 * - readTemperatureC()
 * - readTemperatureF()
 *
 * Connection Guide
 * TMP102      Arduino
 * ---------------------
 * VCC   ->    3.3V
 * GND   ->    GND
 * SDA   ->    SDA
 * SCL   ->    SCL
 *
 * Default I2C Address
 * - 0x48
 */

#include <7Semi_TMP102.h>

TMP102_7Semi tmp;

void setup()
{
    Serial.begin(115200);

    /**
     * Initialize sensor
     */
    tmp.begin(0x49);

    Serial.println("TMP102 Basic Temperature Example");
}

void loop()
{
    float temperatureC;
    float temperatureF;

    if (tmp.readTemperatureC(temperatureC))
    {
        tmp.readTemperatureF(temperatureF);

        Serial.print("Temperature: ");
        Serial.print(temperatureC);
        Serial.print(" °C  |  ");

        Serial.print(temperatureF);
        Serial.println(" °F");
    }
    else
    {
        Serial.println("Temperature read failed");
    }

    delay(1000);
}