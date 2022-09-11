#ifndef __LOGCONSOLEWRITER__H__ 
#define __LOGCONSOLEWRITER__H__ 

#include <iostream>
#include <string>
#include "../logger.h"

using namespace std;



class LoggerConsoleWriter: public ILogWriter
{
private:
    bool colors = true;
    bool displayLinePrefix = true;
    bool displayDateTime = true;
    int logLevel = LOGGER_LOGLEVEL_INFO;
public:
    /* A writer to write a plain text log file
     * @param userColors enables the use of colors on terminals that support it
     * @param displayLinePrefix allow you to chose if the 'line header' should be printed or not to the STD */
    LoggerConsoleWriter(int logLevel = LOGGER_LOGLEVEL_INFO, bool useColors = true, bool displayLinePrefix = true, bool displayDateTime = true);
	void write(Logger* sender, string msg, int level, string name);
};
 
#endif 
