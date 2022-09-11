#include  "LoggerLambdaWriter.h" 
 
LoggerLambdaWriter::LoggerLambdaWriter(function<void(Logger* sender, string msg, int level, string name)> writerFunc)
{
    this->writerFunc = writerFunc;
}
	
void LoggerLambdaWriter::write(Logger* sender, string msg, int level, string name)
{
    this->writerFunc(sender, msg, level, name);
}