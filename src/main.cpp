#include <WiFi.h>
#include <HTTPClient.h>

#include <ArduinoJson.h>
#include <Wire.h>

#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <PMserial.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */

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
void format_http_post(char* output);

void setup()
{
    Serial.begin(115200);
    while(!Serial);

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Starting measurements!");
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
}

void loop()
{
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(serverName);

        pms.read();
        char output[1024];
        StaticJsonDocument<1024> doc;

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

        serializeJson(doc, output);

        // Specify content-type header
        http.addHeader("Content-Type", "application/json");

        Serial.print("httpRequestData: ");
        Serial.println(output);

        // Send HTTP POST request
        int httpResponseCode = http.POST(output);

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
    else
    {
        Serial.println("WiFi Disconnected");
    }

    //Send an HTTP POST request every 30 seconds
    delay(30000);
}

void parse_json(const JsonArray& data, float value, const char* unit, const char* sensor, const char* quantity)
{
    JsonObject jo = data.createNestedObject();
    jo["value"] = value;
    jo["unit"] = unit;
    jo["sensor"] = sensor;
    jo["quantity"] = quantity;
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
