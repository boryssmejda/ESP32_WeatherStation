#pragma once


namespace battery
{
// KiloOhm
constexpr unsigned long long int operator"" _kOhm(unsigned long long int resistance)
{
    return resistance*1000;
}

// Ohm
constexpr unsigned long long int operator"" _Ohm(unsigned long long int resistance)
{
    return resistance;
}

class BatteryVoltage
{
    private:
        constexpr static int BATTERY_VOLTAGE_PIN = 33;
        constexpr static int RESISTOR_R1 = 100_kOhm;
        constexpr static int RESISTOR_R2 = 100_kOhm;

    public:
        static float measureVoltage();
};
} // battery
