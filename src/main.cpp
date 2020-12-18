#include "HttpRequest.h"
#include "WeatherStation.h"
#include "Timestamp.h"
#include "SDCard.h"
#include "JSONParser.h"


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */


void initCommunicationProtocols()
{
    Serial.begin(115200);
    while(!Serial);
    Serial.flush();

    Wire.begin();

    Serial2.begin(9600);
    Serial2.flush();
}

Timestamp timestamp;
SDCard mySD;
WeatherStation weatherStation;

void setup()
{
    initCommunicationProtocols();

    mySD.init();
    weatherStation.init();

    mySD.deleteFile(SDCard::weatherConditions);
    mySD.deleteFile(SDCard::begin_filename);
    mySD.deleteFile(SDCard::end_filename);
}

void loop()
{
    Serial.println("\r\n--------------------------------");

    char jsonSerializedOutput[1024];
    char timestampBuffer[64];

    timestamp.getTimestamp(timestampBuffer, sizeof(timestampBuffer));

    JsonParser jsonParser(timestampBuffer, sizeof(timestampBuffer), weatherStation.requestWeatherConditions());
    jsonParser.createFile(jsonSerializedOutput);

    mySD.save(jsonSerializedOutput);

    while(true)
    {
        memset(jsonSerializedOutput, 0, sizeof(jsonSerializedOutput));
        mySD.readWeatherCondtions(jsonSerializedOutput);

        JsonParser::mergeWeatherConditionsWithHeader(jsonSerializedOutput);

        auto httpResponse = HttpRequest::sendWeatherConditions(jsonSerializedOutput);

        if(HttpRequest::isRequestSuccessfull(httpResponse))
        {
            if(mySD.areAnyWeatherConditionsLeft())
            {
                delay(1000);
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    Serial.println("===============\r\n");


    delay(10000);
}
