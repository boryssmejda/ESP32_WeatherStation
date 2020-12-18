#include "HttpRequest.h"
#include "WeatherStation.h"
#include "Timestamp.h"
#include "SDCard.h"
#include "JSONParser.h"
#include "WifiWrapper.h"
#include "Timer.h"


void try_sending_weatherConditions(char weatherConditons[1024]);
void initCommunicationProtocols();
void print_wakeup_reason();

Timestamp timestamp;
SDCard mySD;
WeatherStation weatherStation;

unsigned long lastTime = 0;

void setup()
{
    Timer t(std::chrono::seconds{60});

    initCommunicationProtocols();

    mySD.init();
    weatherStation.init();

    WifiWrapper::connect();

    print_wakeup_reason();

    Serial.println("\r\n--------------------------------");

    char jsonSerializedOutput[1024];
    char timestampBuffer[64];

    timestamp.getTimestamp(timestampBuffer, sizeof(timestampBuffer));

    JsonParser jsonParser(timestampBuffer, sizeof(timestampBuffer), weatherStation.requestWeatherConditions());
    jsonParser.createFile(jsonSerializedOutput);

    mySD.save(jsonSerializedOutput);

    try_sending_weatherConditions(jsonSerializedOutput);

    Serial.println("===============\r\n");
}

void loop()
{
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

void print_wakeup_reason()
{
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
