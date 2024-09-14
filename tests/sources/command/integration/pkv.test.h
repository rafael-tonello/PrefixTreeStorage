#ifndef __HTTP_TEST__H__ 
#define __HTTP_TEST__H__ 

#include <tester.h>
#include <string>
#include <utils.h>

using namespace std;

class Pkv_test: public Tester { 
private:
    string pkvPath = "../../command/build/pkv";
    void compileCommand();
    void testSet();
    void testGet();
    void testDelete();
    void testHttpKeepAlive();
    void testHttpSet();
    void testHttpGet();
    void testHttpDelete();
    void testHttpPidDependency();
    string runScript(string lines, string name);
public:
    /* Tester implementation */
    vector<string> getContexts();
    void run(string context);
}; 

 
#endif 
