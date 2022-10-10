#ifndef __LOGGER__H__ 
#define __LOGGER__H__ 

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
	virtual void debug(string name, string msg) = 0;
	virtual void debug2(string name, string msg) = 0;
	virtual void info(string name, string msg) = 0;
	virtual void info2(string name, string msg) = 0;
	virtual void warning(string name, string msg) = 0;
	virtual void error(string name, string msg) = 0;
	virtual void critical(string name, string msg) = 0;

	virtual void log(int level, string name, vector<DynamicVar> msgs) = 0;
	virtual void debug(string name, vector<DynamicVar> msgs) = 0;
	virtual void debug2(string name, vector<DynamicVar> msgs) = 0;
	virtual void info(string name, vector<DynamicVar> msgs) = 0;
	virtual void info2(string name, vector<DynamicVar> msgs) = 0;
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
	virtual void debug(string msg) = 0;
	virtual void debug2(string msg) = 0;
	virtual void info(string msg) = 0;
	virtual void info2(string msg) = 0;
	virtual void warning(string msg) = 0;
	virtual void error(string msg) = 0;
	virtual void critical(string msg) = 0;

	virtual void log(int level, vector<DynamicVar> msgs) = 0;
	virtual void debug(vector<DynamicVar> msgs) = 0;
	virtual void debug2(vector<DynamicVar> msgs) = 0;
	virtual void info(vector<DynamicVar> msgs) = 0;
	virtual void info2(vector<DynamicVar> msgs) = 0;
	virtual void warning(vector<DynamicVar> msgs) = 0;
	virtual void error(vector<DynamicVar> msgs) = 0;
	virtual void critical(vector<DynamicVar> msgs) = 0;

	virtual NLogger getNamedLogger(string name) = 0;
	virtual NLogger* getNamedLoggerP(string name) = 0;
};

class Logger;

class ILogWriter{
public:
	virtual void write(Logger* sender, string msg, int level, string name, std::time_t dateTime) = 0;
};


class ILogWriterCacher: ILogWriter{
private:
	condition_variable waiter;
	mutex listLocker;
	mutex threadExitMutex;

	mutex waitFlushMutex;

	bool running = true;
	ILogWriter *driver;
	vector<tuple<Logger*, string, int, string, time_t>> cache;
public:
	ILogWriterCacher(ILogWriter *driver);
	~ILogWriterCacher();
	void write(Logger* sender, string msg, int level, string name, time_t dateTime);
	void run();
	void flush();
};

class Logger: public ILogger{
private:
	vector<ILogWriterCacher*> writers;

	bool running = true;
	
	void threadReadStdBuffer();

	map<int, string> logLevels {
		{LOGGER_LOGLEVEL_DEBUG2, "DEBUG2"},
		{LOGGER_LOGLEVEL_DEBUG, "DEBUG"},
		{LOGGER_LOGLEVEL_INFO2, "INFO2"},
		{LOGGER_LOGLEVEL_INFO, "INFO"},
		{LOGGER_LOGLEVEL_WARNING, "WARNING"},
		{LOGGER_LOGLEVEL_ERROR, "ERROR"},
		{LOGGER_LOGLEVEL_CRITICAL, "CRITICAL"},
	};

	void init(vector<ILogWriter*> writers, bool intercepCoutCerrAndCLog = false);
	void flushCaches();
public:

	streambuf* cout_originalBuffer;
	ostringstream coutInterceptor;
	streambuf* cerr_originalBuffer;
	ostringstream cerrInterceptor;
	streambuf* clog_originalBuffer;
	ostringstream clogInterceptor;

	Logger(vector<ILogWriter*> writers, bool intercepCoutCerrAndCLog = false);
	~Logger();

	//a special function used intercept stdout. It's used by driver LoggerConsoleWriter
	void interceptStdout();

	//a special function used restore stdout. It's used by driver LoggerConsoleWriter
	void restoreStdout();

	//an util function to return current date in a string
	static string getDateString(std::time_t rawTime);

	//an util function to return the current time in a string
	static string getTimeString(std::time_t rawTime);

