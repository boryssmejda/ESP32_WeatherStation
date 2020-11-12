#pragma once

#include <Wire.h>
#include <BH1750.h>

class BH1750_wrapper
{
    private:
        constexpr static int DEFAULT_ADDRESS = 0x23;
        BH1750 _lightMeter;
    public:
        BH1750_wrapper(TwoWire *i2c_handle);
        uint16_t measureLuminosity();
        void printLuminosity();
};
