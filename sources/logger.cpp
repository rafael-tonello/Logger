#include  "logger.h" 

#pragma region Logger class

void Logger::init(vector<ILogWriter*> writers, bool intercepCoutCerrAndCLog, bool tryToIdentifyLogLevelOfStdoutMessages, bool useCustomTimezone, int customTimeZoneOffsetInSeconds)
{
    //this->writers = writers;

    for (auto &c: writers)
    {
        ILogWriterCacher *tmp = new ILogWriterCacher(this, c);
        this->writers.push_back(tmp);
    }

    this->_intercepCoutCerrAndCLog = intercepCoutCerrAndCLog;
    this->tryToIdentifyLogLevelOfStdoutMessages = tryToIdentifyLogLevelOfStdoutMessages;
    this->useCustomTimezone = useCustomTimezone;
    this->customTimeZoneOffsetInSeconds = customTimeZoneOffsetInSeconds;


    if (intercepCoutCerrAndCLog)
    {
        _interceptStdout();

        threadReadStdBuffer();
    }
}

Logger::Logger(vector<ILogWriter*> writers, bool intercepCoutCerrAndCLog, bool tryToIdentifyLogLevelOfStdoutMessages, bool useCustomTimezone, int customTimeZoneOffsetInSeconds){
    this->init(writers, intercepCoutCerrAndCLog, tryToIdentifyLogLevelOfStdoutMessages, useCustomTimezone, customTimeZoneOffsetInSeconds);
}

Logger::~Logger()
{
    this->flushCaches();
    usleep(15000);
    for (auto &c: this->writers)
        delete c;
        
    this->writers.clear();
    running = false;
}

void Logger::_interceptStdout()
{
    if (isCurrentlyTnterceptingCoutCerrAndCLog)
        return;
    isCurrentlyTnterceptingCoutCerrAndCLog = true;
    // Redirect cout.
    cout_originalBuffer = cout.rdbuf();
    cout.rdbuf( coutInterceptor.rdbuf() );

    cerr_originalBuffer = cerr.rdbuf();
    cerr.rdbuf( cerrInterceptor.rdbuf() );

    clog_originalBuffer = clog.rdbuf();
    clog.rdbuf( clogInterceptor.rdbuf() );

}

void Logger::_restoreStdout()
{
    if (!isCurrentlyTnterceptingCoutCerrAndCLog)
        return;

    isCurrentlyTnterceptingCoutCerrAndCLog = false;
    // Restore old cout.
    cout.rdbuf( cout_originalBuffer );
    cerr.rdbuf( cerr_originalBuffer );
    clog.rdbuf( clog_originalBuffer );
}

void Logger::threadReadStdBuffer()
{
    thread th ([&](){
        while (running)
        {
            auto tmp = coutInterceptor.str();
            tmp = remoteLastLineBreak(tmp);
            if (tmp.size() > 0)
            {
                if (tryToIdentifyLogLevelOfStdoutMessages)
                {
                    string tmp2;
                    if (tmp.size() > 25)
                        tmp2 = tmp.substr(0, 25);
                    else
                        tmp2 = tmp;

                    tmp2 = toLowerCase(tmp2);

                    if (tmp2.find("trace") != string::npos)
                        this->trace("stdout", tmp);
                    else if (tmp2.find("debug") != string::npos)
                        this->debug("stdout", tmp);
                    else if (tmp2.find("debug2") != string::npos)
                        this->debug2("stdout", tmp);
                    else if (tmp2.find("info2") != string::npos)
                        this->info2("stdout", tmp);
                    else if (tmp2.find("warning") != string::npos)
                        this->warning("stdout", tmp);
                    else if (tmp2.find("error") != string::npos)
                        this->error("stdout", tmp);
                    else if ((tmp2.find("critical") != string::npos) || (tmp2.find("fatal") != string::npos))
                        this->critical("stdout", tmp);

                    else
                        this->info("stdout", tmp);
                }
                else
                    this->info("stdout", tmp);

                coutInterceptor.str("");
                coutInterceptor.clear();
            }

            tmp = cerrInterceptor.str();
            tmp = remoteLastLineBreak(tmp);
            if (tmp.size() > 0)
            {
                this->error("stderr", tmp);
                cerrInterceptor.str("");
                cerrInterceptor.clear();
            }

            tmp = clogInterceptor.str();
            tmp = remoteLastLineBreak(tmp);
            if (tmp.size() > 0)
            {
                this->warning("stdlog", tmp);
                clogInterceptor.str("");
                clogInterceptor.clear();
            }
            usleep(10000);
        }
    });
    th.detach();
}

