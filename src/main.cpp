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

#include "BME280_wrapper.h"
#include "BH1750_wrapper.h"
#include "SoilHumiditySensor.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */


void printValues(Adafruit_BME280& bme);
void printSoilHumidity();
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

    BH1750_wrapper bh1750(&Wire);
    bh1750.printLuminosity();

    BME280_wrapper bme(&Wire);
    bme.printAll();

    SoilHumiditySensor soilHumSensor(soilHumSupplyPin, soilHumAnalogPin);
    soilHumSensor.printSoilHumidity();

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    Serial.flush();
    esp_deep_sleep_start();
}

void loop()
{
}

void printSoilHumidity()
{
    digitalWrite(soilHumSupplyPin, HIGH);
    delay(5);
    int soilAnalogVoltage = analogRead(soilHumAnalogPin);
    digitalWrite(soilHumSupplyPin, LOW);

    int soilHumidity = map(soilAnalogVoltage, 0, maxValueInWater, 0, 100);

    Serial.print("Soil: ");
    Serial.print(soilHumidity);
    Serial.println("%");
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
