#pragma once

class WifiWrapper
{
    static bool isConnected;

    public:
        enum class ConnectionStatus
        {
            SUCCESS,
            FAILURE
        };

        constexpr static char const* _ssid     = "FunBox2-F407";
        constexpr static char const* _password = "1CEE231DEF373D27744763AEFF";

        static void connect();
        static bool isWifiConnected();
        void disconnect();
        void turnOff();
};