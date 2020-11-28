#include <WiFi.h>
#include <HTTPClient.h>

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

//void printValues();

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

        // Specify content-type header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        // Prepare your HTTP POST request data
        String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                          + "&location=" + sensorLocation + "&value1=" + String(bme.readTemperature())
                          + "&value2=" + String(bme.readHumidity()) + "&value3=" + String(bme.readPressure()/100.0F) + "";
        Serial.print("httpRequestData: ");
        Serial.println(httpRequestData);

        // You can comment the httpRequestData variable above
        // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
        //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);

        // If you need an HTTP request with a content type: text/plain
        //http.addHeader("Content-Type", "text/plain");
        //int httpResponseCode = http.POST("Hello, World!");

        // If you need an HTTP request with a content type: application/json, use the following:
        //http.addHeader("Content-Type", "application/json");
        //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

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
//
//void printValues()
//{
//    char temperatureBuff[25];
//    char pressureBuff[25];
//    char humidityBuff[25];
//    char soilHumBuf[25];
//    char luminosityBuff[25];
//
//    snprintf(temperatureBuff, sizeof(temperatureBuff), "Temp: %.2f*C\r\n", bme.readTemperature());
//    Serial.print(temperatureBuff);
//
//    snprintf(pressureBuff, sizeof(pressureBuff), "Pressure: %.2f hPa\r\n", bme.readPressure());
//    Serial.print(pressureBuff);
//
//    snprintf(humidityBuff, sizeof(humidityBuff), "Air Hum: %.2f%%\r\n", bme.readHumidity()/ 100.0f);
//    Serial.print(humidityBuff);
//
//    snprintf(luminosityBuff, sizeof(luminosityBuff), "Lum: %ulx\r\n", (uint16_t)lightMeter.readLightLevel());
//    Serial.print(luminosityBuff);
//
//    pms.read();
//    char airQualityBuffer[50];
//    snprintf(airQualityBuffer, 50, "PM1.0 %u | PM2.5 %u | PM10 %u [ug/m3]\r\n", pms.pm01, pms.pm25, pms.pm10);
//    Serial.println(airQualityBuffer);
//    Serial.println("==================\n");
//}
//
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