string Logger::toLowerCase(string source)
{
    std::string result = source;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}

void Logger::log(int level, string name, string msg)
{
    for (auto &c : this->writers) {
        c->write(this, msg, level, name, getRawTime());
    }

    if (level == LOGGER_LOGLEVEL_CRITICAL)
        this->flushCaches();
}

void Logger::trace(string name, string msg){
    log(LOGGER_LOGLEVEL_TRACE, name, msg);
}

void Logger::debug(string name, string msg){
    log(LOGGER_LOGLEVEL_DEBUG, name, msg);
}

void Logger::debug2(string name, string msg){
    log(LOGGER_LOGLEVEL_DEBUG2, name, msg);
}

void Logger::Logger::info(string name, string msg){
    log(LOGGER_LOGLEVEL_INFO, name, msg);
}

void Logger::Logger::info2(string name, string msg){
    log(LOGGER_LOGLEVEL_INFO2, name, msg);
}

void Logger::warning(string name, string msg){
    log(LOGGER_LOGLEVEL_WARNING, name, msg);
}

void Logger::error(string name, string msg){
    log(LOGGER_LOGLEVEL_ERROR, name, msg);
}

void Logger::critical(string name, string msg){
    log(LOGGER_LOGLEVEL_CRITICAL, name, msg);
}

void Logger::log(int level, string name, vector<DynamicVar> msgs)
{
    this->log(level, name, fromList(msgs));
}

void Logger::trace(string name, vector<DynamicVar> msgs)
{
    this->trace(name, fromList(msgs));
}

void Logger::debug(string name, vector<DynamicVar> msgs)
{
    this->debug(name, fromList(msgs));
}

void Logger::debug2(string name, vector<DynamicVar> msgs)
{
    this->debug2(name, fromList(msgs));
}

void Logger::info(string name, vector<DynamicVar> msgs)
{
    this->info(name, fromList(msgs));
}

void Logger::info2(string name, vector<DynamicVar> msgs)
{
    this->info2(name, fromList(msgs));
}

void Logger::warning(string name, vector<DynamicVar> msgs)
{
    this->warning(name, fromList(msgs));
}

void Logger::error(string name, vector<DynamicVar> msgs)
{
    this->error(name, fromList(msgs));
}

void Logger::critical(string name, vector<DynamicVar> msgs)
{
    this->critical(name, fromList(msgs));
}

void Logger::flushCaches()
{
    for (auto &c: writers)
    {
        c->flush();
    }
}

string Logger::levelToString(int level, string defaultName)
{
    if (this->logLevels.count(level))
        return this->logLevels[level];
    else
        return defaultName;
}

void Logger::addLogLevel(int logLevel, string levelDescription)
{
    logLevels[logLevel] = levelDescription;
}

map<int, string> Logger::getLogLevels()
{
    return logLevels;
}

std::time_t Logger::getRawTime()
{
    std::time_t rawtime;
    std::time(&rawtime);

    return rawtime;
}

string Logger::getDateString(std::time_t rawTime)
{
    std::tm* timeinfo;
    char buffer [80];
    timeinfo = std::localtime(&rawTime);

    std::strftime(buffer,80,"%Y-%m-%d",timeinfo);

    return string(buffer);
}

string Logger::getTimeString(std::time_t rawTime, bool includeMilisseconds, bool useCustomTimezone, int customTimeZoneOffsetInSeconds)
{
    std::tm* timeinfo;
    char buffer [80];

    timeinfo = std::localtime(&rawTime);

    if (useCustomTimezone)
    {
        //time in current timezone + (destination timezone - current timezone)
        auto totalOffset = customTimeZoneOffsetInSeconds - timeinfo->tm_gmtoff;

        timeinfo->tm_sec += totalOffset;

        auto tmp = std::mktime(timeinfo);

        //convert time_t to tm
        timeinfo = std::localtime(&tmp);
        timeinfo->tm_gmtoff = customTimeZoneOffsetInSeconds;

    }


    std::strftime(buffer,80,"%H:%M:%Sms%z",timeinfo);
    auto result = string(buffer);
    if (includeMilisseconds)
    {
        auto ms_raw = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;
        auto ms = "." + std::to_string(ms_raw);
        result = Logger::stringReplace(result, "ms", ms);  
    }
    else
    {
        result = Logger::stringReplace(result, "ms", "");   
    }

    return result;

}

