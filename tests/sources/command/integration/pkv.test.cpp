#include  "pkv.test.h" 
 
/* Tester implementation */
vector<string> Pkv_test::getContexts()
{
    return { string("command.integration.compilation"), string("command.integration.commandinterface"), string("command.integration.httpinterface") };
}

string Pkv_test::runScript(string lines, string name){
    lines = "#!/bin/bash\n" + lines;
    string scriptPath = "./" + name + ".sh";
    Utils::writeTextFileContent(scriptPath, lines);
    Utils::ssystem("chmod +x " + scriptPath);
    string result = Utils::ssystem(scriptPath);
    
    //Utils::ssystem("rm -rf " + scriptPath);
    return result;
}

void Pkv_test::run(string context)
{
    if (context == string("command.integration.compilation")){
        this->compileCommand();
    }
    else if (context == string("command.integration.commandinterface")){
        this->testSet();
        this->testGet();
        this->testDelete();
    }
    else if (context == string("command.integration.httpinterface")){
        this->testHttpKeepAlive();
        this->testHttpSet();
        this->testHttpGet();
        this->testHttpDelete();
        this->testHttpPidDependency();
    }
}

void Pkv_test::compileCommand()
{
    this->test("Compile command", [&](){
        this->yellowMessage("Compiling command. This may take a while...");
        string script="";
        script += "rm -rf ../../command/build >/dev/null 2> /dev/null\n";
        script += "cd ../../command\n";

        script += "make all >>/dev/null 2>> /dev/null\n";
        script += "echo $?\n";

        //string result=runScript(script, "compileCommand");
        string result=Utils::ssystem(script);

        return TestResult{
            .result = result == "0",
            .expected = "0",
            .returned = result
        };
    });
}

void Pkv_test::testSet()
{
    this->test("Set key-value pair", [&](){
        string script="";
        script += this->pkvPath + " set key value -f:./testdb >/dev/null 2> /dev/null\n";
        script += "echo $?";

        //string result=runScript(script, "testSet");
        string result=Utils::ssystem(script);
        
        //remove the testdb file
        Utils::ssystem("rm -rf ./testdb");

        return TestResult{
            .result = result == "0",
            .expected = "0",
            .returned = result
        };
    });
}

void Pkv_test::testGet()
{
    this->test("Set key-value pair", [&](){
        string expected = "value to be returned";
        string script="";
        script += this->pkvPath + " set key \""+expected+"\" -f:./testdb2>/dev/null 2> /dev/null\n";
        script += this->pkvPath + " get key -f:./testdb2 \n";

        string result=runScript(script, "testGet");

        //remove the testdb2 file
        Utils::ssystem("rm -rf ./testdb2");

        return TestResult{
            .result = result == expected,
            .expected = expected,
            .returned = result
        };
    });

}

void Pkv_test::testDelete()
{
    this->test("Set key-value pair", [&](){
        string expected = "";
        string script="";
        script += this->pkvPath + " set key \"this value sould be deleted\" -f:./testdb3>/dev/null 2> /dev/null\n";
        script += this->pkvPath + " delete key -f:./testdb3 \n";
        script += this->pkvPath + " get key -f:./testdb3 \n";

        string result=runScript(script, "testGet");

        //remove the testdb2 file
        Utils::ssystem("rm -rf ./testdb3");

        return TestResult{
            .result = result == expected,
            .expected = expected,
            .returned = result
        };
    });
}

void Pkv_test::testHttpKeepAlive()
{
    this->test("Set key-value pair (via http interface)", [=](){
        //run pkv in http mode, in background, and get its PID
        thread th([=](){
            Utils::ssystem(this->pkvPath + " -H 30082 -f:./testdb6");
        });
        th.detach();
        usleep (1000000);

        //make an empty request to the server (should return 200)
        string result = Utils::ssystem("curl -s -o /dev/null -w \"%{http_code}\" http://localhost:30082/");

        //kill the server
        Utils::ssystem("killall pkv");

        //remote the temporary db file
        Utils::ssystem("rm -rf ./testdb6");

        return TestResult{
            .result = result == "204",
            .expected = "204",
            .returned = result
        };
    });
}

void Pkv_test::testHttpSet()
{
    this->test("Set key-value pair (via http interface)", [=](){
        //run pkv in http mode, in background, and get its PID
        thread th([=](){
            Utils::ssystem(this->pkvPath + " -H 30082 -f:./testdb7");
        });
        th.detach();
        usleep (1000000);

        //make a request to the server to set a key-value pair
        string result = Utils::ssystem("curl -s -o /dev/null -w \"%{http_code}\" -X \"POST\" -d \"value to be setted\" http://localhost:30082/set/this/key");

        //kill the server
        Utils::ssystem("killall pkv");

        //remote the temporary db file
        Utils::ssystem("rm -rf ./testdb7");

        return TestResult{
            .result = result == "200",
            .expected = "200",
            .returned = result
        };
    });
}

void Pkv_test::testHttpGet()
{
    this->test("Get key-value pair (via http interface)", [=](){
        string expected = "value to be returned";
        //run pkv in http mode, in background, and get its PID
        thread th([=](){
            Utils::ssystem(this->pkvPath + " -H 30082 -f:./testdb8");
        });
        th.detach();
        usleep (1000000);
        
        //make a request to the server to set a key-value pair
        Utils::ssystem("curl -s -o /dev/null -w \"%{http_code}\" -X \"POST\" -d \""+expected+"\" http://localhost:30082/the/key");

        //make a request to the server to get a key-value pair
        string result = Utils::ssystem("curl -s http://localhost:30082/the/key");

        //kill the server
        Utils::ssystem("killall pkv");

        //remote the temporary db file
        Utils::ssystem("rm -rf ./testdb8");

        return TestResult{
            .result = result == expected,
            .expected = expected,
            .returned = result
        };
    });

}

void Pkv_test::testHttpDelete()
{
    this->test("Get key-value pair (via http interface)", [=](){
        string expected = "";
        //run pkv in http mode, in background, and get its PID
        thread th([=](){
            Utils::ssystem(this->pkvPath + " -H 30082 -f:./testdb9");
        });
        th.detach();
        usleep (1000000);

        //make a request to the server to set a key-value pair
        Utils::ssystem("curl -s -o /dev/null -w \"%{http_code}\" -X \"POST\" -d \""+expected+"\" http://localhost:30082/the/key");

        //make a request to the server to get a key-value pair
        Utils::ssystem("curl -s -o /dev/null -w \"%{http_code}\" -X \"DELETE\" http://localhost:30082/the/key");

        //make a request to the server to get a key-value pair
        string result = Utils::ssystem("curl -s http://localhost:30082/the/key");

        //kill the server
        Utils::ssystem("killall pkv");

        //remote the temporary db file
        Utils::ssystem("rm -rf ./testdb9");

        return TestResult{
            .result = result == expected,
            .expected = expected,
            .returned = result
        };
    });
}

void Pkv_test::testHttpPidDependency()
{

}

