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
#include "ilogger.h"

class Logger;
class LoggerConsoleWriter;

class ILogWriter{
public:
	virtual void write(Logger* sender, string msg, int level, string name, std::time_t dateTime) = 0;
	virtual ~ILogWriter(){};
};

class ILogWriterCacher: public ILogWriter{
private:
	condition_variable waiter;
	mutex listLocker;
	mutex threadExitMutex;

	mutex waitFlushMutex;

	bool running = true;
	Logger* ctrl;
	vector<tuple<Logger*, string, int, string, time_t>> cache;
public:
	ILogWriterCacher(Logger *ctrl, ILogWriter *driver);
	~ILogWriterCacher();
	void write(Logger* sender, string msg, int level, string name, time_t dateTime) override;
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
	string generateLineBegining(string level, string name, bool generateDateTime = true, time_t dateTime = -1, bool includeMilisseconds = false);

	string levelToString(int level, string defaultName = "INFO");
	string generateLineBegining(Logger *logger, int level, string name, bool generateDateTime = true, time_t dateTime = -1, bool includeMilisseconds = false)
	{
		return generateLineBegining(logger->levelToString(level), name, generateDateTime, dateTime, includeMilisseconds);
	}

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

	void addLogLevel(int logLevel, string levelDescription);
	map<int, string> getLogLevels();
	
	void log(int level, string name, string msg) override;
	void trace(string name, string msg) override;
	void debug2(string name, string msg) override;
	void debug(string name, string msg) override;
	void info2(string name, string msg) override;
	void info(string name, string msg) override;
	void warning(string name, string msg) override;
	void error(string name, string msg) override;
	void critical(string name, string msg) override;

	using ILogger::log;
	using ILogger::trace;
	using ILogger::debug2;
	using ILogger::debug;
	using ILogger::info2;
	using ILogger::info;
	using ILogger::warning;
	using ILogger::error;
	using ILogger::critical;
};

#include "writers/LoggerConsoleWriter.h"


#endif 
