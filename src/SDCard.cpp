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

void SDCard::save(char jsonSerialized[1024])
{
    auto begin = getWeatherConditionsBegin();
    auto end = getWeatherConditionsEnd();

    Serial.print("Begin: ");
    Serial.println(begin);

    Serial.print("End: ");
    Serial.println(end);

    File file = SD.open(weatherConditions, FILE_APPEND);
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

    createFileWithContent(SDCard::end_filename, buffer);
}

void SDCard::updateRangeBeginning(size_t newBeginning)
{
    char buffer[8];
    sprintf(buffer, "%u", newBeginning);

    createFileWithContent(SDCard::begin_filename, buffer);
}

size_t SDCard::getWeatherConditionsBegin()
{
    char rangeBeginningBuffer[8] = "0";
    readContentFromFile(SDCard::begin_filename, rangeBeginningBuffer, sizeof(rangeBeginningBuffer));
    return atoi(rangeBeginningBuffer);
}

size_t SDCard::getWeatherConditionsEnd()
{
    char rangeEndBuffer[8] = "0";
    readContentFromFile(SDCard::end_filename, rangeEndBuffer, sizeof(rangeEndBuffer));
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

    file.readBytes(content, contentSize);
}

void SDCard::createFileWithContent(const char* filename, const char* content)
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

void SDCard::readWeatherCondtions(char jsonSerialized[1024])
{
    File file = SD.open(weatherConditions);
    if(!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    auto begin = getWeatherConditionsBegin();
    for(int i = 0; i < begin; ++i)
    {
        file.readBytesUntil('\r', jsonSerialized, 1024);
    }

    auto lastReadChar = file.readBytesUntil('\n', jsonSerialized, 1024);
    jsonSerialized[lastReadChar - 1] = '\0'; // to overwrite \r in the end
}

void SDCard::incrementBegin()
{
    auto begin = getWeatherConditionsBegin();
    updateRangeBeginning(begin+1);
}

bool SDCard::shouldDeleteWeatherCondtionsFile(int serverResponse)
{
    if(!isServerResponsePositive(serverResponse))
    {
        return false;
    }

    auto begin = getWeatherConditionsBegin();
    updateRangeBeginning(begin + 1);
    ++begin;

    auto end = getWeatherConditionsEnd();

    if(begin == end)
    {
        Serial.println("File is empty now! Deleting!");
        return true;
    }
    else
    {
        Serial.println("File not empty! Not deleting yet!");
        return false;
    }
}

bool SDCard::isServerResponsePositive(int serverResponse)
{
    return (serverResponse >= 200 && serverResponse <= 299);
}

void SDCard::deleteFile(const char* filename)
{
    if(!SD.exists(filename))
    {
        Serial.println("File does not exists! Cannot be deleted");
        return;
    }
    if(SD.remove(filename))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}

bool SDCard::isBeginEqualEnd()
{
    auto begin = getWeatherConditionsBegin();
    auto end = getWeatherConditionsEnd();

    return (begin == end);
}

bool SDCard::areAnyWeatherConditionsLeft()
{
    incrementBegin();
    if(isBeginEqualEnd())
    {
        deleteFile(SDCard::weatherConditions);
        deleteFile(SDCard::begin_filename);
        deleteFile(SDCard::end_filename);

        return false;
    }
    else
    {
        return true;
    }
}