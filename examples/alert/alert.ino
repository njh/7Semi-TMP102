/**
 * 7Semi TMP102 Arduino Library
 *
 * Example: Temperature Alert Mode
 *
 * Description
 * - Demonstrates thermostat alert feature
 * - ALERT pin triggers when temperature exceeds threshold
 *
 * Library Features Used
 * - setHighLimit()
 * - setLowLimit()
 * - setAlert()
 * - getAlert()
 *
 * Connection Guide
 * TMP102      Arduino
 * ---------------------
 * VCC   ->    3.3V
 * GND   ->    GND
 * SDA   ->    SDA
 * SCL   ->    SCL
 * ALERT ->    Digital pin (optional)
 *
 * Notes
 * - ALERT pin can be connected to an interrupt
 * - Useful for over-temperature protection
 */

#include <7Semi_TMP102.h>

TMP102_7Semi tmp;

void setup() {
  Serial.begin(115200);

  tmp.begin();

  /**
     * Configure alert thresholds
     */
  tmp.setHighLimit(25);

  float high_limit = 0;
  tmp.getHighLimit(high_limit);
  Serial.print("HIGH Limit: ");
  Serial.println(high_limit);

  tmp.setLowLimit(20);

  Serial.print("LOW Limit: ");
  float low_limit = 0;
  tmp.getLowLimit(low_limit);
  Serial.println(low_limit);

  /**
     * Configure alert mode
     */
  tmp.setAlert(true, TMP102_ALERT_ACTIVE_LOW);

  Serial.println("TMP102 Alert Mode Example");
}

void loop() {
  float temp;
  bool alert;

  tmp.readTemperatureC(temp);
  tmp.getAlert(alert);

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C");

  if (alert)
    Serial.println("  ALERT!");
  else
    Serial.println();

  delay(1000);
}