string Logger::remoteLastLineBreak(string data)
{
    if (data.find("\r\n") == data.size()-2)
        return data.substr(0, data.size()-2);
    else if (data.find("\r") == data.size()-1 || data.find("\n") == data.size()-1)
        return data.substr(0, data.size()-1);
    
    return data;
}

string Logger::generateDateTimeString(time_t dateTime, bool date, bool time, bool milisseconds, bool useCustomTimezone, int customTimeZoneOffsetInSeconds)
{

    string result = "";

    if (date)
        result = getDateString(dateTime);

    if (time)
    {
        string time = getTimeString(dateTime, milisseconds, useCustomTimezone, customTimeZoneOffsetInSeconds);
        if (time != "")
        {
            if (result != "")
                result += "T";

            result += time;
        }
    }

    return result;
}

string Logger::generateLineBegining(string level, string name, bool generateDateTime, time_t dateTime, bool includeMilisseconds)
{
    string prefix = "";
    
    if (generateDateTime)
    {
        if (dateTime == -1)
            dateTime = getRawTime();
        prefix += "["+Logger::generateDateTimeString(dateTime, true, true, includeMilisseconds, useCustomTimezone, customTimeZoneOffsetInSeconds)+"] ";
    }

    prefix += "["+level + "] ";
    
    if (name != "") 
        prefix += "[" + name + "] ";


    return prefix;
}

NLogger Logger::getNamedLogger(string name)
{
    NLogger namedLogger(name, this);
    return namedLogger;
}

NLogger* Logger::getNamedLoggerP(string name)
{
    return new NLogger(name, this);
}

string Logger::stringReplace(string source, string replace, string by)
{
    if (source.find(replace) != string::npos)
    {
        string tmp = source.substr(0, source.find(replace));
        tmp += by;
        tmp += source.substr(source.find(replace)+replace.size());
        return stringReplace(tmp, replace, by);
    }
    else
        return source;
}

void Logger::log(int level, string msg)
{
    this->log(level, DEFAULT_LOG_NAME, msg);
}

void Logger::trace(string msg)
{
    this->trace(DEFAULT_LOG_NAME, msg);
}

void Logger::debug(string msg)
{
    this->debug(DEFAULT_LOG_NAME, msg);
}
void Logger::debug2(string msg)
{
    this->debug2(DEFAULT_LOG_NAME, msg);
}
void Logger::info(string msg)
{
    this->info(DEFAULT_LOG_NAME, msg);
}
void Logger::info2(string msg)
{
    this->info2(DEFAULT_LOG_NAME, msg);
}
void Logger::warning(string msg)
{
    this->warning(DEFAULT_LOG_NAME, msg);
}
void Logger::error(string msg)
{
    this->error(DEFAULT_LOG_NAME, msg);
}
void Logger::critical(string msg)
{
    this->critical(DEFAULT_LOG_NAME, msg);
}

void Logger::log(int level, vector<DynamicVar> msgs)
{
    this->log(level, DEFAULT_LOG_NAME, msgs);
}
void Logger::trace(vector<DynamicVar> msgs)
{
    this->trace(DEFAULT_LOG_NAME, msgs);
}
void Logger::debug(vector<DynamicVar> msgs)
{
    this->debug(DEFAULT_LOG_NAME, msgs);
}
void Logger::debug2(vector<DynamicVar> msgs)
{
    this->debug2(DEFAULT_LOG_NAME, msgs);
}
void Logger::info(vector<DynamicVar> msgs)
{
    this->info(DEFAULT_LOG_NAME, msgs);
}
void Logger::info2(vector<DynamicVar> msgs)
{
    this->info2(DEFAULT_LOG_NAME, msgs);
}
void Logger::warning(vector<DynamicVar> msgs)
{
    this->warning(DEFAULT_LOG_NAME, msgs);
}
void Logger::error(vector<DynamicVar> msgs)
{
    this->error(DEFAULT_LOG_NAME, msgs);
}
void Logger::critical(vector<DynamicVar> msgs)
{
    this->critical(DEFAULT_LOG_NAME, msgs);
}


Logger::GetTzLoggerInfo Logger::getTzLoggerInfo()
{
    return GetTzLoggerInfo{
        this->useCustomTimezone,
        this->customTimeZoneOffsetInSeconds
    };
}
#pragma endregion

