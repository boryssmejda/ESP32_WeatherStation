#include "WifiWrapper.h"
#include <WiFi.h>

bool WifiWrapper::isConnected = false;

void WifiWrapper::connect()
{
    WiFi.begin(WifiWrapper::_ssid, WifiWrapper::_password);
    Serial.println("Connecting");

    for(int i = 0; i < 10; ++i)
    {
        if(WiFi.status() == WL_CONNECTED)
        {
            Serial.println("Connected!");
            Serial.println("");
            Serial.print("Connected to WiFi network with IP Address: ");
            Serial.println(WiFi.localIP());
            isConnected = true;
            break;
        }
        else
        {
            Serial.print(".");
            delay(500);
        }
    }
}

bool WifiWrapper::isWifiConnected()
{
    return isConnected;
}

void WifiWrapper::disconnect()
{
    WiFi.disconnect();
}

void WifiWrapper::turnOff()
{
    WiFi.mode(WIFI_OFF);
}