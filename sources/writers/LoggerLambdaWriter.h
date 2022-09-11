#ifndef __LOGLAMBDAWRITER__H__ 
#define __LOGLAMBDAWRITER__H__ 

#include <functional>
#include "../logger.h"

using namespace std;
 
class LoggerLambdaWriter: public ILogWriter
{
public:
	function<void(Logger* sender, string msg, int level, string name)> writerFunc;
public:
	LoggerLambdaWriter(function<void(Logger* sender, string msg, int level, string name)> writerFunc);
	
	void write(Logger* sender, string msg, int level, string name);
};
 
#endif 
