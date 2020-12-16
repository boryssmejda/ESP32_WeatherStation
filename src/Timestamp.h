#pragma once
#include "time.h"
#include "RTClib.h"

class Timestamp
{
        void formatTime(const struct tm& currentTime, char formattedTime[], int size);
    public:
        void getTimestamp(char timestamp[], int timestampSize);
};

class BoardTimeKeeper
{
    private:
        constexpr static char const* _ntpServer = "pool.ntp.org";
        constexpr static long  _gmtOffset_sec = 3600;
        constexpr static int _daylightOffset_sec = 3600;
        RTC_DS3231 _rtc;

        struct tm getTimeFromRTC();
        struct tm getTimeFromNTP();
        void updateRTCTimeWithNTP(struct tm ntpTime);
        int adjustYear(int year);
    public:
        BoardTimeKeeper();
        struct tm getCurrentTime();
};