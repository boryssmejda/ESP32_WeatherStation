#pragma once

using httpRequestCode = int;

class HttpRequest
{
    private:
        void prepareJson(char weatherCondtions[1024]);
        httpRequestCode sendJsonThroughHttp(char weatherCondtions[1024]);
    public:
        constexpr static const char* serverName = "https://studenci.zts.p.lodz.pl/stud_005/praca/post-esp-data.php";

        static httpRequestCode sendWeatherConditions(char weatherConditions[1024]);
};