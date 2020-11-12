#pragma once

#include <cstdint>

class SoilHumiditySensor
{
    private:
        uint8_t _supplyPin;
        uint8_t _analogPin;
        uint16_t _measurementDelay;
        constexpr static uint16_t MAX_MEASURED_SOIL_HUMIDITY = 2300; // measured by placing sensor into glass of water
        uint16_t readADCValue() const;
    public:
        SoilHumiditySensor(uint8_t supplyPin, uint8_t analogPin, uint16_t measurementDelay = 5);
        uint16_t readSoilHumidity();
        void printSoilHumidity();
};