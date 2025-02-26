#include  "LoggerLambdaWriter.h" 
 
LoggerLambdaWriter::LoggerLambdaWriter(function<void(ILogger* sender, string msg, int level, string name, std::time_t dateTime)> writerFunc)
{
    this->writerFunc = writerFunc;
}
	
void LoggerLambdaWriter::write(ILogger* sender, string msg, int level, string name, std::time_t dateTime)
{
    this->writerFunc(sender, msg, level, name, dateTime);
}