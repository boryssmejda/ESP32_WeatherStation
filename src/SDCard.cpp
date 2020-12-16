#include "SDCard.h"
#include "SD.h"

// Define CS pin for the SD card module
#define SD_CS 5

void SDCard::init()
{
    assert(SD.begin(SD_CS));

    uint8_t cardType = SD.cardType();
    if(cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
    }
}

void SDCard::write()
{
    File file = SD.open("/data.txt", FILE_WRITE);
    if(!file)
    {
        Serial.println("Could not create file!");
        return;
    }

    if(file.print("Hello Borys Smejda!"))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }

    file.close();
}

void SDCard::read()
{
    File file = SD.open(filename);
    if(!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available())
    {
        Serial.write(file.read());
    }
}

void SDCard::append(char jsonSerialized[1024])
{
    auto begin = getWeatherConditionsBegin();
    auto end = getWeatherConditionsEnd();

    Serial.print("Begin: ");
    Serial.println(begin);

    Serial.print("End: ");
    Serial.println(end);

    File file = SD.open(filename, FILE_APPEND);
    if(!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.println(jsonSerialized))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();

    updateRangeEnd(end+1);
}

void SDCard::updateRangeEnd(size_t newEnd)
{
    char buffer[8];
    sprintf(buffer, "%u", newEnd);

    createFileWithContent(weatherConditionsEnd, buffer);
}

size_t SDCard::getWeatherConditionsBegin()
{
    char rangeBeginningBuffer[8] = "0";
    readContentFromFile(weatherConditionsBegin, rangeBeginningBuffer, sizeof(rangeBeginningBuffer));
    return atoi(rangeBeginningBuffer);
}

size_t SDCard::getWeatherConditionsEnd()
{
    char rangeEndBuffer[8] = "0";
    readContentFromFile(weatherConditionsBegin, rangeEndBuffer, sizeof(rangeEndBuffer));
    return atoi(rangeEndBuffer);
}

void SDCard::readContentFromFile(const char* filename, char* content, size_t contentSize)
{
    File file = SD.open(filename);
    if(!file)
    {
        Serial.println("Failed to open file for reading");
        createFileWithContent(filename, "0");
        strcpy(content, "0");
    }

    Serial.print("Read from file: ");
    file.readBytes(content, contentSize);
}

void SDCard::createFileWithContent(const char* filename, char* content)
{
    File file = SD.open(filename, FILE_WRITE);
    if(!file)
    {
        Serial.println("Could not create file!");
        return;
    }

    if(file.print(content))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}

void SDCard::deleteFile(const char* filename)
{
    if(SD.remove(filename))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}