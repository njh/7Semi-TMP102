/**
 * 7Semi TMP102 Arduino Library
 *
 * Extended Resolution Mode Example
 *
 * Description
 * - Demonstrates enabling extended temperature mode
 * - Allows 13-bit temperature resolution
 *
 * Library Features Used
 * - setExtendedMode()
 * - readTemperatureC()
 *
 * Connection Guide
 * TMP102      Arduino
 * ---------------------
 * VCC   ->    3.3V
 * GND   ->    GND
 * SDA   ->    SDA
 * SCL   ->    SCL
 *
 * Notes
 * - Extended mode increases temperature range
 * - Useful for industrial applications
 */

#include <7Semi_TMP102.h>

TMP102_7Semi tmp;

void setup()
{
    Serial.begin(115200);

    tmp.begin();

    /**
     * Enable extended temperature mode
     */
    tmp.setExtendedMode(true);

    Serial.println("TMP102 Extended Mode Example");
}

void loop()
{
    float temp;

    if (tmp.readTemperatureC(temp))
    {
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.println(" °C");
    }

    delay(1000);
}