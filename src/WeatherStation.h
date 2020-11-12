#pragma once

#include "BME280_wrapper.h"
#include "BH1750_wrapper.h"
#include "SoilHumiditySensor.h"

struct MeasuredData
{
    float temperature;
    float pressure;
    float airHumidity;
    uint16_t soilHumidity;
    uint16_t luminosity;
};

class WeatherStation
{
    private:
        BME280_wrapper _bme280;
        BH1750_wrapper _bh1750;
        SoilHumiditySensor _soilHumSensor;

    public:
        WeatherStation(TwoWire *i2c_handle, uint8_t _supplyPin, uint8_t _analogPin,
                        uint16_t _measurementDelay = 5);
        MeasuredData requestData();
};
