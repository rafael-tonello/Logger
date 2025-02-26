#include "loggercommandcallwriter.h"

LoggerCommandCallWriter::LoggerCommandCallWriter(string command, int minIntervalBetweenCalls_us): command(command), minIntervalBetweenCalls_us(minIntervalBetweenCalls_us)
{
    srand(time(NULL));
    running = true;
    flushCurrentCacheFile();
    waitFlushMutex.lock();

    //start thread
    this->threadHandle = new thread([&](){
        this->threadRun();
    });
    this->threadHandle->detach();
}

LoggerCommandCallWriter::~LoggerCommandCallWriter()
{  
    //release thread to exit
    waitFlushMutex.unlock();

    //wait thread to exit
    exitMutex.lock();
}


void LoggerCommandCallWriter::threadRun()
{
    while (running){
        waitFlushMutex.lock();
        //prepare a new cache file

        if (running){
            flushCurrentCacheFile();

            // Do something
            //wait some time to prevent too frequent external command calls
            if (minIntervalBetweenCalls_us > 0)
                usleep(minIntervalBetweenCalls_us);
        }
        else{
            break;
        }
    }

    flushCurrentCacheFile(false);
    //release destructor
    exitMutex.unlock();
}

void LoggerCommandCallWriter::flushCurrentCacheFile(bool createNew)
{
    string oldCacheFileName = currentCacheFileName;

    if (currentCacheFile.is_open())
    {
        currentCacheFile.flush();
        currentCacheFile.close();
    }

    if (!createNew)
    {
        this->currentCacheFileName = createNewCacheFileName();
        currentCacheFile.open(currentCacheFileName, ios::app);
        currentCacheFile << getCsvFileHeader() << "\n";
    }

    if (oldCacheFileName != "")
        flushCacheFile(oldCacheFileName, this->command);
        
}

void LoggerCommandCallWriter::write(ILogger* sender, string msg, int level, string name, std::time_t dateTime){
    //"<completeLineHeader>","<Text>","<name>",<lovLevelNumber>,"<logLevelStr>"
    string lineHeader = sender->generateLineBegining(sender, level, name, true, dateTime, true);

    string cacheFileLine = 
        "\""+Utils::escapeString(lineHeader)+ "\","+
        "\""+Utils::escapeString(msg)+ "\","+
        "\""+Utils::escapeString(name)+ "\","+
        to_string(level)+ ",";
        
    
    currentCacheFile << cacheFileLine << "\n";

    waitFlushMutex.unlock();
}

void LoggerCommandCallWriter::flushCacheFile(string cacheFile, string command)
{
    Utils::ssystem(command + " \"" + cacheFile + "\"");
    Utils::ssystem("rm -f \"" + cacheFile + "\"");
}

string LoggerCommandCallWriter::createNewCacheFileName()
{
    string currMicrosseconds = to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    string random = to_string(rand());
    
    string ret = "/dev/shm/logger_commandwriter_cache_"+currMicrosseconds+"_"+random+"_"+".cache";

    Utils::writeTextFileContent(ret, getCsvFileHeader() + "\n");

    return ret;
}

string LoggerCommandCallWriter::getCsvFileHeader()
{
    return "\"completeLineHeader\",\"Text\",\"name\",\"lovLevelNumber\",\"logLevelStr\"";
}   