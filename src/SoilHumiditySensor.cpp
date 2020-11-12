#include "SoilHumiditySensor.h"
#include <Arduino.h>

uint16_t SoilHumiditySensor::readADCValue() const
{
    digitalWrite(_supplyPin, HIGH);
    delay(5);
    uint16_t soilAnalogVoltage = analogRead(_analogPin);
    digitalWrite(_supplyPin, LOW);

    return soilAnalogVoltage;
}

SoilHumiditySensor::SoilHumiditySensor(uint8_t supplyPin, uint8_t analogPin, uint16_t measurementDelay)
    : _supplyPin{supplyPin}
    , _analogPin{analogPin}
    , _measurementDelay{measurementDelay}
{
     pinMode(_supplyPin, OUTPUT);
}

float SoilHumiditySensor::readSoilHumidity()
{
    uint16_t adcValue = readADCValue();
    uint16_t soilHumidity = map(adcValue, 0, MAX_MEASURED_SOIL_HUMIDITY, 0, 100);

    return soilHumidity;
}

void SoilHumiditySensor::printSoilHumidity()
{
    uint16_t soilHumidity = readSoilHumidity();
    char soilBuffer[25];

    snprintf(soilBuffer, sizeof(soilBuffer), "Soil Hum: %d%%\r\n", soilHumidity);
    Serial.print(soilBuffer);
}
