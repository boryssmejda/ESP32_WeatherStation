#include <WiFi.h>
#include <HTTPClient.h>


#include "WeatherStation.h"
#include "Timestamp.h"
#include "SDCard.h"
#include "JSONParser.h"
#include <array>


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */


// ===============================================================

const char* ssid     = "FunBox2-F407";
const char* password = "1CEE231DEF373D27744763AEFF";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://studenci.zts.p.lodz.pl/stud_005/praca/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "BME280";
String sensorLocation = "Garden";

WeatherConditions printWeatherConditions();
void parse_json(const JsonArray& data, float value, const char* unit, const char* sensor, const char* quantity);
void post_http_json_file(const std::string& serializedJson);
std::string create_json_file();

Timestamp timestamp;
SDCard mySD;
WeatherStation weatherStation;

void setup()
{
    Serial.begin(115200);
    while(!Serial);

    Wire.begin();
    Serial2.begin(9600);

    Serial.flush();
    Serial2.flush();

    mySD.init();
    weatherStation.init();
}

void loop()
{
    Serial.println("\r\n--------------------------------");

    char jsonSerializedOutput[1024];
    char timestampBuffer[64];

    timestamp.getTimestamp(timestampBuffer, sizeof(timestampBuffer));

    JsonParser jsonParser(timestampBuffer, sizeof(timestampBuffer), weatherStation.requestWeatherConditions());
    jsonParser.createFile(jsonSerializedOutput);

    Serial.println(jsonSerializedOutput);

    mySD.append(jsonSerializedOutput);

    Serial.println("===============\r\n");


    delay(10000);
}


void parse_json(const JsonArray& data, float value, const char* unit, const char* sensor, const char* quantity)
{
    JsonObject jo = data.createNestedObject();
    jo["value"] = value;
    jo["unit"] = unit;
    jo["sensor"] = sensor;
    jo["quantity"] = quantity;
}


void post_http_json_file(const std::string& serializedJson)
{
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    // Specify content-type header
    http.addHeader("Content-Type", "application/json");
    Serial.print("httpRequestData: ");
    Serial.println(serializedJson.c_str());
    // Send HTTP POST request
    int httpResponseCode = http.POST(serializedJson.c_str());

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
}

std::string create_json_file()
{
    StaticJsonDocument<1024> doc;
    char serializedJson[1024];
    doc["apiKeyValue"] = "tPmAT5Ab3j7F9";
    doc["location"] = "Garden";

    JsonArray data = doc.createNestedArray("data");

    //parse_json(data, bme.readTemperature(), "*C", "BME280", "Temperature");
    //parse_json(data, bme.readPressure() / 100.0, "hPa", "BME280", "Pressure");
    //parse_json(data, bme.readHumidity(), "%", "BME280", "Humidity");
    //parse_json(data, lightMeter.readLightLevel(), "lx", "BH1750", "Luminosity");

    //parse_json(data, pms.pm01, "ug/m3", "PMSA003", "PM1.0");
    ///parse_json(data, pms.pm25, "ug/m3", "PMSA003", "PM2.5")//;
    //parse_json(data, pms.pm10, "ug/m3", "PMSA003", "PM10");

    serializeJson(doc, serializedJson);

    return serializedJson;
}


//esp_sleep_wakeup_cause_t print_wakeup_reason(){
//    esp_sleep_wakeup_cause_t wakeup_reason;
//
//    wakeup_reason = esp_sleep_get_wakeup_cause();
//
//    switch(wakeup_reason)
//    {
//        case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
//        case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
//        case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
//        case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
//        case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
//        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
//    }
//
//    return wakeup_reason;
//}
//
