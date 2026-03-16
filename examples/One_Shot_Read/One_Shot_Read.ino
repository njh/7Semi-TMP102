/**
 * 7Semi TMP102 Arduino Library
 *
 * One-Shot Temperature Measurement Example
 *
 * Description
 * - Demonstrates low power temperature measurement
 * - Sensor remains in shutdown mode
 * - Conversion occurs only when requested
 *
 * Library Features Used
 * - setShutdown()
 * - startOneShot()
 * - readTemperatureC()
 *
 * Application
 * - Battery powered devices
 * - IoT nodes
 * - Environmental logging
 *
 * Connection Guide
 * TMP102      Arduino
 * ---------------------
 * VCC   ->    3.3V
 * GND   ->    GND
 * SDA   ->    SDA
 * SCL   ->    SCL
 */

#include <7Semi_TMP102.h>

TMP102_7Semi tmp;

void setup()
{
    Serial.begin(115200);

    tmp.begin();

    /**
     * Enable shutdown mode
     */
    tmp.setShutdown(true);

    Serial.println("TMP102 One-Shot Mode Example");
}

void loop()
{
    float temperature;

    /**
     * Trigger one-shot conversion
     */
    tmp.startOneShot();

    delay(30);

    if (tmp.readTemperatureC(temperature))
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" °C");
    }

    delay(3000);
}