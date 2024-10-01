package main

import (
	"fmt"
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
	log.Critical("Hello world", "with", "some", "arguments", "and the number", 42, "ant the logger itself", log)

	tmp := struct {
		Name  string
		Email string
	}{"Rafael", "rafaeltonello55@gmail.com"}

	log.Info("struct:", tmp)

	logger.Finalize()

	fmt.Print()
}
