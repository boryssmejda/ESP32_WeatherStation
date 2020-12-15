#include "WeatherStation.h"

WeatherStation::WeatherStation(TwoWire *i2c_handle)
                        : _bme280{i2c_handle}
                        , _bh1750{i2c_handle}
                        , _pms{PMSx003, Serial2}
{
    assert(i2c_handle);
    _pms.init();
}

WeatherConditions WeatherStation::requestWeatherConditions()
{
    WeatherConditions weatherCond;
    weatherCond.temperature = _bme280.readTemperature();
    weatherCond.pressure = _bme280.readPressure();
    weatherCond.airHumidity = _bme280.readHumidity();
    weatherCond.luminosity = _bh1750.measureLuminosity();

    _pms.read();
    weatherCond.pm01 = _pms.pm01;
    weatherCond.pm25 = _pms.pm25;
    weatherCond.pm10 = _pms.pm10;

    return weatherCond;
}