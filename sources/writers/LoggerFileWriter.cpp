#include  "LoggerFileWriter.h" 
 
LoggerFileWriter::LoggerFileWriter(string fname, int logLevel)
{
    this->logLevel = logLevel;
    file = ofstream(fname, ios::app);
    if (!file.is_open())
        throw runtime_error("Can't open log file to write");
    
}

LoggerFileWriter::~LoggerFileWriter()
{
    file.close();
}

void LoggerFileWriter::write(Logger* sender, string msg, int level, string name, std::time_t dateTime){
    if (level >= logLevel)
    {
        string lineHeader = Logger::generateLineBegining(sender, level, name, dateTime);
        
        auto identPrefix = string(lineHeader.size(), ' ');
        msg = Logger::identLog(msg, identPrefix);

        file << lineHeader << msg << endl;
    }
}


