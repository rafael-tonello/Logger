# About Logger

The Logger library is a lib written in C++ that is very simple to use and very flexible. It allows log messages to be sent to more than one place at the same time. You, for example, send the same message to a file and to stdout. The logging library achieves this through drivers, which are specified in the class instantiation.

Each driver is a class that must provide a suitable destination for log messages. The library currently comes with three built-in drivers: One for writing to stdou, one for writing to file and one for intercepting messages via lambada functions.

You can either use the lambda driver to give the information a personalized destination or write your own driver for this. This will be seen below.

The simplest way to use the library is to simply instantiate it and specify one or more drivers for it.
In the example below, an instance of the library is created that writes data to stdout.


```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.info("main", "this is a log");
```

on line 1 there is the instance of the Logger class.
on line 2, still in the Logger class instance, we inform the default driver for writing to stdout.
on line 5, we use the Info method to generate a log of information. We'll see more about logging methods below.

Good! Having made the initial presentations, below are some interesting topics on how to use and make better use of the library.

# Functions for log

The Logger library uses some functions to facilitate information logging. They are: log, debug, info, warning error and critical. All of these functions receive a string or a vector of strings with the data that will be saved in the logs. There is also a version for each of these functions that uses the DynamicVar type, allowing greater flexibility when generating logs. The DynamicVar type is a class that has a separate repository and you can find out more about it here: https://github.com/rafael-tonello/DynamicVar

## log function
This is the main function for data logging and receives a logLevel, which is an int indicating your log level, a name for your log and the data (or vector of data) to be stored.

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.log(LOGGER_LOGLEVEL_INFO, "examples", "an example log");
//all vector data will be merge in one log line
logger.log(LOGGER_LOGLEVEL_INFO, "examples", {"a ", "vector", "of", "data"});
logger.log(LOGGER_LOGLEVEL_INFO, "a log with default log name");
```

> Note about the log level: The log level is an integer that indicates the criticality (or importance) of the log. The Logger bibiote already has 5 standard log levels, namely: Debug (log level 1), Info (log level 2), Warning (log level 3), Error (log level 4) and Critical (log level 4). Drivers normally receive a log level in their construction, indicating that only log levels greater than or equal to it will be considered valid and the rest will be discarded.
>
> Note about the log name: The log name is a name that is used to identify modules and groups of logs. It is usually added by drivers at the beginning of the log lines, along with the date and time and the log level. See the example below to better understand what the log name is.
> ```log
> [2022-05-01T00:02:03-0300] [INFO] [HttpServerService] new data requested to the server
> ```
> In the example above, the log name is "HttpServerService"



## debug function
The "debug" function is a function that facilitates the generation of logs with level 1 (LOGGER_LOGLEVEL_DEBUG).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.debug("examples", "an example log");
//all vector data will be merge in one log line
logger.debug("examples", {"a ", "vector", "of", "data"});
logger.debug("a log with default log name");
```

## info function
The "info" function is a function that, like the debug function, facilitates the generation of logs, but with level 2 (LOGGER_LOGLEVEL_INFO).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.info("examples", "an example log");
//all vector data will be merge in one log line
logger.info("examples", {"a ", "vector", "of", "data"});
logger.info("a log with default log name");
```

## warning function
The "warning" function is a function that facilitates the generation of logs with level 3 (LOGGER_LOGLEVEL_WARNING).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.warning("examples", "an example log");
//all vector data will be merge in one log line
logger.warning("examples", {"a ", "vector", "of", "data"});
logger.warning("a log with default log name");
```

## error function
The "error" function is a function that facilitates the generation of logs with level 4 (LOGGER_LOGLEVEL_ERROR).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.error("examples", "an example log");
//all vector data will be merge in one log line
logger.error("examples", {"a ", "vector", "of", "data"});
logger.error("a log with default log name");
```

## critical function
The "critical" function is a function that facilitates the generation of logs with level 5 (LOGGER_LOGLEVEL_CRITICAL).

```c++
Logger logger({
	new LoggerConsoleWriter()
});

logger.critical("examples", "an example log");
//all vector data will be merge in one log line
logger.critical("examples", {"a ", "vector", "of", "data"});
logger.critical("a log with default log name");
```
The critical function has an additional and optional parameter (raiseException) that allows you to automatically throw an exception as soon as the log is logged. The exception thrown is a "runtime_error" which contains the log text itself as "wath".

# TODO LISTS
## general
```
	[x] Create a flush function to wait cache clear
  	[x] Remove exception from critical function 
	[ ] Critical function must flush library
```

## tests
```
    [ ] logger
    [ ] logger->LoggerConsoleWriter
    [ ] logger->LoggerFileWriter
    [ ] logger->LoggerLambdaWriter
```