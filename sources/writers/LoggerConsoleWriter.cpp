#include  "LoggerConsoleWriter.h" 
 
LoggerConsoleWriter::LoggerConsoleWriter(int logLevel, bool useColors, bool displayLinePrefix, bool displayDateTime)
{
    this->colors = useColors;
    this->logLevel = logLevel;
    this->displayLinePrefix = displayLinePrefix;
    this->displayDateTime = displayDateTime;
}
 
void LoggerConsoleWriter::write(Logger* sender, string msg, int level, string name){

    if (level >= logLevel)
    {
        if (displayLinePrefix)
        {
            auto ls = Logger::generateLineBegining(sender, level, name, displayDateTime);
            auto identPrefix = string(ls.size(), ' ');
            msg = Logger::identLog(msg, identPrefix);

            msg = ls + msg;
        }

        if (sender->intercepCoutCerrAndCLog)
        {
            //sender->restoreStdout();
            string s;

            if (level == LOGGER_LOGLEVEL_WARNING)
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
            else
            {
                s =  msg + "\n";
                (*(sender->cout_originalBuffer)).sputn(s.c_str(), s.size()); 
            }


            //cout.flush();
            //cerr.flush();
            //cout.clear();
            //cerr.clear();
            //sender->interceptStdout();        

        }
        else
        {

        
            if (level == LOGGER_LOGLEVEL_WARNING)
                cout << "\e[0;33m" << msg << "\e[0m" << endl; 
            else if (level == LOGGER_LOGLEVEL_ERROR)
                cerr << "\e[0;31m" << msg << "\e[0m" << endl;
            else if (level == LOGGER_LOGLEVEL_CRITICAL)
                cerr << "\e[0;31m" << msg << "\e[0m" << endl;
            //else if (level == Logger::LOGGER_INFO_LEVEL)	
            //	cout << "\e[0;34m" << msg << "\e[0m" << endl;
            else if (level == LOGGER_LOGLEVEL_DEBUG)
                cout << "\e[0;35m" << msg << "\e[0m" << endl;
            else
                cout << msg << endl;
        }

        
        
    }

}

/*
void LoggerConsoleWriter::write(Logger* sender, string msg, int level, string name){
    
    

    if (level >= logLevel)
    {
        

        if (displayLinePrefix)
        {
            auto ls = Logger::generateLineBegining(sender, level, name, displayDateTime);
            auto identPrefix = string(ls.size(), ' ');
            msg = Logger::identLog(msg, identPrefix);

            msg = ls + msg;
        }


        if (sender->intercepCoutCerrAndCLog)
            sender->restoreStdout();

        
        if (level == LOGGER_LOGLEVEL_WARNING)
            cout << "\e[0;33m" << msg << "\e[0m" << endl; 
        else if (level == LOGGER_LOGLEVEL_ERROR)
            cerr << "\e[0;31m" << msg << "\e[0m" << endl;
        else if (level == LOGGER_LOGLEVEL_CRITICAL)
            cerr << "\e[0;31m" << msg << "\e[0m" << endl;
        //else if (level == Logger::LOGGER_INFO_LEVEL)	
        //	cout << "\e[0;34m" << msg << "\e[0m" << endl;
        else if (level == LOGGER_LOGLEVEL_DEBUG)
        	cout << "\e[0;35m" << msg << "\e[0m" << endl;
        else
            cout << msg << endl;

        
        if (sender->intercepCoutCerrAndCLog)
        {
            cout.flush();
            cerr.flush();

            cout.clear();
            cerr.clear();
            sender->interceptStdout();        
        }
    }

}
*/