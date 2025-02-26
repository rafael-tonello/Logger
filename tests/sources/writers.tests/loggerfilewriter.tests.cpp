#include "loggerfilewriter.tests.h"

vector<string> LoggerFileWriterTests::getContexts(){
    return {
        "LoggerFileWriter"
    };
}

void LoggerFileWriterTests::run(string context){
    if(context != "LoggerFileWriter")
        return;
        
    this->test("Should compact the log file when it reaches the maximum size", [](){
        string workingFile = "/tmp/logger_test_working_file";
        
        auto writer = new LoggerFileWriter("/tmp/logger_test.log", LOGGER_LOG_ALL_LEVELS, true, 100);
        string longMessage = "This is a long message";

        auto logger = new Logger({writer});

        for (int i = 0; i < 100; i++)
        {
            writer->write(logger, longMessage, LOGGER_LOGLEVEL_INFO, "LoggerFileWriterTests", Logger::getRawTime());
        }

        delete writer;

        //check if a folder named /tmp/logger_test.log.history was created (do not use Utils)
        ifstream f("/tmp/logger_test.log.history");
        assertTrue(fs::exists("/tmp/logger_test.log.history") && fs::is_directory("/tmp/logger_test.log.history"));

        //check if exists at least one file in /tmp/logger_test.log.history (do not use Utils)
        assertTrue(fs::directory_iterator("/tmp/logger_test.log.history") != fs::directory_iterator("/tmp/logger_test.log.history"));
        
        return true;
    });    

}