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

using namespace std;




#define LOGGER_LOGLEVEL_DEBUG 1
#define LOGGER_LOGLEVEL_INFO 2
#define LOGGER_LOGLEVEL_WARNING 3
#define LOGGER_LOGLEVEL_ERROR 4
#define LOGGER_LOGLEVEL_CRITICAL 5

class NLogger;

class ILogger{
public:
	virtual void log(int level, string name, string msg) = 0;
	virtual void debug(string name, string msg) = 0;
	virtual void info(string name, string msg) = 0;
	virtual void warning(string name, string msg) = 0;
	virtual void error(string name, string msg) = 0;
	virtual void critical(string name, string msg, bool raiseException = false) = 0;

	virtual void log(int level, string name, vector<DynamicVar> msgs) = 0;
	virtual void debug(string name, vector<DynamicVar> msgs) = 0;
	virtual void info(string name, vector<DynamicVar> msgs) = 0;
	virtual void warning(string name, vector<DynamicVar> msgs) = 0;
	virtual void error(string name, vector<DynamicVar> msgs) = 0;
	virtual void critical(string name, vector<DynamicVar> msgs, bool raiseException = false) = 0;

	static string fromList(vector<DynamicVar> items){
		string out;
		for (size_t i = 0; i < items.size(); i++)
			out += items[i].getString() + (i <items.size()-1? " ": "");

		return out;
	}

	virtual void log(int level, string msg) = 0;
	virtual void debug(string msg) = 0;
	virtual void info(string msg) = 0;
	virtual void warning(string msg) = 0;
	virtual void error(string msg) = 0;
	virtual void critical(string msg, bool raiseException = true) = 0;
	virtual void log(int level, vector<DynamicVar> msgs) = 0;
	virtual void debug(vector<DynamicVar> msgs) = 0;
	virtual void info(vector<DynamicVar> msgs) = 0;
	virtual void warning(vector<DynamicVar> msgs) = 0;
	virtual void error(vector<DynamicVar> msgs) = 0;
	virtual void critical(vector<DynamicVar> msgs, bool raiseException = true) = 0;

	virtual NLogger getNamedLogger(string name) = 0;
};

class Logger;

class ILogWriter{
public:
	virtual void write(Logger* sender, string msg, int level, string name) = 0;
};


class ILogWriterCacher: ILogWriter{
private:
	mutex waiter;
	mutex listLocker;
	mutex threadExitMutex;

	bool running = true;
	ILogWriter *driver;
	vector<tuple<Logger*, string, int, string>> cache;
public:
	ILogWriterCacher(ILogWriter *driver)
	{
		waiter.lock();
		this->driver = driver;
		thread th([&](){this->run(); });

		th.detach();
	}

	~ILogWriterCacher()
	{
		threadExitMutex.lock();
		running = false;
		waiter.unlock();
		threadExitMutex.lock();
		threadExitMutex.unlock();
	}

	void write(Logger* sender, string msg, int level, string name)
	{
		listLocker.lock();
		cache.push_back(make_tuple(sender, msg, level, name));
		listLocker.unlock();

		waiter.unlock();
	}

	void run()
	{
		while (running)
		{
			waiter.lock();
			//if (cache.size() == 0)
			
			listLocker.lock();
			vector<tuple<Logger*, string, int, string>> cacheTmp = cache;
			cache.clear();
			listLocker.unlock();

			for (auto &c: cacheTmp)
				driver->write(std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c));
			
			cacheTmp.clear();
		}

		threadExitMutex.unlock();
	}
};

class Logger: public ILogger{
private:
	vector<ILogWriterCacher*> writers;

	bool running = true;
	
	void threadReadStdBuffer();

	map<int, string> logLevels {
		{LOGGER_LOGLEVEL_DEBUG, "DEBUG"},
		{LOGGER_LOGLEVEL_INFO, "INFO"},
		{LOGGER_LOGLEVEL_WARNING, "WARNING"},
		{LOGGER_LOGLEVEL_ERROR, "ERROR"},
		{LOGGER_LOGLEVEL_CRITICAL, "CRITICAL"},
	};

	void init(vector<ILogWriter*> writers, bool intercepCoutCerrAndCLog = false);
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
	static string getDate();

	//an util function to return the current time in a string
	static string getTime();

