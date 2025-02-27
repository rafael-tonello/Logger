#ifndef __ILOGGER__H__ 
#define __ILOGGER__H__ 

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <DynamicVar.h>
#include <unistd.h>
#include <thread>
#include <ostream>
#include <map>
#include <sstream>
#include <mutex>
#include <condition_variable>

using namespace std;

#define LOGGER_LOGLEVEL_TRACE 9
#define LOGGER_LOGLEVEL_DEBUG2 10
#define LOGGER_LOGLEVEL_DEBUG 20
#define LOGGER_LOGLEVEL_INFO2 30
#define LOGGER_LOGLEVEL_INFO 40
#define LOGGER_LOGLEVEL_WARNING 50
#define LOGGER_LOGLEVEL_ERROR 60
#define LOGGER_LOGLEVEL_CRITICAL 70

//A helper do diferenciate calls between (string arg) functions and (vector<DynamicVar> args) functions
#define DVV vector<DynamicVar>

class NLogger;

class ILogger{
public:
	virtual void log(int level, string name, string msg) = 0;
	virtual void trace(string name, string msg) = 0;
	virtual void debug2(string name, string msg) = 0;
	virtual void debug(string name, string msg) = 0;
	virtual void info2(string name, string msg) = 0;
	virtual void info(string name, string msg) = 0;
	virtual void warning(string name, string msg) = 0;
	virtual void error(string name, string msg) = 0;
	virtual void critical(string name, string msg) = 0;

	virtual void log(int level, string name, vector<DynamicVar> msgs) = 0;
	virtual void trace(string name, vector<DynamicVar> msgs) = 0;
	virtual void debug2(string name, vector<DynamicVar> msgs) = 0;
	virtual void debug(string name, vector<DynamicVar> msgs) = 0;
	virtual void info2(string name, vector<DynamicVar> msgs) = 0;
	virtual void info(string name, vector<DynamicVar> msgs) = 0;
	virtual void warning(string name, vector<DynamicVar> msgs) = 0;
	virtual void error(string name, vector<DynamicVar> msgs) = 0;
	virtual void critical(string name, vector<DynamicVar> msgs) = 0;

	static string fromList(vector<DynamicVar> items){
		string out;
		for (size_t i = 0; i < items.size(); i++)
			out += items[i].getString() + (i <items.size()-1? " ": "");

		return out;
	}

	virtual void log(int level, string msg) = 0;
	virtual void trace(string msg) = 0;
	virtual void debug(string msg) = 0;
	virtual void debug2(string msg) = 0;
	virtual void info(string msg) = 0;
	virtual void info2(string msg) = 0;
	virtual void warning(string msg) = 0;
	virtual void error(string msg) = 0;
	virtual void critical(string msg) = 0;

	virtual void log(int level, vector<DynamicVar> msgs) = 0;
	virtual void trace(vector<DynamicVar> msgs) = 0;
	virtual void debug(vector<DynamicVar> msgs) = 0;
	virtual void debug2(vector<DynamicVar> msgs) = 0;
	virtual void info(vector<DynamicVar> msgs) = 0;
	virtual void info2(vector<DynamicVar> msgs) = 0;
	virtual void warning(vector<DynamicVar> msgs) = 0;
	virtual void error(vector<DynamicVar> msgs) = 0;
	virtual void critical(vector<DynamicVar> msgs) = 0;

	virtual NLogger getNamedLogger(string name) = 0;
	virtual NLogger* getNamedLoggerP(string name) = 0;

	virtual string generateLineBegining(string level, string name, bool generateDateTime = true, time_t dateTime = -1, bool includeMilisseconds = false) = 0;
	virtual string levelToString(int level, string defaultName = "INFO") = 0;
	
	virtual string generateLineBegining(ILogger *logger, int level, string name, bool generateDateTime = true, time_t dateTime = -1, bool includeMilisseconds = false)
	{
		return generateLineBegining(logger->levelToString(level), name, generateDateTime, dateTime, includeMilisseconds);
	}

/** helper functions */
public:
	static string stringReplace(string source, string replace, string by)
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

	static string identLog(string log, string prefix)
	{
		auto temp = stringReplace(log, "\n", "\r\t"+prefix);
		return stringReplace(temp, "\r\t", "\n");
	}
};

class ILogWriter{
public:
	virtual void write(ILogger* sender, string msg, int level, string name, std::time_t dateTime) = 0;
	virtual ~ILogWriter(){};
};

#endif 