#pragma region ILogWriterCacher class

ILogWriterCacher::ILogWriterCacher(Logger *ctrl, ILogWriter *driver)
{
    this->driver = driver;
    this->ctrl = ctrl;
    thread th([&](){this->run(); });

    th.detach();
}

ILogWriterCacher::~ILogWriterCacher()
{
    threadExitMutex.lock();
    running = false;
    waiter.notify_one();
    threadExitMutex.lock();
    threadExitMutex.unlock();
    delete this->driver;
}

void ILogWriterCacher::write(ILogger* sender, string msg, int level, string name, time_t dateTime)
{
    listLocker.lock();
    cache.push_back(make_tuple(sender, msg, level, name, dateTime));
    listLocker.unlock();

    waiter.notify_one();
}

void ILogWriterCacher::run()
{
    mutex mtx;
    unique_lock<std::mutex> lk(mtx);
    while (running)
    {   
        listLocker.lock();
        vector<tuple<ILogger*, string, int, string, time_t>> cacheTmp = cache;
        cache.clear();
        listLocker.unlock();

        for (auto &c: cacheTmp)
        {
            if (dynamic_cast<LoggerConsoleWriter*>(driver) && ctrl->_intercepCoutCerrAndCLog)
            {
                ctrl->_restoreStdout();
            }

            driver->write(std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c), std::get<4>(c));

            if (dynamic_cast<LoggerConsoleWriter*>(driver) && ctrl->_intercepCoutCerrAndCLog)
            {
                ctrl->_restoreStdout();
            }
        }
        
        cacheTmp.clear();

        waitFlushMutex.unlock();
        if (cache.size() == 0 && running)
            waiter.wait(lk);
    }

    threadExitMutex.unlock();
}

void ILogWriterCacher::flush()
{
    waitFlushMutex.lock();
    waiter.notify_one();
    waitFlushMutex.lock();
    waitFlushMutex.unlock();
}

#pragma endregion

#pragma region NLogger class


NLogger::NLogger()
{

}

NLogger::NLogger(string name, ILogger* mainLogger)
{
    this->setMainLogger(mainLogger);
    this->setLogName(name);
}

NLogger::~NLogger()
{
    this->mainLogger = NULL;
}

void NLogger::setLogName(string name)
{
    this->name = name;
}

void NLogger::setMainLogger(ILogger* mainLogger)
{
    this->mainLogger = mainLogger;
}

void NLogger::log(int level, string msg)
{
    this->mainLogger->log(level, this->name, msg);
}

void NLogger::trace(string msg)
{
    this->mainLogger->trace(this->name, msg);
}
void NLogger::debug(string msg)
{
    this->mainLogger->debug(this->name, msg);
}
void NLogger::debug2(string msg)
{
    this->mainLogger->debug2(this->name, msg);
}

void NLogger::info(string msg)
{
    this->mainLogger->info(this->name, msg);
}
void NLogger::info2(string msg)
{
    this->mainLogger->info2(this->name, msg);
}

void NLogger::warning(string msg)
{
    this->mainLogger->warning(this->name, msg);
}

void NLogger::error(string msg)
{
    this->mainLogger->error(this->name, msg);
}

void NLogger::critical(string msg)
{
    this->mainLogger->critical(this->name, msg);
}

void NLogger::log(int level, vector<DynamicVar> msgs)
{
    this->mainLogger->log(level, this->name, msgs);
}

void NLogger::trace(vector<DynamicVar> msgs)
{
    this->mainLogger->trace(this->name, msgs);
}
void NLogger::debug(vector<DynamicVar> msgs)
{
    this->mainLogger->debug(this->name, msgs);
}
void NLogger::debug2(vector<DynamicVar> msgs)
{
    this->mainLogger->debug2(this->name, msgs);
}

void NLogger::info(vector<DynamicVar> msgs)
{
    this->mainLogger->info(this->name, msgs);
}
void NLogger::info2(vector<DynamicVar> msgs)
{
    this->mainLogger->info2(this->name, msgs);
}

void NLogger::warning(vector<DynamicVar> msgs)
{
    this->mainLogger->warning(this->name, msgs);
}

void NLogger::error(vector<DynamicVar> msgs)
{
    this->mainLogger->error(this->name, msgs);
}

void NLogger::critical(vector<DynamicVar> msgs)
{
    this->mainLogger->critical(this->name, msgs);
}

#pragma endregion
