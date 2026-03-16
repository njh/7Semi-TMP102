/**
 * 7Semi TMP102 Arduino Library
 *
 * Texas Instruments TMP102 Digital Temperature Sensor
 *
 * - Interface: I2C
 * - Resolution: 12-bit (default) or 13-bit extended
 * - Temperature resolution: 0.0625 °C
 * - Supply voltage: 1.4V – 3.6V
 *
 * Supported platforms:
 * - Arduino AVR boards
 * - ESP8266
 * - ESP32
 * - Any board supporting TwoWire I2C
 *
 * Library features:
 * - Temperature reading (Celsius / Fahrenheit)
 * - One-shot conversion
 * - Shutdown power mode
 * - Alert thresholds
 * - Fault queue configuration
 * - Conversion rate control
 * - Extended temperature mode
 *
 * Design goals:
 * - Clean I2C abstraction
 * - Minimal RAM usage
 * - Register-safe configuration
 */

#ifndef _7SEMI_TMP102_H_
#define _7SEMI_TMP102_H_

#include <Arduino.h>
#include <Wire.h>

/**
 * TMP102 Register Map
 *
 * All registers are 16-bit wide.
 * MSB transmitted first over I2C.
 */
enum TMP102_REGISTER
{
    REG_TEMP = 0x00,   /** Temperature register */
    REG_CONFIG = 0x01, /** Configuration register */
    REG_T_LOW = 0x02,  /** Low temperature alert threshold */
    REG_T_HIGH = 0x03  /** High temperature alert threshold */
};

/**
 * Configuration Register Bit Positions
 *
 * Layout (MSB → LSB)
 *
 * 15  SD   Shutdown mode
 * 14  TM   Thermostat mode
 * 13  POL  Alert polarity
 * 12  F1
 * 11  F0   Fault queue
 * 10  R1
 * 9   R0   Conversion rate
 * 8   OS   One-shot start
 * 7   EM   Extended mode
 * 6-0 Reserved
 */
enum TMP102_CONFIG_BITS
{
    TMP102_SD = 8,   /** Shutdown mode */
    TMP102_TM = 9,   /** Thermostat mode */
    TMP102_POL = 10, /** Alert polarity */
    TMP102_F0 = 11,  /** Fault queue bit 0 */
    TMP102_F1 = 12,  /** Fault queue bit 1 */
    TMP102_CR0 = 13, /** Conversion rate bit 0 */
    TMP102_CR1 = 14, /** Conversion rate bit 1 */
    TMP102_OS = 15,  /** One-shot conversion trigger */
    TMP102_EM = 4,   /** Extended mode */
    TMP102_AL = 5    /** Alert status */
};

/**
 * TMP102 Conversion Rate
 *
 * Controls how often temperature conversions occur
 * in continuous measurement mode.
 */
enum TMP102_CONVERSION_RATE
{
    TMP102_CONVERSION_RATE_0_25_HZ = 0, /** 0.25 conversions per second */
    TMP102_CONVERSION_RATE_1_HZ = 1,    /** 1 conversion per second */
    TMP102_CONVERSION_RATE_4_HZ = 2,    /** 4 conversions per second */
    TMP102_CONVERSION_RATE_8_HZ = 3     /** 8 conversions per second */
};

/**
 * TMP102 Fault Queue
 *
 * Determines how many consecutive fault conditions
 * must occur before ALERT pin is triggered.
 */
enum TMP102_FAULT_QUEUE
{
    TMP102_FAULT_QUEUE_1 = 0, /** Alert after 1 fault */
    TMP102_FAULT_QUEUE_2 = 1, /** Alert after 2 faults */
    TMP102_FAULT_QUEUE_4 = 2, /** Alert after 4 faults */
    TMP102_FAULT_QUEUE_6 = 3  /** Alert after 6 faults */
};

/**
 * ALERT Polarity Configuration
 *
 * Determines electrical behavior of ALERT output pin.
 */
enum TMP102_ALERT_POLARITY
{
    TMP102_ALERT_ACTIVE_LOW = 0,  /** ALERT pulls LOW when triggered */
    TMP102_ALERT_ACTIVE_HIGH = 1  /** ALERT drives HIGH when triggered */
};

