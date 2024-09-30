#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "tester.h"
#include <Logger.tests.h>

using namespace std;
int main(int argc, char* argv[])
{
   
    vector<Tester*> testers;



    //***** testers instances
    //***** make your changes only here
        testers.push_back(new LoggerTests());
        // testers.push_back(new SocketInfoTester());
        // testers.push_back(new PhomauTester());
        // testers.push_back(new DependencyInjectionManagerTester());
        // testers.push_back(new SimpleConfFileProviderTester());
        // testers.push_back(new ConfsTester());
        // testers.push_back(new Controller_ClientHelperTester());
        // testers.push_back(new Controller_VarHelperTester());
        // testers.push_back(new ControllerTester());

    //*****

    auto ret = Tester::runTests(testers, argc, argv);
    cout << flush;
    return ret;
}