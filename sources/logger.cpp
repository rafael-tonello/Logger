#include  "logger.h" 

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
            if (tmp.size() > 0)
            {
                this->info("stdout", tmp);
                coutInterceptor.str("");
                coutInterceptor.clear();
            }

            tmp = cerrInterceptor.str();
            if (tmp.size() > 0)
            {
                this->error("stderr", tmp);
                cerrInterceptor.str("");
                cerrInterceptor.clear();
            }

            tmp = clogInterceptor.str();
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

void Logger::Logger::info(string name, string msg){
    log(LOGGER_LOGLEVEL_INFO, name, msg);
}

void Logger::warning(string name, string msg){
    log(LOGGER_LOGLEVEL_WARNING, name, msg);
}

void Logger::error(string name, string msg){
    log(LOGGER_LOGLEVEL_ERROR, name, msg);
}

void Logger::critical(string name, string msg, bool raiseException){
    log(LOGGER_LOGLEVEL_CRITICAL, name, msg);
    if (raiseException)
        throw runtime_error(msg.c_str());
}

void Logger::log(int level, string name, vector<DynamicVar> msgs)
{
    this->log(level, name, fromList(msgs));
}

void Logger::debug(string name, vector<DynamicVar> msgs)
{
    this->debug(name, fromList(msgs));
}

void Logger::info(string name, vector<DynamicVar> msgs)
{
    this->info(name, fromList(msgs));
}

void Logger::warning(string name, vector<DynamicVar> msgs)
{
    this->warning(name, fromList(msgs));
}

void Logger::error(string name, vector<DynamicVar> msgs)
{
    this->error(name, fromList(msgs));
}

void Logger::critical(string name, vector<DynamicVar> msgs, bool raiseException)
{
    this->critical(name, fromList(msgs), raiseException);

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
