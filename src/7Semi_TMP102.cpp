/**
 * 7Semi TMP102 Arduino Library
 *
 * - Device: Texas Instruments TMP102 Digital Temperature Sensor
 * - Interface: I2C (7-bit address)
 * - Platform: Arduino / ESP32 / ESP8266 compatible
 *
 * Sensor features:
 * - Temperature resolution: 12-bit (default) or 13-bit extended mode
 * - Temperature LSB:
 *      - Normal mode   = 0.0625 °C
 *      - Extended mode = 0.0625 °C
 *
 * Register format:
 * - 16-bit registers
 * - MSB transmitted first over I2C
 *
 * Library design goals:
 * - Safe register access
 * - Minimal RAM footprint
 * - Platform-independent Wire usage
 */

#include "7Semi_TMP102.h"

/**
 * Constructor
 *
 * - Stores reference to I2C interface
 * - Allows alternate TwoWire buses
 *
 * Example:
 * - Wire       (Arduino default)
 * - Wire1      (ESP32 second I2C bus)
 */
TMP102_7Semi::TMP102_7Semi(TwoWire &wirePort)
{
    i2c = &wirePort;
}

/**
 * Initialize sensor and I2C bus
 *
 * Configuration steps:
 * - Store device address
 * - Start I2C bus
 * - Configure optional SDA/SCL pins (ESP platforms)
 * - Set I2C clock speed
 *
 * Parameters:
 * - i2cAddress
 *      - TMP102 address (default 0x48)
 *
 * - sda / scl
 *      - Optional custom pins (ESP32 / ESP8266)
 *      - Use 0xFF to keep default pins
 *
 * - i2cClockSpeed
 *      - Default: 400 kHz
 *      - TMP102 supports standard and fast mode
 */
bool TMP102_7Semi::begin(uint8_t i2cAddress,
                         uint8_t sda,
                         uint8_t scl,
                         uint32_t i2cClockSpeed)
{
    address = i2cAddress;

#if defined(ESP32) || defined(ESP8266)

    /**
     * ESP platforms allow dynamic I2C pins
     */
    if (sda != 0xFF && scl != 0xFF)
        i2c->begin(sda, scl);
    else
        i2c->begin();

#else

    /**
     * AVR boards use fixed hardware pins
     */
    i2c->begin();

#endif

    (void)sda;
    (void)scl;

    /**
     * Set I2C clock speed
     */
    i2c->setClock(i2cClockSpeed);

    i2c->beginTransmission(address);
    if (i2c->endTransmission(false) != 0)
        return false;

    return true;
}

/**
 * Read raw temperature register
 *
 * TMP102 temperature register layout:
 *
 * Normal mode (12-bit)
 *  MSB: T11 T10 ... T0 0000
 *
 * Extended mode (13-bit)
 *  MSB: T12 T11 ... T0 000
 *
 * Steps:
 * - Read temperature register
 * - Detect extended mode
 * - Align bits
 * - Return signed value
 */
bool TMP102_7Semi::readRawTemperature(int16_t &rawTemperature)
{
    uint16_t raw;

    if (!readReg(REG_TEMP, raw))
        return false;

    bool extended;

    if (!getExtendedMode(extended))
        return false;

    /**
     * Extract temperature bits
     */
    if (extended)
        rawTemperature = (int16_t)(raw >> 3) & 0x1FFF;
    else
        rawTemperature = (int16_t)(raw >> 4) & 0x0FFF;

    return true;
}

/**
 * Read temperature in Celsius
 *
 * Conversion formula:
 *
 * temperature = raw * 0.0625
 */
bool TMP102_7Semi::readTemperatureC(float &temperatureC)
{
    int16_t raw;

    if (!readRawTemperature(raw))
        return false;

    temperatureC = rawToCelsius(raw);

    return true;
}

/**
 * Read temperature in Fahrenheit
 *
 * Conversion:
 *
 * F = (C × 1.8) + 32
 */
bool TMP102_7Semi::readTemperatureF(float &temperatureF)
{
    float tempC;

    if (!readTemperatureC(tempC))
        return false;

    temperatureF = (tempC * 1.8f) + 32.0f;

    return true;
}

/**
 * Enable or disable shutdown mode
 *
 * Shutdown mode:
 * - Stops continuous temperature conversion
 * - Reduces power consumption
 *
 * Typical usage:
 * - Battery powered systems
 */
bool TMP102_7Semi::setShutdown(bool enable)
{
    return setBit(REG_CONFIG, TMP102_SD, enable);
}

/**
 * Start one-shot temperature conversion
 *
 * Requirements:
 * - Device must be in shutdown mode
 *
 * Operation:
 * - Triggers single temperature measurement
 */
bool TMP102_7Semi::startOneShot()
{
    return setBit(REG_CONFIG, TMP102_OS, true);
}

/**
 * Configure ALERT output behaviour
 *
 * ALERT modes:
 *
 * Comparator mode
 * - ALERT active while threshold exceeded
 *
 * Interrupt mode
 * - ALERT latched until temperature returns
 */
bool TMP102_7Semi::setAlert(bool interruptMode,
                            TMP102_ALERT_POLARITY polarity)
{
    if (!setBit(REG_CONFIG, TMP102_TM, interruptMode))
        return false;

    if (!setBit(REG_CONFIG, TMP102_POL, polarity))
        return false;

    return true;
}

/**
 * Read ALERT status bit
 */
bool TMP102_7Semi::getAlert(bool &alert)
{
    return getBit(REG_CONFIG, TMP102_AL, alert);
}

/**
 * Configure thermostat alert mode
 */
bool TMP102_7Semi::setThermAlertMode(TMP102_THERM_ALERT mode)
{
    return setBit(REG_CONFIG, TMP102_TM, mode);
}