	/** an util function that returnt he current date and time in a string 
	 * @param date indicates if the result should contains the current date
	 * @param date indicates if the result should contains the current time
	 * @return return a date and time string*/
	static string generateDateTimeString(bool date = true, bool time = true);

	/** an utils function that returns a string to be put in the start of log lines
	 * @param name the name of the log
	 * @param level the log level of the line
	 * @param generateDateTime indicates if the date and time should be included in the result
	 * @return generate a head to log lines or log texts*/
	static string generateLineBegining(string level, string name, bool generateDateTime = true);

	static string generateLineBegining(Logger *logger, int level, string name, bool generateDateTime = true);

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

	void log(int level, string name, string msg);
	void debug(string name, string msg);
	void info(string name, string msg);
	void warning(string name, string msg);
	void error(string name, string msg);
	void critical(string name, string msg, bool raiseException = true);

	void log(int level, string name, vector<DynamicVar> msgs);
	void debug(string name, vector<DynamicVar> msgs);
	void info(string name, vector<DynamicVar> msgs);
	void warning(string name, vector<DynamicVar> msgs);
	void error(string name, vector<DynamicVar> msgs);
	void critical(string name, vector<DynamicVar> msgs, bool raiseException = true);



	void log(int level, string msg)
	{
		this->log(level, DEFAULT_LOG_NAME, msg);
	}

	void debug(string msg)
	{
		this->debug(DEFAULT_LOG_NAME, msg);
	}
	void info(string msg)
	{
		this->info(DEFAULT_LOG_NAME, msg);
	}
	void warning(string msg)
	{
		this->warning(DEFAULT_LOG_NAME, msg);
	}
	void error(string msg)
	{
		this->error(DEFAULT_LOG_NAME, msg);
	}
	void critical(string msg, bool raiseException = true)
	{
		this->critical(DEFAULT_LOG_NAME, msg);
	}


	void log(int level, vector<DynamicVar> msgs)
	{
		this->log(level, DEFAULT_LOG_NAME, msgs);
	}
	void debug(vector<DynamicVar> msgs)
	{
		this->debug(DEFAULT_LOG_NAME, msgs);
	}
	void info(vector<DynamicVar> msgs)
	{
		this->info(DEFAULT_LOG_NAME, msgs);
	}
	void warning(vector<DynamicVar> msgs)
	{
		this->warning(DEFAULT_LOG_NAME, msgs);
	}
	void error(vector<DynamicVar> msgs)
	{
		this->error(DEFAULT_LOG_NAME, msgs);
	}
	void critical(vector<DynamicVar> msgs, bool raiseException = true)
	{
		this->critical(DEFAULT_LOG_NAME, msgs);
	}

	
};

class NLogger{
private:
	Logger* mainLogger;
	string name;
public:
	NLogger(string name, Logger* mainLogger)
	{
		this->mainLogger = mainLogger;
		this->name = name;
	}

	~NLogger()
	{
		this->mainLogger = NULL;
	}
	
	void log(string msg, int level)
	{
		this->mainLogger->log(level, this->name, msg);
	}

	void debug(string msg)
	{
		this->mainLogger->debug(this->name, msg);
	}

	void info(string msg)
	{
		this->mainLogger->info(this->name, msg);
	}

	void warning(string msg)
	{
		this->mainLogger->warning(this->name, msg);
	}

	void error(string msg)
	{
		this->mainLogger->error(this->name, msg);
	}

	void critical(string msg, bool raiseException = true)
	{
		this->mainLogger->critical(this->name, msg, raiseException);
	}

	void log(vector<DynamicVar> msgs, int level)
	{
		this->mainLogger->log(level, this->name, msgs);
	}

	void debug(vector<DynamicVar> msgs)
	{
		this->mainLogger->debug(this->name, msgs);
	}

	void info(vector<DynamicVar> msgs)
	{
		this->mainLogger->info(this->name, msgs);
	}

	void warning(vector<DynamicVar> msgs)
	{
		this->mainLogger->warning(this->name, msgs);
	}

	void error(vector<DynamicVar> msgs)
	{
		this->mainLogger->error(this->name, msgs);
	}

	void critical(vector<DynamicVar> msgs, bool raiseException = true)
	{
		this->mainLogger->critical(this->name, msgs, raiseException);
	}
};

#endif 
