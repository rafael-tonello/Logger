#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include <DynamicVar.h>
#include "tester.h"
#include <Logger.tests.h>
#include <writers.tests/commandcallwriter.tests.h>
#include <writers.tests/loggerfilewriter.tests.h>

using namespace std;
int main(int argc, char* argv[])
{
    return Tester::runTests({
        new LoggerTests(),
        new CommandCallWriterTests(),
        new LoggerFileWriterTests()
    }, argc, argv, "Logger library");
}



 