bool TMP102_7Semi::getThermAlertMode(bool &mode)
{
    return getBit(REG_CONFIG, TMP102_TM, mode);
}

/**
 * Set ALERT polarity
 *
 * Options:
 * - Active LOW
 * - Active HIGH
 */
bool TMP102_7Semi::setAlertPolarity(TMP102_ALERT_POLARITY polarity)
{
    return setBit(REG_CONFIG, TMP102_POL, polarity);
}

bool TMP102_7Semi::getAlertPolarity(bool &polarity)
{
    return getBit(REG_CONFIG, TMP102_POL, polarity);
}

/**
 * Enable or disable extended temperature mode
 *
 * Extended mode:
 * - Enables 13-bit temperature resolution
 */
bool TMP102_7Semi::setExtendedMode(bool enable)
{
    return setBit(REG_CONFIG, TMP102_EM, enable);
}

bool TMP102_7Semi::getExtendedMode(bool &enabled)
{
    return getBit(REG_CONFIG, TMP102_EM, enabled);
}

/**
 * Set temperature conversion rate
 *
 * Options:
 * - 0.25 Hz
 * - 1 Hz
 * - 4 Hz
 * - 8 Hz
 */
bool TMP102_7Semi::setConversionRate(TMP102_CONVERSION_RATE rate)
{
    return setBits(REG_CONFIG, TMP102_CR0, 2, rate);
}

bool TMP102_7Semi::getConversionRate(uint8_t &rate)
{
    return getBits(REG_CONFIG, TMP102_CR0, 2, rate);
}

/**
 * Configure fault queue
 *
 * Fault queue:
 * - Number of consecutive faults required
 *   before ALERT is triggered
 */
bool TMP102_7Semi::setFaultQueue(TMP102_FAULT_QUEUE faults)
{
    return setBits(REG_CONFIG, TMP102_F0, 2, faults);
}

bool TMP102_7Semi::getFaultQueue(uint8_t &faults)
{
    return getBits(REG_CONFIG, TMP102_F0, 2, faults);
}

/**
 * Set high temperature alert threshold
 */
bool TMP102_7Semi::setHighLimit(float tempC)
{
    uint16_t raw = celsiusToRaw(tempC) << 4;
    return writeReg(REG_T_HIGH, raw);
}

/**
 * Set low temperature alert threshold
 */
bool TMP102_7Semi::setLowLimit(float tempC)
{
    return writeReg(REG_T_LOW, celsiusToRaw(tempC) << 4);
}

/**
 * Read high temperature threshold
 */
bool TMP102_7Semi::getHighLimit(float &tempC)
{
    uint16_t raw;

    if (!readReg(REG_T_HIGH, raw))
        return false;
    tempC = rawToCelsius((int16_t)(raw >> 4));

    return true;
}

/**
 * Read low temperature threshold
 */
bool TMP102_7Semi::getLowLimit(float &tempC)
{
    uint16_t raw;

    if (!readReg(REG_T_LOW, raw))
        return false;

    tempC = rawToCelsius((int16_t)(raw >> 4));

    return true;
}

/**
 * Read 16-bit register
 *
 * Steps:
 * - Write register pointer
 * - Perform repeated start
 * - Read two bytes
 */
bool TMP102_7Semi::readReg(uint8_t reg, uint16_t &value)
{
    i2c->beginTransmission(address);
    i2c->write(reg);

    if (i2c->endTransmission(false) != 0)
        return false;

    if (i2c->requestFrom(address, (uint8_t)2) != 2)
        return false;

    value = ((uint16_t)i2c->read() << 8) | i2c->read();

    return true;
}

/**
 * Write 16-bit register
 */
bool TMP102_7Semi::writeReg(uint8_t reg, uint16_t value)
{
    i2c->beginTransmission(address);

    i2c->write(reg);
    i2c->write(value >> 8);
    i2c->write(value & 0xFF);

    return (i2c->endTransmission() == 0);
}

/**
 * Modify multi-bit field
 */
bool TMP102_7Semi::setBits(uint8_t reg,
                           uint8_t position,
                           uint8_t number_of_bits,
                           uint8_t value)
{
    uint16_t v;

    if (!readReg(reg, v))
        return false;

    uint16_t mask = (1 << number_of_bits) - 1;

    v &= ~(mask << position);
    v |= ((value & mask) << position);

    return writeReg(reg, v);
}

/**
 * Read multi-bit field
 */
bool TMP102_7Semi::getBits(uint8_t reg,
                           uint8_t position,
                           uint8_t number_of_bits,
                           uint8_t &value)
{
    uint16_t v;

    if (!readReg(reg, v))
        return false;

    value = (v >> position) & ((1 << number_of_bits) - 1);

    return true;
}

/**
 * Set single configuration bit
 */
bool TMP102_7Semi::setBit(uint8_t reg,
                          uint8_t position,
                          bool value)
{
    return setBits(reg, position, 1, value);
}

/**
 * Read single configuration bit
 */
bool TMP102_7Semi::getBit(uint8_t reg,
                          uint8_t position,
                          bool &value)
{
    uint8_t v;

    if (!getBits(reg, position, 1, v))
        return false;

    value = v;

    return true;
}

/**
 * Convert raw temperature value to Celsius
 *
 * TMP102 temperature encoding:
 * - Two's complement format
 * - Sign bit located in MSB
 *
 * Sign extension is required
 * for negative temperatures.
 */
float TMP102_7Semi::rawToCelsius(int16_t raw)
{
    if (raw & 0x800)
        raw |= 0xF000;

    return raw * 0.0625f;
}

/**
 * Convert Celsius value to raw register format
 */
int16_t TMP102_7Semi::celsiusToRaw(float temp)
{
    return (int16_t)(temp / 0.0625f);
}