#include  "LoggerConsoleWriter.h" 
 
LoggerConsoleWriter::LoggerConsoleWriter(int logLevel, bool useColors, bool displayLinePrefix, bool displayDateTime, bool printMilisseconds)
{
    this->colors = useColors;
    this->logLevel = logLevel;
    this->displayLinePrefix = displayLinePrefix;
    this->displayDateTime = displayDateTime;
    this->printMilisseconds = printMilisseconds;
}

LoggerConsoleWriter::~LoggerConsoleWriter()
{

}
 
void LoggerConsoleWriter::write(Logger* sender, string msg, int level, string name, std::time_t dateTime){
    if (level >= logLevel)
    {
        if (displayLinePrefix)
        {
            auto ls = sender->generateLineBegining(sender, level, name, displayDateTime, dateTime, printMilisseconds);
            auto identPrefix = string(ls.size(), ' ');
            msg = Logger::identLog(msg, identPrefix);

            msg = ls + msg;
        }

        if (sender->intercepCoutCerrAndCLog)
        {
            
            string s;

            if (level == LOGGER_LOGLEVEL_INFO2)
            {
                s = "\e[0;36m" + msg + "\e[0m" + "\n";
                (*(sender->cout_originalBuffer)).sputn(s.c_str(), s.size()); 
            }
            else if (level == LOGGER_LOGLEVEL_WARNING)
            {
                s = "\e[0;33m" + msg + "\e[0m" + "\n";
                (*(sender->cout_originalBuffer)).sputn(s.c_str(), s.size()); 
            }
            else if (level == LOGGER_LOGLEVEL_ERROR || level == LOGGER_LOGLEVEL_CRITICAL)
            {
                s = "\e[0;31m" + msg + "\e[0m" + "\n";
                (*(sender->cerr_originalBuffer)).sputn(s.c_str(), s.size()); 
            }
            else if (level == LOGGER_LOGLEVEL_DEBUG)
            {
                s = "\e[0;35m" + msg + "\e[0m" + "\n";
                (*(sender->cout_originalBuffer)).sputn(s.c_str(), s.size()); 
            }
            else if (level == LOGGER_LOGLEVEL_DEBUG2 || level == LOGGER_LOGLEVEL_TRACE)
            {
                s = "\e[0;90m" + msg + "\e[0m" + "\n";
                (*(sender->cout_originalBuffer)).sputn(s.c_str(), s.size()); 
            }

            else
            {
                s =  msg + "\n";
                (*(sender->cout_originalBuffer)).sputn(s.c_str(), s.size()); 
            }


        }
        else
        {

        
            if (level == LOGGER_LOGLEVEL_INFO2)
                cout << "\e[0;36m" << msg << "\e[0m" << endl; 
            else if (level == LOGGER_LOGLEVEL_WARNING)
                cout << "\e[0;33m" << msg << "\e[0m" << endl; 
            else if (level == LOGGER_LOGLEVEL_ERROR)
                cerr << "\e[0;31m" << msg << "\e[0m" << endl;
            else if (level == LOGGER_LOGLEVEL_CRITICAL)
                cerr << "\e[0;31m" << msg << "\e[0m" << endl;
            //else if (level == Logger::LOGGER_INFO_LEVEL)	
            //	cout << "\e[0;34m" << msg << "\e[0m" << endl;
            else if (level == LOGGER_LOGLEVEL_DEBUG)
                cout << "\e[0;35m" << msg << "\e[0m" << endl;
            else if (level == LOGGER_LOGLEVEL_DEBUG2 || level == LOGGER_LOGLEVEL_TRACE)
                cout << "\e[0;90m" << msg << "\e[0m" << endl;
            else
                cout << msg << endl;
        }

        
        
    }

}