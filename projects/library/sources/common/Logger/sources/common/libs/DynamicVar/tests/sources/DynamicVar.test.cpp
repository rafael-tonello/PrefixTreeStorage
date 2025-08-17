
#include "DynamicVar.test.h"

vector<string> DynamicVarTests::getContexts()
{
    return { "DynamicVarTests" };

}

void DynamicVarTests::run(string context)
{
    if (context != "DynamicVarTests")
        return;

    testConstructors();
    testSets();
    testGets();
    testSetUsingAttribuitions();
    testGetUsingAttribuitions();
    testPointers();
    testeSerializable();
}

void DynamicVarTests::testConstructors()
{
    this->blueMessage("Constructors tests");
    this->test("DynamicVar empty contructor should let internal value = \"\"", []{
        DynamicVar dv;

        return dv.getString() == "";
    });

    this->test("DynamicVar 'char const*' contructor should set internal value to 'sample text'", []{
        const char* value = "sample text";
        DynamicVar dv(value);

        string expected(value);
        string received = dv.getString();

        return TestResult{
            expected == received,
            expected,
            received
        };
    });

    this->test("DynamicVar 'string' contructor should set internal value to 'this is a test'", []{
        string value = "this is a test";
        DynamicVar dv(value);

        string received = dv.getString();

        return TestResult{
            value == received,
            value,
            received
        };
    });

    this->test("DynamicVar 'int' contructor should set internal value to '1092'", []{
        int value = 1092;
        DynamicVar dv(value);

        string received = dv.getString();

        return TestResult{
            std::to_string(value) == received,
            std::to_string(value),
            received
        };
    });

    this->test("DynamicVar 'int' contructor should NOT set internal value to '10000000000' (ten bilion)", []{
        int value = 10000000000;
        DynamicVar dv(value);

        string received = dv.getString();

        return TestResult{
            "10000000000" != received,
            "!= 10000000000",
            received
        };
    });

    this->test("DynamicVar 'int64_t' contructor should set internal value to '100000000000' (100 bilion)", []{
        int64_t value = 100000000000;
        DynamicVar dv(value);

        string received = dv.getString();

        return TestResult{
            std::to_string(value) == received,
            std::to_string(value),
            received
        };
    });

    this->test("DynamicVar 'double' contructor should set internal value to '3874.5601'", []{
        double value = 3874.560100000000f;
        DynamicVar dv(value);

        double received = dv.getDouble();

        return TestResult{
            value == received,
            to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar 'bool' contructor should set internal value to '1'", []{
        bool value = true;
        DynamicVar dv(value);

        string received = dv.getString();

        return TestResult{
            "1" == received,
            "1",
            received
        };
    });
}

void DynamicVarTests::testSets()
{
    this->blueMessage("Set* functions tests");

    this->test("DynamicVar::setSstring method should set internal value to 'this is a test'", []{
        string value = "this is a test";
        DynamicVar dv;
        dv.setString(value);

        string received = dv.getString();

        return TestResult{
            value == received,
            value,
            received
        };
    });

    this->test("DynamicVar::setInt method should set internal value to '1092'", []{
        int value = 1092;
        DynamicVar dv;
        dv.setInt(value);

        string received = dv.getString();

        return TestResult{
            std::to_string(value) == received,
            std::to_string(value),
            received
        };
    });

    this->test("DynamicVar::setInt methodshould NOT set internal value to '10000000000' (ten bilion)", []{
        int value = 10000000000;
        DynamicVar dv;
        dv.setInt(value);

        string received = dv.getString();

        return TestResult{
            "10000000000" != received,
            "!= 10000000000",
            received
        };
    });

    this->test("DynamicVar::setInt64 method should set internal value to '100000000000' (100 bilion)", []{
        int64_t value = 100000000000;
        DynamicVar dv;
        dv.setInt64(value);

        string received = dv.getString();

        return TestResult{
            std::to_string(value) == received,
            std::to_string(value),
            received
        };
    });

    this->test("DynamicVar::setDouble method should set internal value to '3874.5601'", []{
        double value = 3874.5601000000000f;
        DynamicVar dv;
        dv.setDouble(value);

        double received = dv.getDouble();

        return TestResult{
            value == received,
            to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar::setBool method should set internal value to '1'", []{
        bool value = true;
        DynamicVar dv;
        dv.setBool(value);

        string received = dv.getString();

        return TestResult{
            "1" == received,
            "1",
            received
        };
    });
}

void DynamicVarTests::testGets()
{
    this->blueMessage("Get* functions tests");

    this->test("DynamicVar::getSstring method should receive the value 'this is a test'", []{
        string value = "this is a test";
        DynamicVar dv;
        dv.setString(value);

        string received = dv.getString();

        return TestResult{
            value == received,
            value,
            received
        };
    });

    this->test("DynamicVar::getInt method should receive the value '1092'", []{
        int value = 1092;
        DynamicVar dv;
        dv.setInt(value);

        int received = dv.getInt();

        return TestResult{
            value == received,
            std::to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar::getInt methodshould NOT receive the value '10000000000' (ten bilion)", []{
        int value = 10000000000;
        DynamicVar dv;
        dv.setInt(value);

        int received = dv.getInt();

        return TestResult{
            "10000000000" != to_string(received),
            "!= 10000000000",
            to_string(received)
        };
    });

    this->test("DynamicVar::getInt64 method should receive the value '100000000000' (100 bilion)", []{
        int64_t value = 100000000000;
        DynamicVar dv;
        dv.setInt64(value);

        int64_t received = dv.getInt64();

        return TestResult{
            value == received,
            std::to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar::getDouble method should receive the value '3874.5601'", []{
        double value = 3874.5601000000000f;
        DynamicVar dv;
        dv.setDouble(value);

        double received = dv.getDouble();

        return TestResult{
            value == received,
            to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar::getBool method should reveive the boolean value 'true'", []{
        bool value = true;
        DynamicVar dv;
        dv.setBool(value);

        bool received = dv.getBool();

        return received == true;
    });

}

void DynamicVarTests::testSetUsingAttribuitions()
{
    this->blueMessage("Test of setting value using attribuition operator");

    this->test("DynamicVar::setSstring method should set internal value to 'this is a test'", []{
        string value = "this is a test";
        DynamicVar dv = value;

        string received = dv.getString();

        return TestResult{
            value == received,
            value,
            received
        };
    });

    this->test("DynamicVar::setInt method should set internal value to '1092'", []{
        int value = 1092;
        DynamicVar dv = value;

        string received = dv.getString();

        return TestResult{
            std::to_string(value) == received,
            std::to_string(value),
            received
        };
    });

    this->test("DynamicVar::setInt methodshould NOT set internal value to '10000000000' (ten bilion)", []{
        int value = 10000000000;
        DynamicVar dv = value;

        string received = dv.getString();

        return TestResult{
            "10000000000" != received,
            "10000000000",
            received
        };
    });

    this->test("DynamicVar::setInt64 method should set internal value to '100000000000' (100 bilion)", []{
        int64_t value = 100000000000;
        DynamicVar dv = value;

        string received = dv.getString();

        return TestResult{
            std::to_string(value) == received,
            std::to_string(value),
            received
        };
    });

    this->test("DynamicVar::setDouble method should set internal value to '3874.5601'", []{
        double value = 3874.5601000000000f;
        DynamicVar dv = value;

        double received = dv.getDouble();

        return TestResult{
            value == received,
            to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar::setBool method should set internal value to '1'", []{
        bool value = true;
        DynamicVar dv = value;

        string received = dv.getString();

        return TestResult{
            "1" == received,
            "1",
            received
        };
    });
}

void DynamicVarTests::testGetUsingAttribuitions()
{
    this->blueMessage("Test of getting value using attribuition operator");

    this->test("DynamicVar::getSstring method should receive the value 'this is a test'", []{
        string value = "this is a test";
        DynamicVar dv;
        dv.setString(value);

        string received = dv;

        return TestResult{
            value == received,
            value,
            received
        };
    });

    this->test("DynamicVar::getInt method should receive the value '1092'", []{
        int value = 1092;
        DynamicVar dv;
        dv.setInt(value);

        int received = dv;

        return TestResult{
            value == received,
            std::to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar::getInt methodshould NOT receive the value '10000000000' (ten bilion)", []{
        int value = 10000000000;
        DynamicVar dv;
        dv.setInt(value);

        int received = dv;

        return TestResult{
            "10000000000" != to_string(received),
            "!= 10000000000",
            to_string(received)
        };
    });

    this->test("DynamicVar::getInt64 method should receive the value '100000000000' (100 bilion)", []{
        int64_t value = 100000000000;
        DynamicVar dv;
        dv.setInt64(value);

        int64_t received = dv;

        return TestResult{
            value == received,
            std::to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar::getDouble method should receive the value '3874.5601'", []{
        double value = 3874.5601000000000f;
        DynamicVar dv;
        dv.setDouble(value);

        double received = dv;

        return TestResult{
            value == received,
            to_string(value),
            to_string(received)
        };
    });

    this->test("DynamicVar::getBool method should reveive the boolean value 'true'", []{
        bool value = true;
        DynamicVar dv;
        dv.setBool(value);

        bool received = dv;

        return received == true;
    });

}

void DynamicVarTests::testPointers()
{
    this->blueMessage("Storage of pointers tests");

    class MyClass{  
    public:
        string tmp;
        string getTmp(){
            return tmp;
        }

    };

    this->test("Create object and save pointer inside DynamicVar", [](){
        DynamicVar tmp;

        tmp.setPointer<MyClass>(new MyClass());

        tmp.getPointer<MyClass>()->tmp = "Sample text";
        return tmp.getPointer<MyClass>()->getTmp() == "Sample text";
    });

    this->test("Get object pointer using direct attribuition", [](){
        DynamicVar tmp;

        tmp.setPointer<MyClass>(new MyClass());
        tmp.getPointer<MyClass>()->tmp = "Sample text";


        //direct attribuition
        MyClass *myObject = tmp;

        return myObject->getTmp() == "Sample text";
    });
}

void DynamicVarTests::testeSerializable()
{
    this->blueMessage("ISerializable tests");
    class MyClass: public DynamicVar::ISerializable{  
    public:
        string prop1;
        int prop2;

        string serialize(){
            return prop1 + ";" +to_string(prop2);
        }

        void deserialize(string data){
            if (auto pos = data.find(";"); pos != string::npos)
            {
                this->prop1 = data.substr(0, pos);
                this->prop2 = stoi(data.substr(pos+1));
            }
            else 
                throw runtime_error("Invalid data to be deserialized");
            
        }
    };


    this->test("Verifying serialization function using reference", []()
    {
        MyClass myObject;
        myObject.prop1 = "aa";
        myObject.prop2 = 2;

        DynamicVar serializedData;
        serializedData.setISerializable(myObject);

        return TestResult{
            result: serializedData.getString() == "aa;2",
            expected: "aa;2",
            returned: serializedData
        };
    });

    this->test("Verifying serialization function using pointer", []()
    {
        auto myObject = new MyClass;
        myObject->prop1 = "dd";
        myObject->prop2 = 5;

        DynamicVar serializedData;
        serializedData.setISerializable(myObject);

        return TestResult{
            result: serializedData.getString() == "dd;5",
            expected: "dd;5",
            returned: serializedData
        };
    });

    this->test("Deserialization function", []()
    {
        auto myObject = new MyClass;
        myObject->prop1 = "abc";
        myObject->prop2 = 12;

        DynamicVar firstDV;
        firstDV.setISerializable(myObject);

        DynamicVar secondDV;
        secondDV.setString(firstDV.getString());

        return TestResult{
            result: secondDV.getString() == "abc;12",
            expected: "abc;12",
            returned: secondDV
        };
    });
}