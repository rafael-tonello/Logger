#ifndef __LOGFILEWRITER__H__ 
#define __LOGFILEWRITER__H__ 

#include <fstream>
#include "../logger.h"
#include <ctime>
#include <limits.h>
 
using namespace std;

#define LOGGER_LOG_ALL_LEVELS INT_MIN

class LoggerFileWriter: public ILogWriter
{		
private:
	ofstream file;
	string getDate();
	string getTime();
	string generateDateTimeString();
	int logLevel = LOGGER_LOG_ALL_LEVELS;
	bool printMilisseconds = true;
public:
	/* A driver to write a plain text log file. This driver ignores the 'logLevel' and writers all logs to the file.
     * @param fName is the destination filename */
	LoggerFileWriter(string fname, int logLevel = LOGGER_LOG_ALL_LEVELS, bool printMilisseconds = true);
	~LoggerFileWriter();
	void write(Logger* sender, string msg, int level, string name, std::time_t dateTime);
};
 
#endif 
