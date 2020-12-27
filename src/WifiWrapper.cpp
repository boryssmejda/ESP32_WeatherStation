#include "WifiWrapper.h"
#include <WiFi.h>
#include "SDCard.h"

bool WifiWrapper::isConnected = false;

void WifiWrapper::connect()
{
    SDCard::logToFile("Trying to connect!");
    SDCard::logToFile(WifiWrapper::_ssid);
    SDCard::logToFile(WifiWrapper::_password);

    WiFi.begin(WifiWrapper::_ssid, WifiWrapper::_password);
    SDCard::logToFile("After wifi Begin!");
    Serial.println("Connecting");

    for(int i = 0; i < 10; ++i)
    {
        if(WiFi.status() == WL_CONNECTED)
        {
            SDCard::logToFile("Connected to WIFI!");
            Serial.println("Connected!");
            Serial.println("");
            Serial.print("Connected to WiFi network with IP Address: ");
            Serial.println(WiFi.localIP());
            isConnected = true;
            break;
        }
        else
        {
            SDCard::logToFile(".");
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