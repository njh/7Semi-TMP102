/**
 * 7Semi TMP102 Arduino Library
 *
 *  Continuous Temperature Monitoring Example
 *
 * Description
 * - Demonstrates continuous temperature measurements
 * - Configures sensor conversion rate
 * - Shows how to monitor temperature trends
 *
 * Library Features Used
 * - setConversionRate()
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
 * - Higher conversion rate increases update speed
 * - Also increases power consumption
 */

#include <7Semi_TMP102.h>

TMP102_7Semi tmp;

void setup()
{
    Serial.begin(115200);

    tmp.begin();

    /**
     * Set conversion rate
     *
     * Options
     * - 0.25 Hz
     * - 1 Hz
     * - 4 Hz
     * - 8 Hz
     */
    tmp.setConversionRate(TMP102_CONVERSION_RATE_4_HZ);

    Serial.println("Continuous Temperature Monitoring");
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

    delay(500);
}