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


void ILogger::log(int level, string msg)
{
    this->log(level, DEFAULT_LOG_NAME, msg);
}

void ILogger::trace(string msg)
{
    this->trace(DEFAULT_LOG_NAME, msg);
}

void ILogger::debug(string msg)
{
    this->debug(DEFAULT_LOG_NAME, msg);
}
void ILogger::debug2(string msg)
{
    this->debug2(DEFAULT_LOG_NAME, msg);
}
void ILogger::info(string msg)
{
    this->info(DEFAULT_LOG_NAME, msg);
}
void ILogger::info2(string msg)
{
    this->info2(DEFAULT_LOG_NAME, msg);
}
void ILogger::warning(string msg)
{
    this->warning(DEFAULT_LOG_NAME, msg);
}
void ILogger::error(string msg)
{
    this->error(DEFAULT_LOG_NAME, msg);
}
void ILogger::critical(string msg)
{
    this->critical(DEFAULT_LOG_NAME, msg);
}

void ILogger::log(int level, vector<DynamicVar> msgs)
{
    this->log(level, DEFAULT_LOG_NAME, msgs);
}
void ILogger::trace(vector<DynamicVar> msgs)
{
    this->trace(DEFAULT_LOG_NAME, msgs);
}
void ILogger::debug(vector<DynamicVar> msgs)
{
    this->debug(DEFAULT_LOG_NAME, msgs);
}
void ILogger::debug2(vector<DynamicVar> msgs)
{
    this->debug2(DEFAULT_LOG_NAME, msgs);
}
void ILogger::info(vector<DynamicVar> msgs)
{
    this->info(DEFAULT_LOG_NAME, msgs);
}
void ILogger::info2(vector<DynamicVar> msgs)
{
    this->info2(DEFAULT_LOG_NAME, msgs);
}
void ILogger::warning(vector<DynamicVar> msgs)
{
    this->warning(DEFAULT_LOG_NAME, msgs);
}
void ILogger::error(vector<DynamicVar> msgs)
{
    this->error(DEFAULT_LOG_NAME, msgs);
}
void ILogger::critical(vector<DynamicVar> msgs)
{
    this->critical(DEFAULT_LOG_NAME, msgs);
}

void ILogger::log(int level, string name, vector<DynamicVar> msgs)
{
    this->log(level, name, fromList(msgs));
}

void ILogger::trace(string name, vector<DynamicVar> msgs)
{
    this->trace(name, fromList(msgs));
}

void ILogger::debug(string name, vector<DynamicVar> msgs)
{
    this->debug(name, fromList(msgs));
}

void ILogger::debug2(string name, vector<DynamicVar> msgs)
{
    this->debug2(name, fromList(msgs));
}

void ILogger::info(string name, vector<DynamicVar> msgs)
{
    this->info(name, fromList(msgs));
}

void ILogger::info2(string name, vector<DynamicVar> msgs)
{
    this->info2(name, fromList(msgs));
}

void ILogger::warning(string name, vector<DynamicVar> msgs)
{
    this->warning(name, fromList(msgs));
}

void ILogger::error(string name, vector<DynamicVar> msgs)
{
    this->error(name, fromList(msgs));
}

void ILogger::critical(string name, vector<DynamicVar> msgs)
{
    this->critical(name, fromList(msgs));
}

