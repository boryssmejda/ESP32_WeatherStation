#pragma once

#define ARDUINOJSON_ENABLE_STD_STREAM 1
#include <ArduinoJson.h>
#include "WeatherStation.h"
#include "Timestamp.h"
#include <array>

struct MeasuredQuantity
{
    float value;
    const char* unit;
    const char* sensor;
    const char* quantity;
};

class JsonParser
{
    JsonArray _measuredQuantity;
    WeatherConditions _weatherCondtions;
    char* _timestamp;
    size_t _timestampSize;

    public:
        JsonParser(char* timestamp, size_t timestampSize, const WeatherConditions& weatherConditions);
        void createJsonObject(const JsonArray& measuredParameter, const MeasuredQuantity& measuredQuantity);
        void createFile(char jsonSerializedOutput[1024]);
};
