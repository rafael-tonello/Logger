#include "main.h"

ulong logger_new_logger()
{
    auto tmp = new SetupData();
    ulong instance = (ulong)tmp;

    loggersSetups[instance] = tmp;

    return instance;
}

const char* logger_setup_addOutputFile(ulong instance, char* filename, int logLevel)
{
    
    if (loggersSetups.find(instance) == loggersSetups.end())
    {
        return "Invalid instance";
    }

    auto loggerSetup = loggersSetups[instance];

    loggerSetup->writers.push_back(new LoggerFileWriter(filename, logLevel));

    return nullptr;
}

const char* logger_setup_addConsoleOutput(ulong instance, int logLevel, bool useColors, bool displayLinePrefix, bool displayDateTime, bool printMilisseconds)
{
    if (loggersSetups.find(instance) == loggersSetups.end())
    {
        return "Invalid instance";
    }
    auto loggerSetup = loggersSetups[instance];
    loggerSetup->writers.push_back(new LoggerConsoleWriter(logLevel, useColors, displayLinePrefix, displayDateTime, printMilisseconds));

    return nullptr;

}

const char* logger_setup_done(ulong instance)
{
    if (loggersSetups.find(instance) == loggersSetups.end())
    {
        return "Invalid instance";
    }
    auto loggerSetup = loggersSetups[instance];

    loggerSetup->logger = new Logger(loggerSetup->writers, false, false);

    return nullptr;

}

const char* logger_free_logger(ulong instance)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { return "Invalid instance"; }
    auto loggerSetup = loggersSetups[instance];

    loggerSetup->writers.clear();
    delete loggerSetup;

    loggersSetups.erase(instance);
    

    return nullptr;

}

const char* logger_log(ulong instance, int level, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }

    loggerSetup->logger->log(level, string(name), string(msg));

    
    return nullptr;
}

const char* logger_trace(ulong instance, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }


    loggerSetup->logger->trace(string(name), string(msg));

    
    return nullptr;    

}

const char* logger_debug2(ulong instance, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }


    loggerSetup->logger->debug2(string(name), string(msg));

    
    return nullptr;    
}

const char* logger_debug(ulong instance, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }


    loggerSetup->logger->debug(string(name), string(msg));

    
    return nullptr;    
}

const char* logger_info2(ulong instance, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }

    loggerSetup->logger->info2(string(name), string(msg));

    return nullptr;
}

const char* logger_info(ulong instance, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }

    loggerSetup->logger->info(string(name), string(msg));

    return nullptr;

}

const char* logger_warning(ulong instance, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }

    loggerSetup->logger->warning(string(name), string(msg));

    return nullptr;

}

const char* logger_error(ulong instance, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }

    loggerSetup->logger->error(string(name), string(msg));

    return nullptr;
}

const char* logger_critical(ulong instance, char* name, char* msg)
{
    if (loggersSetups.find(instance) == loggersSetups.end()) { 
        return "Invalid instance"; 
    }
    auto loggerSetup = loggersSetups[instance];

    if (loggerSetup->logger == nullptr)
    {
        return "Logger not initialized";
    }

    loggerSetup->logger->critical(string(name), string(msg));

    return nullptr;
}
