package main

import (
	"rafaeltonello/logger/logger"
)

func main() {
	logger := logger.NewLoggerWithCustomLibPath([]logger.ILogWriter{
		logger.NewConsoleWriter(0, true, true, true, true),
		logger.NewFileWriter(0, "log.log"),
	}, "../../library/build/logger.so")
	log := logger.GetNamedLogger("main")

	log.Trace("Hello world")
	log.Debug("Debug message")
	log.Debug2("Debug2 message")
	log.Info("Hello world")
	log.Info2("Hello world")
	log.Warning("Debug2 message")
	log.Error("Hello world")
	log.Critical("Hello world")

	logger.Finalize()
}
