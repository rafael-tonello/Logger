package logger

//https://github.com/lxwagn/using-go-with-c-libraries

//https://stackoverflow.com/questions/2734719/how-to-compile-a-static-library-in-linux
//https://stackoverflow.com/questions/10454263/command-to-compile-c-files-with-a-files
//https://stackoverflow.com/questions/25084855/how-does-gcc-shared-option-affect-the-output

/* #cgo LDFLAGS: -ldl
#cgo CFLAGS:-I../../library/sources
#cgo LDFLAGS: -ldl
#include <stdlib.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>

typedef ulong (*logger_new_logger_t)();
typedef const char* (*logger_setup_addOutputFile_t)(ulong instance, char* filename, int logLevel);
typedef const char* (*logger_setup_addConsoleOutput_t)(ulong instance, int logLevel, int useColors, int displayLinePrefix, int displayDateTime, int printMilisseconds);
typedef const char* (*logger_setup_done_t)(ulong instance);
typedef const char* (*logger_free_logger_t)(ulong instance);
typedef const char* (*logger_log_t)(ulong instance, int level, char* name, char* msg);
typedef const char* (*logger_trace_t)(ulong instance, char* name, char* msg);
typedef const char* (*logger_debug2_t)(ulong instance, char* name, char* msg);
typedef const char* (*logger_debug_t)(ulong instance, char* name, char* msg);
typedef const char* (*logger_info2_t)(ulong instance, char* name, char* msg);
typedef const char* (*logger_info_t)(ulong instance, char* name, char* msg);
typedef const char* (*logger_warning_t)(ulong instance, char* name, char* msg);
typedef const char* (*logger_error_t)(ulong instance, char* name, char* msg);
typedef const char* (*logger_critical_t)(ulong instance, char* name, char* msg);

logger_setup_addOutputFile_t logger_setup_addOutputFile_p;
logger_new_logger_t logger_new_logger_p;
logger_setup_addOutputFile_t logger_setup_addOutputFile_p;
logger_setup_addConsoleOutput_t logger_setup_addConsoleOutput_p;
logger_setup_done_t logger_setup_done_p;
logger_free_logger_t logger_free_logger_p;
logger_log_t logger_log_p;
logger_trace_t logger_trace_p;
logger_debug2_t logger_debug2_p;
logger_debug_t logger_debug_p;
logger_info2_t logger_info2_p;
logger_info_t logger_info_p;
logger_warning_t logger_warning_p;
logger_error_t logger_error_p;
logger_critical_t logger_critical_p;



int libStarted = 0;
int init(char* soPath){
	if (libStarted){
		return 0;
	}

	libStarted = 1;

	//check if soPath is empty
	if(soPath == NULL || soPath[0] == '\0'){
		soPath = "logger.so";
	}

	void* libp = dlopen(soPath, RTLD_LAZY);
	if (libp == 0){
		printf("Failed to load library: %s\n", dlerror());
		return 1;
	}

	logger_new_logger_p = (logger_new_logger_t)dlsym(libp, "logger_new_logger");
	logger_setup_addOutputFile_p = (logger_setup_addOutputFile_t)dlsym(libp, "logger_setup_addOutputFile");
	logger_setup_addConsoleOutput_p = (logger_setup_addConsoleOutput_t)dlsym(libp, "logger_setup_addConsoleOutput");
	logger_setup_done_p = (logger_setup_done_t)dlsym(libp, "logger_setup_done");
	logger_free_logger_p = (logger_free_logger_t)dlsym(libp, "logger_free_logger");
	logger_log_p = (logger_log_t)dlsym(libp, "logger_log");
	logger_trace_p = (logger_trace_t)dlsym(libp, "logger_trace");
	logger_debug2_p = (logger_debug2_t)dlsym(libp, "logger_debug2");
	logger_debug_p = (logger_debug_t)dlsym(libp, "logger_debug");
	logger_info2_p = (logger_info2_t)dlsym(libp, "logger_info2");
	logger_info_p = (logger_info_t)dlsym(libp, "logger_info");
	logger_warning_p = (logger_warning_t)dlsym(libp, "logger_warning");
	logger_error_p = (logger_error_t)dlsym(libp, "logger_error");
	logger_critical_p = (logger_critical_t)dlsym(libp, "logger_critical");

	return 0;
}

ulong logger_new_logger()
{
	return logger_new_logger_p();
}

const char* logger_setup_addOutputFile(ulong instance, char* filename, int logLevel)
{
	return logger_setup_addOutputFile_p(instance, filename, logLevel);
}

const char* logger_setup_addConsoleOutput(ulong instance, int logLevel, int useColors, int displayLinePrefix, int displayDateTime, int printMilisseconds)
{
	return logger_setup_addConsoleOutput_p(instance, logLevel, useColors, displayLinePrefix, displayDateTime, printMilisseconds);
}

const char* logger_setup_done(ulong instance)
{
	return logger_setup_done_p(instance);
}

const char* logger_free_logger(ulong instance)
{
	return logger_free_logger_p(instance);
}

const char* logger_log(ulong instance, int level, char* name, char* msg)
{
	return logger_log_p(instance, level, name, msg);
}

const char* logger_trace(ulong instance, char* name, char* msg)
{
	return logger_trace_p(instance, name, msg);
}

const char* logger_debug2(ulong instance, char* name, char* msg)
{
	return logger_debug2_p(instance, name, msg);
}

const char* logger_debug(ulong instance, char* name, char* msg)
{
	return logger_debug_p(instance, name, msg);
}

const char* logger_info2(ulong instance, char* name, char* msg)
{
	return logger_info2_p(instance, name, msg);
}

const char* logger_info(ulong instance, char* name, char* msg)
{
	return logger_info_p(instance, name, msg);
}

const char* logger_warning(ulong instance, char* name, char* msg)
{
	return logger_warning_p(instance, name, msg);
}

const char* logger_error(ulong instance, char* name, char* msg)
{
	return logger_error_p(instance, name, msg);
}

const char* logger_critical(ulong instance, char* name, char* msg)
{
	return logger_critical_p(instance, name, msg);
}
*/
import "C"

