#ifndef __LOGLAMBDAWRITER__H__ 
#define __LOGLAMBDAWRITER__H__ 

#include <functional>
#include "../logger.h"

using namespace std;
 
class LoggerLambdaWriter: public ILogWriter
{
public:
	function<void(Logger* sender, string msg, int level, string name, std::time_t dateTime)> writerFunc;
public:
	LoggerLambdaWriter(function<void(Logger* sender, string msg, int level, string name, std::time_t dateTime)> writerFunc);
	
	void write(Logger* sender, string msg, int level, string name, std::time_t dateTime);
};
 
#endif 
