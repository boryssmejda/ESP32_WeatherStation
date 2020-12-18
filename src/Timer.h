#pragma once

#include <chrono>

class Timer
{
    std::chrono::time_point<std::chrono::system_clock> _start;
    const std::chrono::seconds _timeToSleep;
    constexpr static unsigned int uS_TO_S_FACTOR = 1000000;

    public:
        Timer(std::chrono::seconds timeToSleep);
        ~Timer();
};
