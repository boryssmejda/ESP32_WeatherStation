#include "WifiWrapper.h"
#include <WiFi.h>


WifiWrapper::ConnectionStatus WifiWrapper::connect()
{
    WiFi.begin(_ssid, _password);
    return try_connecting(10, 500);
}

WifiWrapper::ConnectionStatus WifiWrapper::try_connecting(int numOfTrials, int delayInMs)
{
    for(int i = 0; i < numOfTrials; ++i)
    {
        if(isWifiConnected())
        {
            return ConnectionStatus::SUCCESS;
        }
        else
        {
            delay(delayInMs);
        }
    }

    return ConnectionStatus::FAILURE;
}

bool WifiWrapper::isWifiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void WifiWrapper::disconnect()
{
    WiFi.disconnect();
}

void WifiWrapper::turnOff()
{
    WiFi.mode(WIFI_OFF);
}