#include  "LoggerFileWriter.h" 
 
LoggerFileWriter::LoggerFileWriter(string fname, int logLevel, bool printMilisseconds, int maxFileSize)
{
    this->logLevel = logLevel;
    this->printMilisseconds = printMilisseconds;
    this->fileName = fname;
    file = ofstream(fname, ios::app);
    this->maxFileSize = maxFileSize;
    if (!file.is_open())
        throw runtime_error("Can't open log file to write");
    
}

LoggerFileWriter::~LoggerFileWriter()
{
    file.close();

    if (fileSizeCheckerThead != nullptr)
    {
        stopFileSizeCheckerThead=true;
        fileSizeCheckerTheadLocker.unlock();
        fileSizeCheckerThead->join();
    }
}

void LoggerFileWriter::write(Logger* sender, string msg, int level, string name, std::time_t dateTime){
    if (level >= logLevel)
    {
        string lineHeader = sender->generateLineBegining(sender, level, name, true, dateTime, this->printMilisseconds);
        
        auto identPrefix = string(lineHeader.size(), ' ');
        msg = ILogger::identLog(msg, identPrefix);

        fileLock.lock();
        file << lineHeader << msg << endl;
        fileLock.unlock();

        checkFileSize();
    }
}

void LoggerFileWriter::checkFileSize()
{
    if (maxFileSize <= 0)
        return;

    if (fileSizeCheckerThead == nullptr)
    {
        fileSizeCheckerThead = new thread([this](){
            while (!stopFileSizeCheckerThead)
            {
                bool runCompactation=false;
                fileSizeCheckerTheadLocker.lock();
                {
                    if (file.tellp() > maxFileSize)
                    {
                        //get datetime in the format YYYY-MM-DD-HH-MM-SS
                        auto now = chrono::system_clock::now();
                        auto in_time_t = chrono::system_clock::to_time_t(now);
                        auto time = localtime(&in_time_t);
                        string dateTimeString = to_string(time->tm_year + 1900) + "-" + to_string(time->tm_mon + 1) + "-" + to_string(time->tm_mday) + "-" + to_string(time->tm_hour) + "-" + to_string(time->tm_min) + "-" + to_string(time->tm_sec);

                        //separate the extension and the filename
                        string extension = "";
                        string fileNameWithNoExtension=fileName;
                        auto pos = fileName.find_last_of(".");
                        if (pos == string::npos)
                        {
                            extension = "."+fileName.substr(pos + 1);
                            fileNameWithNoExtension = fileName.substr(0, pos);
                        }

                        string fileNameWithoutExtensionAndWithoutPath = fileNameWithNoExtension;
                        pos = fileNameWithNoExtension.find_last_of("/");
                        if (pos != string::npos)
                            fileNameWithoutExtensionAndWithoutPath = fileNameWithNoExtension.substr(pos + 1);
                        
                        //move the file to a temporary file and create a new one for the logs
                        string newFname = fileNameWithNoExtension + "-" + dateTimeString + extension;
                        fileLock.lock();
                        if (file.is_open())
                            file.close();

                        rename(fileName.c_str(), newFname.c_str());
                        //create new file
                        file = ofstream(fileName, ios::app);
                        fileLock.unlock();

                        //create the destination folder if not exists
                        string command = "mkdir -p \"" + fileName + ".history\"";
                        system(command.c_str());

                        //compact the file
                        string tarXzFile = fileName + ".history/" +fileNameWithoutExtensionAndWithoutPath + "-" + dateTimeString + ".tar.xz";
                        command = "nice -n 19 tar -cJf \"" + tarXzFile + "\" \"" + newFname + "\"";
                        system(command.c_str());

                        //remove the temporary file
                        command = "rm \"" + newFname+"\"";
                        system(command.c_str());

                    }
                }
            }
        });
    }

    fileSizeCheckerTheadLocker.unlock();
}