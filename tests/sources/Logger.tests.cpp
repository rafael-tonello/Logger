#include  "Logger.tests.h" 
 
LoggerTests::LoggerTests() 
{ 
     
} 
 
LoggerTests::~LoggerTests() 
{ 
     
} 
 
vector<string> LoggerTests::getContexts()
{
    return {"LoggerTests"};

}

void LoggerTests::run(string context)
{
    if (context != "LoggerTests")
        return;

    this->test("Should create a log line with curent timezone", [](){
        std::time_t outputDate;
        Logger *logger = new Logger({new LoggerLambdaWriter([&](Logger* sender, string msg, int level, string name, std::time_t dateTime){
            outputDate = dateTime;
        })}, false, false, false, 0);

        logger->info("Test", "Test message");
        
        //get time_t timezone
        std::tm* now = std::localtime(&outputDate);
        auto timezone = now->tm_gmtoff;

        //get current timezone
        std::time_t rawtime;
        std::time(&rawtime);
        std::tm* timeinfo;
        timeinfo = std::localtime(&rawtime);
        auto currentTz = timeinfo->tm_gmtoff;

        return timezone == currentTz;


        delete logger;
    });

    this->test("getTimeString should return a string with the current time in the current timezone", [](){
        string nowTime = Logger::getTimeString(Logger::getRawTime());

        //create the desired value for 'nowTime' use strftime with "%H:%M:%Sms%z"
        std::time_t rawtime;
        std::time(&rawtime);
        std::tm* timeinfo;
        timeinfo = std::localtime(&rawtime);
        char buffer [80];
        std::strftime(buffer,80,"%H:%M:%S%z",timeinfo);
        auto desired= string(buffer);

        return nowTime == desired;

    });

    this->test("getTimeString should return a string with the current time in timezone 0 (GMT)", [](){
        string nowTime = Logger::getTimeString(Logger::getRawTime(), false, true, 0);

        //create the desired value for 'nowTime' use strftime with "%H:%M:%Sms%z" (for the timezone 0)
        std::time_t rawtime;
        std::time(&rawtime);
        std::tm* timeinfo;
        timeinfo = std::localtime(&rawtime);

        auto totalOffset = 0 - timeinfo->tm_gmtoff;
        timeinfo->tm_sec += totalOffset;
        auto tmp = std::mktime(timeinfo);

        //convert time_t to tm
        timeinfo = std::localtime(&tmp);
        timeinfo->tm_gmtoff = 0;
        
        char buffer [80];
        std::strftime(buffer,80,"%H:%M:%S%z",timeinfo);
        auto desired= string(buffer);

        return nowTime == desired;

    });


}
