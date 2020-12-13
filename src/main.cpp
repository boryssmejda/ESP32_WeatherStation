#include <WiFi.h>
#include <HTTPClient.h>

#include <ArduinoJson.h>
#include <Wire.h>

#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <PMserial.h>

#include "time.h"
#include "RTClib.h"


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */


RTC_DS3231 rtc;
TwoWire I2CBME = TwoWire(0);
const int soilHumAnalogPin = 34;
const int soilHumSupplyPin = 33;
const int maxValueInWater = 2300;
unsigned long delayTime;
const int rainPin = 4;
constexpr int rainDigPin = 15;

constexpr uint8_t BME280_DEFAULT_ADDRESS = 0x76;
constexpr uint8_t BH1750_DEFAULT_ADDRESS = 0x23;
// ================= Sensor Handlers =============================
Adafruit_BME280 bme;
BH1750 lightMeter;
SerialPM pms(PMSx003, Serial2);

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

void parse_json(const JsonArray& data, float value, const char* unit, const char* sensor, const char* quantity);
void post_http_json_file(const std::string& serializedJson);
std::string create_json_file();
void configure_rtc();
void print_current_datetime();

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


void setup()
{
    Serial.begin(115200);
    while(!Serial);

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println(".");
    }

    Wire.begin();
    Serial2.begin(9600);

    Serial.println("Starting assertion!");
    assert(bme.begin(BME280_DEFAULT_ADDRESS, &Wire) != false);
    Serial.println("Asserted BME280");
    assert(lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE, BH1750_DEFAULT_ADDRESS, &Wire) != false);
    pms.init();
    Serial.println("Asserted BH1750. STARTING!!");

    Serial.flush();
    Serial2.flush();

    //init and get the time
    //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    //printLocalTime();

    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    //configure_rtc();
}

void loop()
{
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {
        pms.read();

        auto serializedJson = create_json_file();

        post_http_json_file(serializedJson);
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }

    //Send an HTTP POST request every 30 seconds
    delay(1000);
    //printLocalTime();
    print_current_datetime();
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

    parse_json(data, bme.readTemperature(), "*C", "BME280", "Temperature");
    parse_json(data, bme.readPressure() / 100.0, "hPa", "BME280", "Pressure");
    parse_json(data, bme.readHumidity(), "%", "BME280", "Humidity");
    parse_json(data, lightMeter.readLightLevel(), "lx", "BH1750", "Luminosity");

    parse_json(data, pms.pm01, "ug/m3", "PMSA003", "PM1.0");
    parse_json(data, pms.pm25, "ug/m3", "PMSA003", "PM2.5");
    parse_json(data, pms.pm10, "ug/m3", "PMSA003", "PM10");

    serializeJson(doc, serializedJson);

    return serializedJson;
}

void configure_rtc()
{
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        abort();
    }

    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    rtc.adjust(DateTime{timeinfo.tm_year - 100, timeinfo.tm_mon +1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec});

    //if (rtc.lostPower()) {
    //    Serial.println("RTC lost power, let's set the time!");
    //    // When time needs to be set on a new device, or after a power loss, the
    //    // following line sets the RTC to the date & time this sketch was compiled
    //    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //    // This line sets the RTC with an explicit date & time, for example to set
    //    // January 21, 2014 at 3am you would call:
    //    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //}
}


void print_current_datetime()
{
    char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
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
