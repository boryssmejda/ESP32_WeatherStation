/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>

#include "WeatherStation.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */


void printValues(MeasuredData& data);
void print_wakeup_reason();

TwoWire I2CBME = TwoWire(0);
const int soilHumAnalogPin = 34;
const int soilHumSupplyPin = 33;
const int maxValueInWater = 2300;
unsigned long delayTime;

void setup()
{
    Serial.begin(115200);
    while(!Serial);

    print_wakeup_reason();

    Wire.begin();

    WeatherStation weatherStation(&Wire, soilHumSupplyPin, soilHumAnalogPin);
    auto measuredData = weatherStation.requestData();

    printValues(measuredData);

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    Serial.flush();
    esp_deep_sleep_start();
}

void loop()
{
}

void printValues(MeasuredData& data)
{
    char temperatureBuff[25];
    char pressureBuff[25];
    char humidityBuff[25];
    char soilHumBuf[25];
    char luminosityBuff[25];

    snprintf(temperatureBuff, sizeof(temperatureBuff), "Temp: %.2f*C\r\n", data.temperature);
    Serial.print(temperatureBuff);

    snprintf(pressureBuff, sizeof(pressureBuff), "Pressure: %.2f hPa\r\n", data.pressure);
    Serial.print(pressureBuff);

    snprintf(humidityBuff, sizeof(humidityBuff), "Air Hum: %.2f%%\r\n", data.airHumidity);
    Serial.print(humidityBuff);

    snprintf(soilHumBuf, sizeof(soilHumBuf), "Soil Hum: %u%%\r\n", data.soilHumidity);
    Serial.print(soilHumBuf);

    snprintf(luminosityBuff, sizeof(luminosityBuff), "Lum: %ulx\r\n", data.luminosity);
    Serial.print(luminosityBuff);
}

void print_wakeup_reason(){
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }
}
