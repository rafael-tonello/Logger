#ifndef __LOGGERFILEWRITER_TESTS__H__ 
#define __LOGGERFILEWRITER_TESTS__H__ 

#include <tester.h>
#include <loggerfilewriter.h>
#include <vector>
#include <filesystem>


using namespace std;
namespace fs = std::filesystem;
class LoggerFileWriterTests: public Tester{
/* Tester interface */
public:

    vector<string> getContexts() override;
    void run(string context) override;

};

#endif
