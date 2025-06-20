#ifndef __LOGCONSOLEWRITER__H__ 
#define __LOGCONSOLEWRITER__H__ 

#include <iostream>
#include <string>
#include "../ilogger.h"

using namespace std;

class LoggerConsoleWriter: public ILogWriter {
private:
    bool colors = true;
    bool displayLinePrefix = true;
    bool displayDateTime = true;
    int logLevel = LOGGER_LOGLEVEL_INFO;
    bool printMilisseconds = false;
public:
    /* A writer to write a plain text log file
     * @param userColors enables the use of colors on terminals that support it
     * @param displayLinePrefix allow you to chose if the 'line header' should be printed or not to the STD */
    LoggerConsoleWriter(int logLevel = LOGGER_LOGLEVEL_INFO, bool useColors = true, bool displayLinePrefix = true, bool displayDateTime = true, bool printMilisseconds = false);
    ~LoggerConsoleWriter();
	void write(ILogger* sender, string msg, int level, string name, std::time_t dateTime) override;
};
 
#endif 
