#include  "logger.h" 

#pragma region Logger class

void Logger::init(vector<ILogWriter*> writers, bool intercepCoutCerrAndCLog)
{
    //this->writers = writers;

    for (auto &c: writers)
    {
        ILogWriterCacher *tmp = new ILogWriterCacher(c);
        this->writers.push_back(tmp);
    }

    this->intercepCoutCerrAndCLog = intercepCoutCerrAndCLog;


    if (intercepCoutCerrAndCLog)
    {
        interceptStdout();

        threadReadStdBuffer();
    }
}

Logger::Logger(vector<ILogWriter*> writers, bool intercepCoutCerrAndCLog){
    this->init(writers, intercepCoutCerrAndCLog);
}

Logger::~Logger()
{
    for (auto &c: this->writers)
        delete c;
        
    this->writers.clear();
    running = false;
    this->flushCaches();
    usleep(15000);
}

void Logger::interceptStdout()
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

void Logger::restoreStdout()
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

void Logger::log(int level, string name, string msg)
{
    for (auto &c : this->writers)
        c->write(this, msg, level, name);
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
    this->flushCaches();
}

void Logger::log(int level, string name, vector<DynamicVar> msgs)
{
    this->log(level, name, fromList(msgs));
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
        c->flush();
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



string Logger::getDate()
{

    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer,80,"%Y-%m-%d",timeinfo);

    return string(buffer);
}

string Logger::getTime()
{
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer,80,"%H:%M:%S%z",timeinfo);

    return string(buffer);

}

string Logger::remoteLastLineBreak(string data)
{
    if (data.find("\r\n") == data.size()-2)
        return data.substr(0, data.size()-2);
    else if (data.find("\r") == data.size()-1 || data.find("\n") == data.size()-1)
        return data.substr(0, data.size()-1);
    
    return data;
}

string Logger::generateDateTimeString(bool date, bool time)
{

    string result = "";

    if (date)
        result = getDate();

    if (time)
    {
        string time = getTime();
        if (time != "")
        {
            if (result != "")
                result += "T";

            result += time;
        }
    }

    return result;
}

string Logger::generateLineBegining( string level, string name, bool generateDateTime)
{
    string prefix = "";
    
    if (generateDateTime)
        prefix += "["+Logger::generateDateTimeString()+"] ";

    prefix += "["+level + "] ";
    
    if (name != "") 
        prefix += "[" + name + "] ";


    return prefix;
}

string Logger::generateLineBegining(Logger *logger, int level, string name, bool generateDateTime)
{
    return Logger::generateLineBegining(logger->levelToString(level), name, generateDateTime);
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

string Logger::identLog(string log, string prefix)
{
    auto temp = stringReplace(log, "\n", "\r\t"+prefix);
    return stringReplace(temp, "\r\t", "\n");
}

void Logger::log(int level, string msg)
{
    this->log(level, DEFAULT_LOG_NAME, msg);
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

#pragma endregion

#pragma region ILogWriterCacher class

ILogWriterCacher::ILogWriterCacher(ILogWriter *driver)
{
    this->driver = driver;
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
}

void ILogWriterCacher::write(Logger* sender, string msg, int level, string name)
{
    listLocker.lock();
    cache.push_back(make_tuple(sender, msg, level, name));
    listLocker.unlock();

    waiter.notify_one();
}

void ILogWriterCacher::run()
{
    mutex mtx;
    unique_lock<std::mutex> lk(mtx);
    while (running)
    {
        
        //if (cache.size() == 0)
        
        listLocker.lock();
        vector<tuple<Logger*, string, int, string>> cacheTmp = cache;
        cache.clear();
        listLocker.unlock();

        for (auto &c: cacheTmp)
            driver->write(std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c));
        
        cacheTmp.clear();

        waitFlushMutex.unlock();

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
