#ifndef __MAIN__H__
#define __MAIN__H__

#include <iostream>
#include <map>
#include <vector>
#include <tuple>
#include<logger.h>
#include <LoggerConsoleWriter.h>
#include <LoggerFileWriter.h>

using namespace std;

struct SetupData
{
    std::vector<ILogWriter*> writers;

    Logger* logger = nullptr;
};

//this map is used to manage the instances of the logger and make the library more "fault tolerant"
map<uint, SetupData*> loggersSetups;

//error buffer
char* errorBuffer = new char[4096];


extern "C" ulong logger_new_logger();

//the functions bellow returns errors in case of failure.
extern "C" const char* logger_setup_addOutputFile(ulong instance, char* filename, int logLevel);
extern "C" const char* logger_setup_addConsoleOutput(ulong instance, int logLevel, bool useColors, bool displayLinePrefix, bool displayDateTime, bool printMilisseconds);
extern "C" const char* logger_setup_done(ulong instance);
extern "C" const char* logger_free_logger(ulong instance);

extern "C" const char* logger_log(ulong instance, int level, char* name, char* msg);
extern "C" const char* logger_trace(ulong instance, char* name, char* msg);
extern "C" const char* logger_debug2(ulong instance, char* name, char* msg);
extern "C" const char* logger_debug(ulong instance, char* name, char* msg);
extern "C" const char* logger_info2(ulong instance, char* name, char* msg);
extern "C" const char* logger_info(ulong instance, char* name, char* msg);
extern "C" const char* logger_warning(ulong instance, char* name, char* msg);
extern "C" const char* logger_error(ulong instance, char* name, char* msg);
extern "C" const char* logger_critical(ulong instance, char* name, char* msg);


#endif