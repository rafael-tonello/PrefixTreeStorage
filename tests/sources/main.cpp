#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "tester.h"

#include "./command/integration/pkv.test.h"

using namespace std;
int main(int argc, char* argv[]){
    vector<Tester*> testers;

    //***** testers instances
    //***** make your changes only here
        testers.push_back(new Pkv_test());
        

        
    //*****

    return Tester::runTests(testers, argc, argv);
}