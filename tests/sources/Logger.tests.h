#ifndef __LOGGERTESTS__H__ 
#define __LOGGERTESTS__H__ 

#include <tester.h>
#include <logger.h>
#include <LoggerLambdaWriter.h>
 
class LoggerTests: public Tester { 
public: 
    LoggerTests(); 
    ~LoggerTests(); 

    vector<string> getContexts();
    void run(string context);
}; 
 
#endif 
