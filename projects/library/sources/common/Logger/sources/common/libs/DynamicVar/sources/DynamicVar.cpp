#include "DynamicVar.h"

DynamicVar::DynamicVar()
{
    __data = "";
}

DynamicVar::DynamicVar(char const *value)
{
    setString(string(value));
}

DynamicVar::DynamicVar(string value)
{
    setString(value);
}

DynamicVar::DynamicVar(int value)
{
    setInt(value);
}

DynamicVar::DynamicVar(uint value)
{
    setUint(value);
}

DynamicVar::DynamicVar(double value)
{
    setDouble(value);
}

DynamicVar::DynamicVar(bool value)
{
    setBool(value);
}

DynamicVar::DynamicVar(int64_t value)
{
    setInt64(value);
}

DynamicVar::DynamicVar(uint64_t value)
{
    setUint64(value);
}

DynamicVar::DynamicVar(DynamicVar::ISerializable* value)
{
    setISerializable(value);
}

DynamicVar::DynamicVar(DynamicVar::ISerializable &value)
{
    setISerializable(value);
}


int DynamicVar::getInt(function<void()> onError)
{
    int ret = 0;
    try
    {
        ret = std::stoi(__data, nullptr);
    }
    catch(...)
    {
        onError();
    }
    return ret;
}

void DynamicVar::setInt(int value)
{
    setOriginalType<int>();
    __data = std::to_string(value);
}

uint DynamicVar::getUint(function<void()> onError)
{
    uint ret = 0;
    try
    {
        ret = (uint)(std::stoul(__data, nullptr));
    }
    catch(...)
    {
        onError();
    }
    return ret;
}

void DynamicVar::setUint(uint value)
{
    setOriginalType<uint>();
    __data = std::to_string(value);
}

int64_t DynamicVar::getInt64(function<void()> onError)
{
    int64_t ret = 0;
    try
    {
        ret = std::stoll(__data, nullptr);
    }
    catch(...)
    {
        onError();
    }
    return ret;
}

void DynamicVar::setInt64(int64_t value)
{
    setOriginalType<int64_t>();
    __data = std::to_string(value);
}

uint64_t DynamicVar::getUint64(function<void()> onError)
{
    uint64_t ret = 0;
    try
    {
        ret = std::stoull(__data, nullptr);
    }
    catch(...)
    {
        onError();
    }
    return ret;
}

void DynamicVar::setUint64(uint64_t value)
{
    setOriginalType<uint64_t>();
    __data = std::to_string(value);
}


double DynamicVar::getDouble(function<void()> onError)
{
    double ret = 0.0;
    try
    {
        ret = std::stof(__data, nullptr);
    }
    catch(...)
    {
        onError();
    }
    return ret;
}

void DynamicVar::setDouble(double value)
{
    setOriginalType<double>();
    __data = std::to_string(value);
}


string DynamicVar::getString()
{
    return __data;
}

void DynamicVar::setString(string value)
{
    setOriginalType<string>();
    __data = value;
}

bool DynamicVar::getBool(function<void()> onError)
{
    bool ret = false;
    try
    {
        string temp = __data;
        std::transform(temp.begin(),
                    temp.end(),
                    temp.begin(),
                    ::tolower);

        ret = (string("trueyes1ok")).find(temp) != string::npos;
    }
    catch(...){
        onError();
    }
    return ret;
}

void DynamicVar::setBool(bool value)
{
    setOriginalType<bool>();

    if (value)
        __data = "1";
    else
        __data = "0";
}

void DynamicVar::setISerializable(ISerializable *object)
{
    setOriginalType_s(string(typeid(object).name()));
    __data = object->serialize();
}

void DynamicVar::setISerializable(ISerializable &object)
{
    setISerializable(&object);
}

DynamicVar::ISerializable* DynamicVar::getISerializable(DynamicVar::ISerializable* object)
{
    object->deserialize(this->__data);
    return object;
}

DynamicVar::ISerializable* DynamicVar::getISerializable(function<DynamicVar::ISerializable*()> createObjectF)
{
    return getISerializable(createObjectF());
}



#ifdef JSONMAKER_H

    DynamicVar::DynamicVar(JsonMaker::JSON value)
    {
        setJson(value);
    }

    JsonMaker::JSON DynamicVar::getJson(function<void()> onError)
    {
        try
        {
            return JsonMaker::JSON(__data);
        }
        catch(...)
        {
            onError();
        }

        return JsonMaker::JSON();
    }

    void DynamicVar::setJson(JsonMaker::JSON data)
    {
        setOriginalType<JsonMaker::JSON>();
        __data = data.ToString();
    }
#endif

bool DynamicVar::isEquals(DynamicVar* other)
{
    return other->getString() == this->__data;
}

bool DynamicVar::isEquals(DynamicVar &other)
{
    return this->isEquals(&other);
}