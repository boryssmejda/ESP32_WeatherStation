#include "BME280_wrapper.h"

BME280_wrapper::BME280_wrapper(TwoWire *i2c_handle)
{
    assert(i2c_handle);
    assert(_bme.begin(DEFAULT_ADDRESS, i2c_handle) != false);
}

float BME280_wrapper::readTemperature()
{
    return _bme.readTemperature();
}

float BME280_wrapper::readPressure()
{
    float pascalPressure = _bme.readPressure();
    float hectoPascalPressure = pascalPressure / 100.0f;

    return hectoPascalPressure;
}

float BME280_wrapper::readHumidity()
{
    return _bme.readHumidity();
}

void BME280_wrapper::printAll()
{
    float t = readTemperature();
    float p = readPressure();
    float h = readHumidity();

    char tempBuff[25];
    char pressureBuff[25];
    char humidityBuff[25];
    char totalBuffer[75];

    snprintf(tempBuff, sizeof(tempBuff), "Temp: %.2f*C\r\n", t);
    snprintf(pressureBuff, sizeof(pressureBuff), "Pressure: %.2f hPa\r\n", p);
    snprintf(humidityBuff, sizeof(humidityBuff), "Humidity: %.2f%%\r\n", h);
    snprintf(totalBuffer, sizeof(totalBuffer), "%s%s%s", tempBuff, pressureBuff, humidityBuff);

    Serial.println(totalBuffer);
}
