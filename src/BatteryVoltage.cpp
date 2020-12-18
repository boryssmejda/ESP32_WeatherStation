#include "BatteryVoltage.h"
#include <Arduino.h>

using namespace battery;

float battery::BatteryVoltage::measureVoltage()
{
    float adcResistorVoltage = analogRead(BATTERY_VOLTAGE_PIN);

    float resistorVoltageInVolts = adcResistorVoltage * 3.3f / 4095.0f;

    return resistorVoltageInVolts * (1 + RESISTOR_R1/static_cast<float>(RESISTOR_R2));
}