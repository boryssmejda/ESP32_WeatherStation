#include "Timer.h"
#include <Arduino.h>

Timer::Timer(std::chrono::seconds timeToSleep)
    : _timeToSleep{timeToSleep}
{
    _start = std::chrono::system_clock::now();
}

Timer::~Timer()
{
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - _start;

    auto delta_time = _timeToSleep.count() - diff.count();

    esp_sleep_enable_timer_wakeup(delta_time * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}