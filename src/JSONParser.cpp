#include "JSONParser.h"


JsonParser::JsonParser(char* timestamp, size_t timestampSize, const WeatherConditions& weatherConditions)
    : _weatherCondtions{weatherConditions}
    , _timestamp{timestamp}
    , _timestampSize{timestampSize}
{
    assert(timestamp);
    assert(timestampSize > 0);
}

void JsonParser::createJsonObject(const JsonArray& measuredParameter, const MeasuredQuantity& measuredQuantity)
{
    JsonObject jo = measuredParameter.createNestedObject();
    jo["value"] = measuredQuantity.value;
    jo["unit"] = measuredQuantity.unit;
    jo["sensor"] = measuredQuantity.sensor;
    jo["quantity"] = measuredQuantity.quantity;
}

void JsonParser::createFile(char jsonSerializedOutput[1024])
{
    StaticJsonDocument<1024> doc;

    doc["timestamp"] = _timestamp;
    JsonArray values = doc.createNestedArray("values");

    createJsonObject(values, {_weatherCondtions.temperature, "*C", "BME280", "Temperature"});
    createJsonObject(values, {_weatherCondtions.pressure, "hPa", "BME280", "Pressure"});
    createJsonObject(values, {_weatherCondtions.airHumidity, "%", "BME280", "Humidity"});
    createJsonObject(values, {_weatherCondtions.luminosity, "lx", "BH1750", "Luminosity"});

    createJsonObject(values, {_weatherCondtions.pm01, "ug/m3", "PMSA003", "PM1.0"});
    createJsonObject(values, {_weatherCondtions.pm25, "ug/m3", "PMSA003", "PM2.5"});
    createJsonObject(values, {_weatherCondtions.pm10 , "ug/m3", "PMSA003", "PM10"});

    serializeJson(doc, jsonSerializedOutput, 1024);
}

void JsonParser::mergeWeatherConditionsWithHeader(char weatherConditions[1024])
{
    StaticJsonDocument<1024> doc;
    doc["apiKeyValue"] = apiKeyValue;
    doc["location"] = sensorLocation;

    StaticJsonDocument<1024> json_weatherConditions;

    assert(deserializeJson(json_weatherConditions, weatherConditions) == DeserializationError::Ok);

    doc["weatherConditions"] = json_weatherConditions;

    serializeJson(doc, weatherConditions, 1024);

    Serial.print(weatherConditions);
}
