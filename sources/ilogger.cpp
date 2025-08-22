#include "ilogger.h"



NLogger ILogger::getNamedLogger(string name)
{
    NLogger namedLogger(name, this);
    return namedLogger;
}

NLogger* ILogger::getNamedLoggerP(string name)
{
    return new NLogger(name, this);
}


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
