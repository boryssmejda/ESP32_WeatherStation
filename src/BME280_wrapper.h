#pragma once

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class BME280_wrapper
{
    private:
        constexpr static int DEFAULT_ADDRESS = 0x76;
        Adafruit_BME280 _bme;

    public:
        void init();
        float readTemperature();
        float readPressure();
        float readHumidity();

        void printAll();
};
