#include "HttpRequest.h"
#include "WeatherStation.h"
#include "Timestamp.h"
#include "SDCard.h"
#include "JSONParser.h"
#include "WifiWrapper.h"


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds) */

void try_sending_weatherConditions(char weatherConditons[1024]);
void initCommunicationProtocols();

Timestamp timestamp;
SDCard mySD;
WeatherStation weatherStation;

void setup()
{
    initCommunicationProtocols();

    mySD.init();
    weatherStation.init();

    WifiWrapper::connect();
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

    try_sending_weatherConditions(jsonSerializedOutput);

    Serial.println("===============\r\n");


    delay(10000);
}


void try_sending_weatherConditions(char weatherConditons[1024])
{
    while(true)
    {
        memset(weatherConditons, 0, 1024);
        mySD.readWeatherCondtions(weatherConditons);

        JsonParser::mergeWeatherConditionsWithHeader(weatherConditons);

        auto httpResponse = HttpRequest::sendWeatherConditions(weatherConditons);

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
}

void initCommunicationProtocols()
{
    Serial.begin(115200);
    while(!Serial);
    Serial.flush();

    Wire.begin();

    Serial2.begin(9600);
    Serial2.flush();
}
