# About Logger

The Logger library is a simple and easy-to-use library for logging information in C++ projects. It is a library that allows you to log information in a simple and easy way, without the need to worry about the details of how to write the logs to the console, file or any other output. The library is designed to be extensible, allowing you to create your own drivers to write logs to any output you want.

You can use the source code directly in your C++ project or use the shared library that is generated in the build process. The shared library is in projects/library folder. In the folder projects/ you also find a Wrapper for GoLang.

To achieve the extensibility of the library, the Logger class make use of the Writer objects. Writer object are instances of classes that implement the LoggerWriter interface. This interface has only one method, the write method, which receiveis data about the log to be written. The Logger class has a list of Writer objects that are used to write the logs. The library already has some default writers, such as the LoggerConsoleWriter, which writes the logs to the console, and the LoggerFileWriter, which writes the logs to a file.

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
on line 5, we use the Info method to generate a log of information. We'll see more about logging methods below.

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

# Other cool functions for log (derivations of the 'log' function)
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