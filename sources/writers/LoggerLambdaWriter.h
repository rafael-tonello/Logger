#ifndef __LOGLAMBDAWRITER__H__ 
#define __LOGLAMBDAWRITER__H__ 

#include <functional>
#include "../ilogger.h"

using namespace std;
 
class LoggerLambdaWriter: public ILogWriter
{
public:
	function<void(ILogger* sender, string msg, int level, string name, std::time_t dateTime)> writerFunc;
public:
	LoggerLambdaWriter(function<void(ILogger* sender, string msg, int level, string name, std::time_t dateTime)> writerFunc);
	
	void write(ILogger* sender, string msg, int level, string name, std::time_t dateTime) override;
};
 
#endif 
