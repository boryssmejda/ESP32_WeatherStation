#pragma once


class SDCard
{
    constexpr static const char* filename = "/weatherConditions.txt";
    constexpr static const char* weatherConditionsBegin = "/begin.txt";
    constexpr static const char* weatherConditionsEnd = "/end.txt";

    size_t getWeatherConditionsBegin();
    size_t getWeatherConditionsEnd();
    void createFileWithContent(const char* filename, char* content);
    void readContentFromFile(const char* filename, char* content, size_t contentSize);

    void updateRangeEnd(size_t newEnd);

    public:
        void init();
        void write();
        void read();
        void append(char jsonSerialized[1024]);
        void deleteFile(const char* filename);
};