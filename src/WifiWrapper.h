#pragma once

class WifiWrapper
{
    constexpr static char const* _ssid     = "FunBox2-F407";
    constexpr static char const* _password = "1CEE231DEF373D27744763AEFF";

    bool isWifiConnected();

    public:
        enum class ConnectionStatus
        {
            SUCCESS,
            FAILURE
        };

        WifiWrapper::ConnectionStatus connect();
        void disconnect();
        void turnOff();

    private:
        WifiWrapper::ConnectionStatus try_connecting(int numOfTrials, int delayInMs);
};