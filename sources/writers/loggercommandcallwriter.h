#ifndef __LOGGERCOMMANDCALLWRITER__H__
#define __LOGGERCOMMANDCALLWRITER__H__

#include "../logger.h"
#include <thread>
#include <mutex>
#include <string>
#include <chrono>
#include <utils.h>

using namespace std;

//"<completeLineHeader>","<Text>","<name>",<lovLevelNumber>,"<logLevelStr>",<timeStrWithTz>,<isLoggerUsinCustomTzOffset>,<loggerCustomTzOffsetInSeconds>

class LoggerCommandCallWriter: public ILogWriter{
public: /* ILogWeriter */
    void write(Logger* sender, string msg, int level, string name, std::time_t dateTime) override;
private:
    mutex waitFlushMutex;
    mutex exitMutex;
    bool running = false;
    string currentCacheFileName="";
    ofstream currentCacheFile;
    string command;
    thread *threadHandle;
    int minIntervalBetweenCalls_us;


    void flushCurrentCacheFile(bool createNew = true);
    static void flushCacheFile(string cacheFile, string command);
    static string createNewCacheFileName();
    static string getCsvFileHeader();
    void threadRun();
public:
    LoggerCommandCallWriter(string command, int minIntervalBetweenCalls_us = 500000);
    ~LoggerCommandCallWriter();

};

#endif
