#include "WeatherStation.h"

WeatherStation::WeatherStation(TwoWire *i2c_handle, uint8_t supplyPin, uint8_t analogPin,
                        uint16_t measurementDelay)
                        : _bme280{i2c_handle}
                        , _bh1750{i2c_handle}
                        , _soilHumSensor{supplyPin, analogPin, measurementDelay}
{
    assert(i2c_handle);
}

MeasuredData WeatherStation::requestData()
{
    MeasuredData measuredData;
    measuredData.temperature = _bme280.readTemperature();
    measuredData.pressure = _bme280.readPressure();
    measuredData.airHumidity = _bme280.readHumidity();
    measuredData.soilHumidity = _soilHumSensor.readSoilHumidity();
    measuredData.luminosity = _bh1750.measureLuminosity();

    return measuredData;
}