type Logger struct {
	instance C.ulong
}

type NamedLogger struct {
	name   string
	logger *Logger
}

type ILogWriter interface {
	doRegistration(onstance C.ulong)
}

type ILogWriterBase struct {
	Loglevel int
}

type LoggerConsoleWriter struct {
	ILogWriterBase

	UseColors         bool
	DisplayLinePrefix bool
	DisplayDateTime   bool
	PrintMilisseconds bool
}

type LoggerFileWriter struct {
	ILogWriterBase

	FileName string
}

var libStarted bool = false

func NewLogger(writers []ILogWriter) Logger {
	return NewLoggerWithCustomLibPath(writers, "logger.so")
}
func NewLoggerWithCustomLibPath(writers []ILogWriter, CustomDlPath string) Logger {
	if !libStarted {
		libStarted = true
		initResult := C.init(C.CString(CustomDlPath))
		if initResult != 0 {
			panic("Failed to initialize logger library")
		}
	}

	instance := C.logger_new_logger()

	for _, writer := range writers {
		writer.doRegistration(instance)
	}

	C.logger_setup_done(instance)

	return Logger{instance}
}

func NewNamedLogger(name string, logger *Logger) NamedLogger {
	return NamedLogger{name, logger}
}

func NewConsoleWriter(logLevel int, useColors bool, displayLinePrefix bool, displayDateTime bool, printMilisseconds bool) LoggerConsoleWriter {
	return LoggerConsoleWriter{ILogWriterBase{logLevel}, useColors, displayLinePrefix, displayDateTime, printMilisseconds}
}

func NewFileWriter(logLevel int, fileName string) LoggerFileWriter {
	return LoggerFileWriter{ILogWriterBase{logLevel}, fileName}
}

