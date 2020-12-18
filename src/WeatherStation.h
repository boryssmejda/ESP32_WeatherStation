#pragma once

#include "BME280_wrapper.h"
#include "BH1750_wrapper.h"
#include <PMserial.h>

struct WeatherConditions
{
    float temperature;
    float pressure;
    float airHumidity;
    uint16_t luminosity;
    uint16_t pm01;
    uint16_t pm25;
    uint16_t pm10;
    float batteryVoltage;
};

class WeatherStation
{
    private:
        BME280_wrapper _bme280;
        BH1750_wrapper _bh1750;
        SerialPM _pms;

    public:
        WeatherStation();
        void init();
        WeatherConditions requestWeatherConditions();
        void print();
};
