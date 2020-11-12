#include "BH1750_wrapper.h"

BH1750_wrapper::BH1750_wrapper(TwoWire *i2c_handle)
    :_lightMeter{DEFAULT_ADDRESS}
    {
        assert(i2c_handle != nullptr);
        assert(_lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE, DEFAULT_ADDRESS, i2c_handle) != false);
    }

uint16_t BH1750_wrapper::measureLuminosity()
{
    _lightMeter.readLightLevel();
    delay(200);
    _lightMeter.configure(BH1750::ONE_TIME_HIGH_RES_MODE);
    return _lightMeter.readLightLevel();
}

void BH1750_wrapper::printLuminosity()
{
    uint16_t lux = measureLuminosity();

    char buffer[20];

    snprintf(buffer, sizeof(buffer), "Light: %dlx", lux);
    Serial.println(buffer);
}
