#ifndef _DYNAMIC_VAR_TESTS_
#define _DYNAMIC_VAR_TESTS_

#include <tester.h>
#include <DynamicVar.h>

class DynamicVarTests: public Tester{
private:
    void testConstructors();
    void testSets();
    void testGets();
    void testSetUsingAttribuitions();
    void testGetUsingAttribuitions();
    void testPointers();
    void testeSerializable();
public:
    vector<string> getContexts();
    void run(string context);
};

#endif
