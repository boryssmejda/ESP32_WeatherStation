#include "Timestamp.h"
#include "WifiWrapper.h"

void Timestamp::getTimestamp(char timestamp[], int timestampSize)
{
    static BoardTimeKeeper timeKeeper;
    auto currentTime = timeKeeper.getCurrentTime();

    formatTime(currentTime, timestamp, timestampSize);
}

void Timestamp::formatTime(const struct tm& currentTime, char formattedTime[], int size)
{
    strftime(formattedTime, size, "%d/%m/%Y %H:%M:%S", &currentTime);
}

BoardTimeKeeper::BoardTimeKeeper()
{
    assert(_rtc.begin() == true);
}

struct tm BoardTimeKeeper::getCurrentTime()
{
    WifiWrapper wifiWrapper;
    if(wifiWrapper.connect() != WifiWrapper::ConnectionStatus::SUCCESS)
    {
        Serial.println("From RTC!");
        return getTimeFromRTC();
    }
    else
    {
        Serial.println("From NTP");
        auto ntpTime = getTimeFromNTP();
        updateRTCTimeWithNTP(ntpTime);

        return ntpTime;
    }
}

struct tm BoardTimeKeeper::getTimeFromRTC()
{
    auto now = _rtc.now();

    struct tm rtcTime;

    rtcTime.tm_year = now.year();
    rtcTime.tm_mon = now.month();
    rtcTime.tm_mday = now.day();

    rtcTime.tm_hour = now.hour();
    rtcTime.tm_min = now.minute();
    rtcTime.tm_sec = now.second();

    rtcTime.tm_year = adjustYear(rtcTime.tm_year);

    return rtcTime;
}

int BoardTimeKeeper::adjustYear(int year)
{
    return year - 100 - 1900;
}

struct tm BoardTimeKeeper::getTimeFromNTP()
{
    configTime(_gmtOffset_sec, _daylightOffset_sec, _ntpServer);

    struct tm timeinfo;
    getLocalTime(&timeinfo);

    return timeinfo;
}

void BoardTimeKeeper::updateRTCTimeWithNTP(struct tm ntpTime)
{
    auto y = static_cast<uint16_t>(ntpTime.tm_year);
    auto mon = static_cast<uint8_t>(ntpTime.tm_mon);
    auto day = static_cast<uint8_t>(ntpTime.tm_mday);

    auto h = static_cast<uint8_t>(ntpTime.tm_hour);
    auto min = static_cast<uint8_t>(ntpTime.tm_min);
    auto sec = static_cast<uint8_t>(ntpTime.tm_sec);

    _rtc.adjust(DateTime{y, mon, day, h, min, sec});
}