//#region ILoggerWriters

func (writer LoggerConsoleWriter) doRegistration(instance C.ulong) {
	useColors := 0
	if writer.UseColors {
		useColors = 1
	}

	displayLinePrefix := 0
	if writer.DisplayLinePrefix {
		displayLinePrefix = 1
	}

	printMilisseconds := 0
	if writer.PrintMilisseconds {
		printMilisseconds = 1
	}

	displayDateTime := 0
	if writer.DisplayDateTime {
		displayDateTime = 1
	}

	C.logger_setup_addConsoleOutput(instance, C.int(writer.Loglevel), C.int(useColors), C.int(displayLinePrefix), C.int(displayDateTime), C.int(printMilisseconds))
}

func (writer LoggerFileWriter) doRegistration(instance C.ulong) {
	C.logger_setup_addOutputFile(instance, C.CString(writer.FileName), C.int(writer.ILogWriterBase.Loglevel))
}

//#endregion ILoggerWriters

// #region Logger {
func (logger *Logger) Finalize() {
	C.logger_free_logger(logger.instance)
}

func (logger *Logger) GetNamedLogger(name string) NamedLogger {
	return NamedLogger{name, logger}
}

func (logger *Logger) Log(level int, name, msg string) string {
	return C.GoString(C.logger_log(logger.instance, C.int(level), C.CString(name), C.CString(msg)))
}

func (logger *Logger) Trace(name, msg string) string {
	return C.GoString(C.logger_trace(logger.instance, C.CString(name), C.CString(msg)))
}

func (logger *Logger) Debug2(name, msg string) string {
	return C.GoString(C.logger_debug2(logger.instance, C.CString(name), C.CString(msg)))
}

func (logger *Logger) Debug(name, msg string) string {
	return C.GoString(C.logger_debug(logger.instance, C.CString(name), C.CString(msg)))
}

func (logger *Logger) Info2(name, msg string) string {
	return C.GoString(C.logger_info2(logger.instance, C.CString(name), C.CString(msg)))
}

func (logger *Logger) Info(name, msg string) string {
	return C.GoString(C.logger_info(logger.instance, C.CString(name), C.CString(msg)))
}

func (logger *Logger) Warning(name, msg string) string {
	return C.GoString(C.logger_warning(logger.instance, C.CString(name), C.CString(msg)))
}

func (logger *Logger) Error(name, msg string) string {
	return C.GoString(C.logger_error(logger.instance, C.CString(name), C.CString(msg)))
}

func (logger *Logger) Critical(name, msg string) string {
	return C.GoString(C.logger_critical(logger.instance, C.CString(name), C.CString(msg)))
}

//#endregion Logger

// #region named logger
func (nlogger *NamedLogger) Log(level int, msg string) string {
	return nlogger.logger.Log(level, nlogger.name, msg)
}

func (nlogger *NamedLogger) Trace(msg string) string {
	return nlogger.logger.Trace(nlogger.name, msg)
}

func (nlogger *NamedLogger) Debug2(msg string) string {
	return nlogger.logger.Debug2(nlogger.name, msg)
}

func (nlogger *NamedLogger) Debug(msg string) string {
	return nlogger.logger.Debug(nlogger.name, msg)
}

func (nlogger *NamedLogger) Info2(msg string) string {
	return nlogger.logger.Info2(nlogger.name, msg)
}

func (nlogger *NamedLogger) Info(msg string) string {
	return nlogger.logger.Info(nlogger.name, msg)
}

func (nlogger *NamedLogger) Warning(msg string) string {
	return nlogger.logger.Warning(nlogger.name, msg)
}

func (nlogger *NamedLogger) Error(msg string) string {
	return nlogger.logger.Error(nlogger.name, msg)
}

func (nlogger *NamedLogger) Critical(msg string) string {
	return nlogger.logger.Critical(nlogger.name, msg)
}

//#endregion named logger
