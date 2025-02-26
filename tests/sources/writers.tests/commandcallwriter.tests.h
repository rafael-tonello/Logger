#ifndef __COMMANDCALLWRITERTESTS__H__
#define __COMMANDCALLWRITERTESTS__H__

#include <tester.h>
#include <logger.h>
#include <loggercommandcallwriter.h>

// this class is not a simple unit tests class, because it tests a class that is a 
// thread and has some side effects that needs to be ckecked, like the creations of 
// files and call of commands

class CommandCallWriterTests: public Tester{
public:
    CommandCallWriterTests();
    ~CommandCallWriterTests();

    vector<string> getContexts() override;
    void run(string context) override;
};

#endif
