#include "HttpRequest.h"
#include "WifiWrapper.h"

#include <HTTPClient.h>

httpRequestCode HttpRequest::sendWeatherConditions(char weatherConditions[1024])
{
    Serial.println("TO BE SENT: ");
    Serial.println(weatherConditions);

    WifiWrapper wifiConnection;
    wifiConnection.connect();

    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(HttpRequest::serverName);
    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(weatherConditions);

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return httpResponseCode;
}


bool HttpRequest::isRequestSuccessfull(httpRequestCode requestReturnCode)
{
    return (requestReturnCode >= 200 && requestReturnCode <= 299);
}