	static std::time_t getRawTime();

	static string remoteLastLineBreak(string data);

	/** an util function that returnt he current date and time in a string 
	 * @param date indicates if the result should contains the current date
	 * @param date indicates if the result should contains the current time
	 * @return return a date and time string*/
	static string generateDateTimeString(time_t dateTime, bool date = true, bool time = true);

	/** an utils function that returns a string to be put in the start of log lines
	 * @param name the name of the log
	 * @param level the log level of the line
	 * @param generateDateTime indicates if the date and time should be included in the result
	 * @return generate a head to log lines or log texts*/
	static string generateLineBegining(string level, string name, bool generateDateTime = true, time_t dateTime = -1);

	static string generateLineBegining(Logger *logger, int level, string name, bool generateDateTime = true, time_t dateTime = -1);

	/** An utils function to ident multi line logs items. Basically, this function put the 'prefix' in the beginning of each line.
	 * @param log the multi line log text
	 * @param prefix the prfix to be inserted in the start of eah line of log text 
	 * @return an idented version of 'log' text */
	static string identLog(string log, string prefix);

	/** An Util function to replace text in a string
	 * @param source the source string
	 * @param replace the text to be replaced
	 * @param by the new text 
	 * @return a new string based in 'source', but with ocurrences of 'replace' changed to 'by' */
	static string stringReplace(string source, string replace, string by);

	bool isCurrentlyTnterceptingCoutCerrAndCLog = false;
	bool intercepCoutCerrAndCLog;

	

	string DEFAULT_LOG_NAME = "";

	

	void addLogLevel(int logLevel, string levelDescription);
	map<int, string> getLogLevels();
	

	string levelToString(int level, string defaultName = "INFO");
	
	
	NLogger getNamedLogger(string name);
	NLogger* getNamedLoggerP(string name);

	void log(int level, string name, string msg);
	void debug(string name, string msg);
	void debug2(string name, string msg);
	void info(string name, string msg);
	void info2(string name, string msg);
	void warning(string name, string msg);
	void error(string name, string msg);
	void critical(string name, string msg);

	void log(int level, string name, vector<DynamicVar> msgs);
	void debug(string name, vector<DynamicVar> msgs);
	void debug2(string name, vector<DynamicVar> msgs);
	void info(string name, vector<DynamicVar> msgs);
	void info2(string name, vector<DynamicVar> msgs);
	void warning(string name, vector<DynamicVar> msgs);
	void error(string name, vector<DynamicVar> msgs);
	void critical(string name, vector<DynamicVar> msgs);



	void log(int level, string msg);
	void debug(string msg);
	void debug2(string msg);
	void info(string msg);
	void info2(string msg);
	void warning(string msg);
	void error(string msg);
	void critical(string msg);

	void log(int level, vector<DynamicVar> msgs);
	void debug(vector<DynamicVar> msgs);
	void debug2(vector<DynamicVar> msgs);
	void info(vector<DynamicVar> msgs);
	void info2(vector<DynamicVar> msgs);
	void warning(vector<DynamicVar> msgs);
	void error(vector<DynamicVar> msgs);
	void critical(vector<DynamicVar> msgs);
};

class NLogger{
private:
	ILogger* mainLogger;
	string name;
public:
	NLogger();
	NLogger(string name, ILogger* mainLogger);
	~NLogger();
	void setLogName(string name);
	void setMainLogger(ILogger* mainLogger);

	void log(int level, string msg);
	void debug(string msg);
	void debug2(string msg);
	void info(string msg);
	void info2(string msg);
	void warning(string msg);
	void error(string msg);
	void critical(string msg);

	void log(int level, vector<DynamicVar> msgs);
	void debug(vector<DynamicVar> msgs);
	void debug2(vector<DynamicVar> msgs);
	void info(vector<DynamicVar> msgs);
	void info2(vector<DynamicVar> msgs);
	void warning(vector<DynamicVar> msgs);
	void error(vector<DynamicVar> msgs);
	void critical(vector<DynamicVar> msgs);
};

#endif 
