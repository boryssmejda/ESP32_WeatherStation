#pragma once

#include <cstddef>

class SDCard
{
    size_t getWeatherConditionsBegin();
    size_t getWeatherConditionsEnd();
    void createFileWithContent(const char* filename, const char* content);
    void readContentFromFile(const char* filename, char* content, size_t contentSize);

    void updateRangeEnd(size_t newEnd);
    void updateRangeBeginning(size_t newBeginning);
    bool isServerResponsePositive(int serverResponse);

    public:
        constexpr static const char* weatherConditions = "/weatherConditions.txt";
        constexpr static const char* begin_filename = "/begin.txt";
        constexpr static const char* end_filename = "/end.txt";

        void init();
        void save(char jsonSerialized[1024]);
        void readWeatherCondtions(char jsonSerialized[1024]);

        void incrementBegin();

        bool shouldDeleteWeatherCondtionsFile(int serverResponse);
        void deleteFile(const char* filename);

        bool isBeginEqualEnd();
        bool areAnyWeatherConditionsLeft();
};