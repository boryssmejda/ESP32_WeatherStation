#include "WeatherStation.h"
#include "BatteryVoltage.h"

WeatherStation::WeatherStation()
                        : _pms{PMSx003, Serial2}
{
}

void WeatherStation::init()
{
    _pms.init();
    _bh1750.init();
    _bme280.init();
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

    weatherCond.batteryVoltage = battery::BatteryVoltage::measureVoltage();

    return weatherCond;
}

void WeatherStation::print()
{
    auto conditions = requestWeatherConditions();

    char weatherConditions[1024];

    sprintf(weatherConditions, "Temp: %.2f || Pressure: %.2f || Hum: %.2f", conditions.temperature, conditions.pressure, conditions.airHumidity);
    sprintf(weatherConditions, "%s || PM01: %u || PM25: %u || PM10: %u", weatherConditions, conditions.pm01, conditions.pm25, conditions.pm10);
    sprintf(weatherConditions, "%s || Lum: %u || Battery Voltage: %.2f", weatherConditions, conditions.luminosity, conditions.batteryVoltage);

    Serial.println(weatherConditions);
}