/**
 * Thermostat Alert Mode
 *
 * Comparator mode
 * - ALERT active while temperature exceeds threshold
 *
 * Interrupt mode
 * - ALERT latched until temperature returns
 */
enum TMP102_THERM_ALERT
{
    TMP102_COMPARATOR_MODE = 0,
    TMP102_INTERRUPT_MODE = 1
};

/**
 * TMP102 Main Driver Class
 *
 * Handles all sensor communication and configuration.
 *
 * Features:
 * - Temperature measurement
 * - Alert configuration
 * - Power management
 * - Conversion timing control
 */
class TMP102_7Semi
{
public:

    /**
     * Constructor
     *
     * Allows passing a custom TwoWire interface.
     *
     * Example:
     * - Wire
     * - Wire1 (ESP32)
     */
    TMP102_7Semi(TwoWire &wirePort = Wire);

    /**
     * Initialize the sensor and I2C bus
     *
     * Configuration options:
     * - Custom I2C address
     * - Custom SDA/SCL pins
     * - Custom I2C clock speed
     */
    bool begin(uint8_t i2cAddress = 0x49,
               uint8_t sda = 0xFF,
               uint8_t scl = 0xFF,
               uint32_t i2cClockSpeed = 400000);

    /** Read temperature in Celsius */
    bool readTemperatureC(float &temperatureC);

    /** Read temperature in Fahrenheit */
    bool readTemperatureF(float &temperatureF);

    /** Read raw temperature register value */
    bool readRawTemperature(int16_t &rawTemperature);

    /** Enable or disable shutdown mode */
    bool setShutdown(bool enable);

    /** Trigger one-shot temperature conversion */
    bool startOneShot();

    /** Configure alert behavior */
    bool setAlert(bool interruptMode,
                  TMP102_ALERT_POLARITY polarity);

    /** Read ALERT status */
    bool getAlert(bool &alert);

    /** Configure thermostat alert mode */
    bool setThermAlertMode(TMP102_THERM_ALERT mode);
    bool getThermAlertMode(bool &mode);

    /** Configure ALERT polarity */
    bool setAlertPolarity(TMP102_ALERT_POLARITY polarity);
    bool getAlertPolarity(bool &polarity);

    /** Enable or disable extended temperature mode */
    bool setExtendedMode(bool enable);
    bool getExtendedMode(bool &enabled);

    /** Configure temperature conversion rate */
    bool setConversionRate(TMP102_CONVERSION_RATE rate);
    bool getConversionRate(uint8_t &rate);

    /** Configure fault queue */
    bool setFaultQueue(TMP102_FAULT_QUEUE faults);
    bool getFaultQueue(uint8_t &faults);

    /** Set high temperature alert limit */
    bool setHighLimit(float tempC);

    /** Set low temperature alert limit */
    bool setLowLimit(float tempC);

    /** Read high temperature limit */
    bool getHighLimit(float &tempC);

    /** Read low temperature limit */
    bool getLowLimit(float &tempC);

private:

    /** Pointer to I2C interface */
    TwoWire *i2c;

    /** Device I2C address */
    uint8_t address;

    /** Read 16-bit register */
    bool readReg(uint8_t reg, uint16_t &value);

    /** Write 16-bit register */
    bool writeReg(uint8_t reg, uint16_t value);

    /** Modify multi-bit field */
    bool setBits(uint8_t reg,
                 uint8_t position,
                 uint8_t number_of_bits,
                 uint8_t value);

    /** Read multi-bit field */
    bool getBits(uint8_t reg,
                 uint8_t position,
                 uint8_t number_of_bits,
                 uint8_t &value);

    /** Set single configuration bit */
    bool setBit(uint8_t reg,
                uint8_t position,
                bool value);

    /** Read single configuration bit */
    bool getBit(uint8_t reg,
                uint8_t position,
                bool &value);

    /** Convert raw register value to Celsius */
    float rawToCelsius(int16_t raw);

    /** Convert Celsius temperature to raw register format */
    int16_t celsiusToRaw(float temp);
};

#endif