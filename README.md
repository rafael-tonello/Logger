# About Logger

The Logger library is a simple and easy-to-use library for logging information in projects written in C++, GoLang and others. It is a library that allows you to log information in a simple and easy way, without the need to worry about the details of how to write the logs to the console, file or any other output. The library is designed to be extensible, allowing you to create your own drivers to write logs to any output you want.

You can use the source code directly in your C++ project or use the shared library that is generated in the build process. The shared library is in projects/library folder. In the folder projects/ you also find a Wrapper for GoLang.

To achieve the extensibility of the library, the Logger class make use of Writer objects. Writer objects are instances of classes that implement the LoggerWriter interface. This interface has only one method, the write method, which receiveis data about the log to be written. The Logger class has a list of Writer objects that are used to write the logs. The library already has some default writers, such as the LoggerConsoleWriter, which writes the logs to the console, and the LoggerFileWriter, which writes the logs to a file.

# How to use (C++ example)

```c++
//create a logger instance with a console writer
Logger logger({
    new LoggerConsoleWriter()
});

//generate a log of information
logger.log(LOGGER_LOGLEVEL_INFO, "main", "this is a log");
```

on line 1 there is the instance of the Logger class.

on line 2, still in the Logger class instance, we inform the default driver for writing to stdout.

on line 5, we use the Info method to generate a log of information. 

Good! Having made the initial presentations, below are some interesting topics on how to use and make better use of the library.

## About the 'log' function and log levels
### log function
This is the main function for data logging and receives a logLevel, which is an int indicating your log level, a name for your log and the data (or vector of data) to be stored.

### Log levels
The Logger library has some predefined log levels. These log levels are constantes used to indicade to the 'log' function the criticality of the log. The log levels are:

```c++
#define LOGGER_LOGLEVEL_TRACE 9
#define LOGGER_LOGLEVEL_DEBUG2 10
#define LOGGER_LOGLEVEL_DEBUG 20
#define LOGGER_LOGLEVEL_INFO2 30
#define LOGGER_LOGLEVEL_INFO 40
#define LOGGER_LOGLEVEL_WARNING 50
#define LOGGER_LOGLEVEL_ERROR 60
#define LOGGER_LOGLEVEL_CRITICAL 70
```

# derivations of the 'log' function
The logs are generated when 'log' method is called. This method receives a log level, a log name and the data to be logged. This is a very nice and simple to create log lines, but it can be a little verbose. To facilitate the generation of logs, the library has some functions that facilitate the generation of logs with predefined log levels. These functions are: debug, info, warning, error and critical. Each of these functions receives a log name and the data to be logged. The log level is predefined for each of these functions, being respectively: LOGGER_LOGLEVEL_DEBUG, LOGGER_LOGLEVEL_INFO, LOGGER_LOGLEVEL_WARNING, LOGGER_LOGLEVEL_ERROR and LOGGER_LOGLEVEL_CRITICAL.

```c++
Logger logger({
    new LoggerConsoleWriter()
});

logger.info("examples", "an example log");
//all vector data will be merge in one log line
logger.debug("examples", {"a ", "vector", "of", "data"});
logger.warning("a log with default log name");
```

# Shared library
The Logger library can be used as a shared library. The shared library lies in the projects/library folder and allow to use the Logger library in another languages, such as C, GoLang, Python, etc.

is in the projects/library folder. To use the shared library, you need to include the Logger.h header file in your project and link the shared library to your project. Below is an example of how to use the shared library in a C++ project.

Almost all the functions of the shared library returns a char*. This return will be NULL if the function was executed successfully. If the function was not executed successfully, the return will be a string with an error message.

The shared library interface is:

```c
//creates a new logger instance. It is importante to setup the logger before using it. The setup is made by 
//the functions logger_setup_addOutputFile, logger_setup_addConsoleOutput and done by logger_setup_done function
//logger_new_logger returns a ulong that is the instance of the logger and must be used in all other functions
extern "C" ulong logger_new_logger();

//add a file output to the logger. The filename is the name of the file that will be used to write the logs.
//internally, creates a new instance of LoggerFileWriter
extern "C" const char* logger_setup_addOutputFile(ulong instance, char* filename, int logLevel);

//add a console output to the logger. The useColors parameter indicates if the output should use colors. The displayLinePrefix
//parameter indicates if the line prefix should be displayed. The displayDateTime parameter indicates if the date and time should be displayed.
//The printMilisseconds parameter indicates if the milisseconds should be printed.
//The line prefix is the 'header of the line' and is composed by the date and time, the log level and the log name.
extern "C" const char* logger_setup_addConsoleOutput(ulong instance, int logLevel, bool useColors, bool displayLinePrefix, bool displayDateTime, bool printMilisseconds);

//finalize the logger setup. This function must be called after all addOutput functions
extern "C" const char* logger_setup_done(ulong instance);

//Flush internal caches and fianlize writers. Alfo free the logger instance and all writers
extern "C" const char* logger_free_logger(ulong instance);

//log a message with a log level. The log level is an int that indicates the criticality of the log. The 
//name is the name of the log and the msg is the message to be logged.
extern "C" const char* logger_log(ulong instance, int level, char* name, char* msg);

//Logs a trace message. Internally redirects to logger_log with LOGGER_LOGLEVEL_TRACE
extern "C" const char* logger_trace(ulong instance, char* name, char* msg);

//Logs a debug2 message. Internally redirects to logger_log with LOGGER_LOGLEVEL_DEBUG2
extern "C" const char* logger_debug2(ulong instance, char* name, char* msg);

//Logs a debug message. Internally redirects to logger_log with LOGGER_LOGLEVEL_DEBUG
extern "C" const char* logger_debug(ulong instance, char* name, char* msg);

//Logs a info2 message. Internally redirects to logger_log with LOGGER_LOGLEVEL_INFO2
extern "C" const char* logger_info2(ulong instance, char* name, char* msg);

//Logs a info message. Internally redirects to logger_log with LOGGER_LOGLEVEL_INFO
extern "C" const char* logger_info(ulong instance, char* name, char* msg);

//Logs a warning message. Internally redirects to logger_log with LOGGER_LOGLEVEL_WARNING
extern "C" const char* logger_warning(ulong instance, char* name, char* msg);

//Logs a error message. Internally redirects to logger_log with LOGGER_LOGLEVEL_ERROR
extern "C" const char* logger_error(ulong instance, char* name, char* msg);

//Logs a critical message. Internally redirects to logger_log with LOGGER_LOGLEVEL_CRITICAL
extern "C" const char* logger_critical(ulong instance, char* name, char* msg);

```

# Golang Wrapper
The Golang wrapper is a rich wrapper for the Logger library.


# Building all

Run the file build.sh. Build sh will build all projects and run all tests.

# TODO LISTS
## general
```
	[x] Create a flush function to wait cache clear
  	[x] Remove exception from critical function 
	[x] Critical function must flush library
	[x] Use custom timezones
	[x] Option to apply offset when using custom timezones
```

## tests
```
    [ ] logger
    [ ] logger->LoggerConsoleWriter
    [ ] logger->LoggerFileWriter
    [ ] logger->LoggerLambdaWriter
	[x] timezones
```