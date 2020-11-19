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

#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <PMserial.h>
#include "BluetoothSerial.h"

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) */

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
    #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

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
BluetoothSerial SerialBT;

// ===============================================================


RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR bool canExecuteRainInterrupt = true;

void turnOnLedWhenRainDetected()
{
    digitalWrite(2, HIGH);
    canExecuteRainInterrupt = false;
    bootCount = 0;
}

void printValues();

void setup()
{
    pinMode(27, OUTPUT);
    Serial.begin(115200);
    while(!Serial);

    Serial.println("Starting measurements!");
    Wire.begin();

    SerialBT.begin("ESP32test"); //Bluetooth device name
    Serial2.begin(9600);

    Serial.println("Starting assertion!");
    assert(bme.begin(BME280_DEFAULT_ADDRESS, &Wire) != false);
    Serial.println("Asserted BME280");
    assert(lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE, BH1750_DEFAULT_ADDRESS, &Wire) != false);
    pms.init();
    Serial.println("Asserted BH1750. STARTING!!");
}

void loop()
{
    digitalWrite(27, HIGH);
    delay(1000);
    printValues();
    digitalWrite(27, LOW);
    delay(9000);
}

void printValues()
{
    char temperatureBuff[25];
    char pressureBuff[25];
    char humidityBuff[25];
    char soilHumBuf[25];
    char luminosityBuff[25];

    snprintf(temperatureBuff, sizeof(temperatureBuff), "Temp: %.2f*C\r\n", bme.readTemperature());
    SerialBT.print(temperatureBuff);

    snprintf(pressureBuff, sizeof(pressureBuff), "Pressure: %.2f hPa\r\n", bme.readPressure());
    SerialBT.print(pressureBuff);

    snprintf(humidityBuff, sizeof(humidityBuff), "Air Hum: %.2f%%\r\n", bme.readHumidity()/ 100.0f);
    SerialBT.print(humidityBuff);

    snprintf(luminosityBuff, sizeof(luminosityBuff), "Lum: %ulx\r\n", (uint16_t)lightMeter.readLightLevel());
    SerialBT.print(luminosityBuff);

    pms.read();
    char airQualityBuffer[50];
    snprintf(airQualityBuffer, 50, "PM1.0 %u | PM2.5 %u | PM10 %u [ug/m3]\r\n", pms.pm01, pms.pm25, pms.pm10);
    SerialBT.println(airQualityBuffer);
    SerialBT.println("==================\n");
}

esp_sleep_wakeup_cause_t print_wakeup_reason(){
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

    return wakeup_reason;
}
