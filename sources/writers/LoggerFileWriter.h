#ifndef __LOGFILEWRITER__H__ 
#define __LOGFILEWRITER__H__ 

#include <fstream>
#include "../ilogger.h"
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
	int logLevel = LOGGER_LOG_ALL_LEVELS;
	bool printMilisseconds = true;
	string fileName;
	int maxFileSize = -1;
	mutex fileLock;
	mutex fileSizeCheckerTheadLocker;
	thread *fileSizeCheckerThead = nullptr;
	bool stopFileSizeCheckerThead = false;

	void checkFileSize();
public:
	/* A driver to write a plain text log file. This driver ignores the 'logLevel' and writers all logs to the file.
     * @param fName is the destination filename 
	 * @param logLevel is the level of the logs that will be written to the file. If the log level is less than the log level of the log, the log will not be written to the file.
	 * @param printMilisseconds is a flag to print the milisseconds in the log
	 * @param maxFileSize is the maximum size of the log file in bytes. If maxFileSize > 0 and the file exceeds this size the logfile will be compatec to the folder <fName>.history/<fName>.<date>.<time>.tar.xz
	 */

	LoggerFileWriter(string fname, int logLevel = LOGGER_LOG_ALL_LEVELS, bool printMilisseconds = true, int maxFileSize = -1);
	~LoggerFileWriter();
	void write(ILogger* sender, string msg, int level, string name, std::time_t dateTime);
};
 
#endif 
