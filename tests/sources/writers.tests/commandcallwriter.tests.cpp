#include "commandcallwriter.tests.h"

CommandCallWriterTests::CommandCallWriterTests()
{
    
}

CommandCallWriterTests::~CommandCallWriterTests()
{
    
}

vector<string> CommandCallWriterTests::getContexts()
{
    return { "CommandCallWriter" };
}

void CommandCallWriterTests::run(string context)
{
    if (context != "CommandCallWriter")
        return;

    //this->test("Overall test - should call the command with a log data file", [](){
    //    string workingFile = "/tmp/logger_test_working_file";
    //    string shFile=
    //        string("#!/bin/bash\n")+
    //        string("fname=\"$1\"\n")+
    //        string("echo \"$fname\" >>/tmp/fname\n")+
//
    //        string("mv \"$fname\" \""+workingFile+"\"\n");
    //    
    //    LoggerUtils::writeTextFileContent("/tmp/logger_test.sh", shFile);
    //    LoggerUtils::ssystem("chmod +x /tmp/logger_test.sh");
    //    auto writer = new LoggerCommandCallWriter("/tmp/logger_test.sh");
    //    auto logger = Logger({writer});
//
    //    logger.info(DVV{"This is a info message"});
    //    logger.warning(DVV{"This is a warning message"});
//
    //    //check if file exists (do not use Utils)
    //    ifstream f(workingFile);
    //    assertTrue(f.good(), "The file cache file was not created");
//
    //    return true;
    //});
//
}
