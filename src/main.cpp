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
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>


void printValues();
void printLuminosity();
void printSoilHumidity();

Adafruit_BME280 bme; // I2C
TwoWire I2CBME = TwoWire(0);
BH1750 lightMeter(0x23);
const int soilHumAnalogPin = 34;
const int soilHumSupplyPin = 33;
const int maxValueInWater = 2300;
unsigned long delayTime;

void setup()
{
    pinMode(soilHumSupplyPin, OUTPUT);

    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));

    unsigned int status;
    I2CBME.begin();
    // default settings
    //status = bme.begin();
    // You can also pass in a Wire library object like &Wire2
    status = bme.begin(0x76, &I2CBME);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }

    while(lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE, 0x23, &I2CBME) != true)
    {
      Serial.print("Could not configure!");
      delay(1000);
    }

    delayTime = 3000;
    esp_sleep_enable_timer_wakeup(3600000);
}


void loop()
{
    printValues();
    printLuminosity();
    printSoilHumidity();

    Serial.println();
    delay(delayTime);
}


void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
}

void printLuminosity()
{
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
}

void printSoilHumidity()
{
    digitalWrite(soilHumSupplyPin, HIGH);
    delay(10);
    int soilAnalogVoltage = analogRead(soilHumAnalogPin);
    digitalWrite(soilHumSupplyPin, LOW);

    int soilHumidity = map(soilAnalogVoltage, 0, maxValueInWater, 0, 100);

    Serial.print("Soil: ");
    Serial.print(soilHumidity);
    Serial.println();
}
