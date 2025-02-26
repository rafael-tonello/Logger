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
#include "writers/LoggerConsoleWriter.h"
#include "ilogger.h"

class Logger;

class ILogWriterCacher: public ILogWriter{
private:
	condition_variable waiter;
	mutex listLocker;
	mutex threadExitMutex;

	mutex waitFlushMutex;

	bool running = true;
	Logger* ctrl;
	vector<tuple<ILogger*, string, int, string, time_t>> cache;
public:
	ILogWriterCacher(Logger *ctrl, ILogWriter *driver);
	~ILogWriterCacher();
	void write(ILogger* sender, string msg, int level, string name, time_t dateTime) override;
	void run();
	void flush();

	ILogWriter *driver;
};

class Logger: public ILogger{
private:
	vector<ILogWriterCacher*> writers;

	bool running = true;
	
	void threadReadStdBuffer();
	int customTimeZoneOffsetInSeconds = -1;
	bool useCustomTimezone = false;

	map<int, string> logLevels {
		{LOGGER_LOGLEVEL_TRACE, "TRACE"},
		{LOGGER_LOGLEVEL_DEBUG2, "DEBUG2"},
		{LOGGER_LOGLEVEL_DEBUG, "DEBUG"},
		{LOGGER_LOGLEVEL_INFO2, "INFO2"},
		{LOGGER_LOGLEVEL_INFO, "INFO"},
		{LOGGER_LOGLEVEL_WARNING, "WARNING"},
		{LOGGER_LOGLEVEL_ERROR, "ERROR"},
		{LOGGER_LOGLEVEL_CRITICAL, "CRITICAL"},
	};

	void init(
		vector<ILogWriter*> writers, 
		bool intercepCoutCerrAndCLog = false, 
		bool tryToIdentifyLogLevelOfStdoutMessages = false, 
		bool useCustomTimezone = false, 
		int customTimeZoneOffsetInSeconds = 0
	);

	void flushCaches();
	string toLowerCase(string source);
public:

	streambuf* cout_originalBuffer;
	ostringstream coutInterceptor;
	streambuf* cerr_originalBuffer;
	ostringstream cerrInterceptor;
	streambuf* clog_originalBuffer;
	ostringstream clogInterceptor;

	Logger(
		vector<ILogWriter*> writers, 
		bool intercepCoutCerrAndCLog = false, 
		bool tryToIdentifyLogLevelOfStdoutMessages = false,
		bool useCustomTimezone = false, 
		int customTimeZoneOffsetInSeconds = 0
	);
	~Logger();

	//a special function used intercept stdout. It's used by driver LoggerConsoleWriter
	void _interceptStdout();

	//a special function used restore stdout. It's used by driver LoggerConsoleWriter
	void _restoreStdout();

	//an util function to return current date in a string
	static string getDateString(std::time_t rawTime);

	//an util function to return the current time in a string
	static string getTimeString(std::time_t rawTime, bool includeMilisseconds = false, bool useCustomTimezone = false, int customTimeZoneOffsetInSeconds = 0);

	static std::time_t getRawTime();

	static string remoteLastLineBreak(string data);

	struct GetTzLoggerInfo{
		bool useCustomTimezone;
		int customTimeZoneOffsetInSeconds;
	}; GetTzLoggerInfo getTzLoggerInfo();

	/** an util function that returnt he current date and time in a string 
	 * @param date indicates if the result should contains the current date
	 * @param date indicates if the result should contains the current time
	 * @return return a date and time string*/
	static string generateDateTimeString(time_t dateTime, bool date = true, bool time = true, bool milisseconds = false, bool useCustomTimezone = false, int customTimeZoneOffsetInSeconds = 0);

	/** an utils function that returns a string to be put in the start of log lines
	 * @param name the name of the log
	 * @param level the log level of the line
	 * @param generateDateTime indicates if the date and time should be included in the result
	 * @return generate a head to log lines or log texts*/
	string generateLineBegining(string level, string name, bool generateDateTime = true, time_t dateTime = -1, bool includeMilisseconds = false) override;

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
	bool _intercepCoutCerrAndCLog = false;
	bool tryToIdentifyLogLevelOfStdoutMessages = false;
	

	

	string DEFAULT_LOG_NAME = "";

	

	void addLogLevel(int logLevel, string levelDescription);
	map<int, string> getLogLevels();
	

	string levelToString(int level, string defaultName = "INFO") override;
	
	
	NLogger getNamedLogger(string name);
	NLogger* getNamedLoggerP(string name);

	void log(int level, string name, string msg);
	void trace(string name, string msg);
	void debug2(string name, string msg);
	void debug(string name, string msg);
	void info2(string name, string msg);
	void info(string name, string msg);
	void warning(string name, string msg);
	void error(string name, string msg);
	void critical(string name, string msg);

	void log(int level, string name, vector<DynamicVar> msgs);
	void trace(string name, vector<DynamicVar> msgs);
	void debug2(string name, vector<DynamicVar> msgs);
	void debug(string name, vector<DynamicVar> msgs);
	void info2(string name, vector<DynamicVar> msgs);
	void info(string name, vector<DynamicVar> msgs);
	void warning(string name, vector<DynamicVar> msgs);
	void error(string name, vector<DynamicVar> msgs);
	void critical(string name, vector<DynamicVar> msgs);



	void log(int level, string msg);
	void trace(string msg);
	void debug2(string msg);
	void debug(string msg);
	void info2(string msg);
	void info(string msg);
	void warning(string msg);
	void error(string msg);
	void critical(string msg);

	void log(int level, vector<DynamicVar> msgs);
	void trace(vector<DynamicVar> msgs);
	void debug2(vector<DynamicVar> msgs);
	void debug(vector<DynamicVar> msgs);
	void info2(vector<DynamicVar> msgs);
	void info(vector<DynamicVar> msgs);
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
	void trace(string msg);
	void debug2(string msg);
	void debug(string msg);
	void info2(string msg);
	void info(string msg);
	void warning(string msg);
	void error(string msg);
	void critical(string msg);

	void log(int level, vector<DynamicVar> msgs);
	void trace(vector<DynamicVar> msgs);
	void debug2(vector<DynamicVar> msgs);
	void debug(vector<DynamicVar> msgs);
	void info2(vector<DynamicVar> msgs);
	void info(vector<DynamicVar> msgs);
	void warning(vector<DynamicVar> msgs);
	void error(vector<DynamicVar> msgs);
	void critical(vector<DynamicVar> msgs);
};

#endif 
