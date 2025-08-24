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
#define DEFAULT_LOG_NAME ""

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

	virtual void log(int level, string name, vector<DynamicVar> msgs);
	virtual void trace(string name, vector<DynamicVar> msgs);
	virtual void debug2(string name, vector<DynamicVar> msgs);
	virtual void debug(string name, vector<DynamicVar> msgs);
	virtual void info2(string name, vector<DynamicVar> msgs);
	virtual void info(string name, vector<DynamicVar> msgs);
	virtual void warning(string name, vector<DynamicVar> msgs);
	virtual void error(string name, vector<DynamicVar> msgs);
	virtual void critical(string name, vector<DynamicVar> msgs);

	static string fromList(vector<DynamicVar> items){
		string out;
		for (size_t i = 0; i < items.size(); i++)
			out += items[i].getString() + (i <items.size()-1? " ": "");

		return out;
	}

	virtual void log(int level, string msg);
	virtual void trace(string msg);
	virtual void debug(string msg);
	virtual void debug2(string msg);
	virtual void info(string msg);
	virtual void info2(string msg);
	virtual void warning(string msg);
	virtual void error(string msg);
	virtual void critical(string msg);

	virtual void log(int level, vector<DynamicVar> msgs);
	virtual void trace(vector<DynamicVar> msgs);
	virtual void debug(vector<DynamicVar> msgs);
	virtual void debug2(vector<DynamicVar> msgs);
	virtual void info(vector<DynamicVar> msgs);
	virtual void info2(vector<DynamicVar> msgs);
	virtual void warning(vector<DynamicVar> msgs);
	virtual void error(vector<DynamicVar> msgs);
	virtual void critical(vector<DynamicVar> msgs);

	//have default implementation (ilogger.cpp)
	virtual NLogger getNamedLogger(string name);
	//have default implementation (ilogger.cpp)
	virtual NLogger* getNamedLoggerP(string name);


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

//have default implementation (ilogger.cpp)
class NLogger{
private:
	ILogger* mainLogger;
	string name;
public:
	NLogger();
	NLogger(string name, ILogger* mainLogger);
	~NLogger();
	virtual void setLogName(string name);
	virtual void setMainLogger(ILogger* mainLogger);

	virtual void log(int level, string msg);
	virtual void trace(string msg);
	virtual void debug2(string msg);
	virtual void debug(string msg);
	virtual void info2(string msg);
	virtual void info(string msg);
	virtual void warning(string msg);
	virtual void error(string msg);
	virtual void critical(string msg);

	virtual void log(int level, vector<DynamicVar> msgs);
	virtual void trace(vector<DynamicVar> msgs);
	virtual void debug2(vector<DynamicVar> msgs);
	virtual void debug(vector<DynamicVar> msgs);
	virtual void info2(vector<DynamicVar> msgs);
	virtual void info(vector<DynamicVar> msgs);
	virtual void warning(vector<DynamicVar> msgs);
	virtual void error(vector<DynamicVar> msgs);
	virtual void critical(vector<DynamicVar> msgs);
};

#endif 
