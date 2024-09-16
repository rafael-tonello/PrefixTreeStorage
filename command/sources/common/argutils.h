#include <string>
#include <vector>
#include <StringUtils.h>
#include <functional>


using namespace std;
class ArgUtils
{
private:
    vector<string> args;

    string _lastError = "";

public:
    ArgUtils(){};
    ArgUtils(int c, char** p, bool usesCommandAsFirst = false)
    {
        int i = usesCommandAsFirst ? 0 : 1;
        for ( ; i < c; i++)
        {
            this->args.push_back(string(p[i]));
        }
    }

    bool isFlag(string arg){
        return arg.find("-") == 0 && arg.find("--") == string::npos;
    }

    bool isArg(string arg){
        return arg.find("--") == 0;
    }

    ArgUtils(vector<string> args, bool usesCommandAsFirst = false)
    {
        size_t i = usesCommandAsFirst ? 0 : 1;
        size_t c = args.size();
        for ( ; i < c; i++)
        {
            this->args.push_back(args[i]);
        }
    }

    size_t size(){
        return this->args.size();
    }

    vector<int> findIndexes(vector<string> args)
    {
        vector<int> result;
        for (int i = 0; i < this->args.size(); i++)
        {
            auto c = this->args[i];
            auto splittedC = StringUtils::split(c, {":", "=", " "});
            for (auto &d: args)
            {
                if (
                    c == d || 
                    (
                        splittedC.size() > 1 &&
                        splittedC[0] == d
                    )
                ){
                    result.push_back(i);
                }
            }
        }
        return result;
    }

    /**
     * @brief returns the arg name/text using an index
     * 
     * @param index the index of argument
     * @return string 
     */
    string getText(int index, string valueIfNotFOund = "")
    {
        _lastError = "";
        if (index >= 0 && index < this->args.size())
            return this->args[index];

        _lastError = "Argument not found by index";
        return valueIfNotFOund;
    }
    
    bool remove(int index)
    {
        if (index >= 0 && index < this->args.size())
        {
            this->args.erase(this->args.begin() + index);
            return true;
        }
        return false;
    }

    /**
     * @brief returns an arg value using its index. The value will be searched after a keyvalue separator or the next value will be used as value
     * 
     * @param index the index of argument
     * @param defaultValue a value to be returned if the argument is not found
     * @return string 
     */
    typedef struct { string value; bool isTheNextArgument; bool found; } GetValueR;
    GetValueR getValue(int index, string defaultValue = "")
    {
        GetValueR result;
        result.isTheNextArgument = false;
        result.found = true;

        auto tmp = getText(index, defaultValue);

        //check if tmp contains ' ', ':' or '=' in any position
        
        if (tmp.find(' ') != string::npos)
            tmp = tmp.substr(tmp.find(' ')+1);
        else if (tmp.find(':') != string::npos)
            tmp = tmp.substr(tmp.find(':')+1);
        else if (tmp.find('=') != string::npos)
            tmp = tmp.substr(tmp.find('=')+1);
        else
        {
            //next argument is the value
            if (index+1 < this->args.size())
            {
                tmp = this->args[index+1];
                result.isTheNextArgument = true;
            }
            else
                result.found = false;
        }

        if (tmp.size()> 0) {
            if(tmp[0] == '"')
                tmp = tmp.substr(1);

            if (tmp[tmp.size()-1] == '"')
                tmp = tmp.substr(0, tmp.size()-1);
        }

        result.value = tmp;


            result.found = false;
        return result;
    }

    /**
     * @brief returns an arg value using its index. The value will be searched after a keyvalue separator or the next value will be used as value
     * 
     * @param index the index of argument
     * @param defaultValue a value to be returned if the argument is not found
     * @return string 
     */
    string getName(int index, string defaultValue = "")
    {
        auto tmp = getText(index, "");

        //check if tmp contains ' ', ':' or '=' in any position
        
        if (tmp.find(' ') != string::npos)
            tmp = tmp.substr(0, tmp.find(' ')-1);
        else if (tmp.find(':') != string::npos)
            tmp = tmp.substr(0, tmp.find(':')-1);
        else if (tmp.find('=') != string::npos)
            tmp = tmp.substr(0, tmp.find('=')-1);

        return tmp;
    }
    
    bool remove(vector<string> argOrFlag)
    {
        bool result = false;
        for (auto &c: argOrFlag)
        {
            for (int i = this->args.size()-1; i >= 0; i--)
            {
                if (this->args[i] == c)
                {
                    this->args.erase(this->args.begin() + i);
                    result = true;
                }
            }
        }
        return result;
    }

    /**
     * @brief Returns the values of the arguments 'args'. The values will be searched after keyvalue separators or the next value of each one will be used as value
     * 
     * @param args the argument names
     * @param defaultValue a value to be returned if the argument is not found
     * @return string 
     */
    vector<GetValueR> getValues(vector<string> argsAndFlags)
    {
        auto matchAnyPrefix = [&](string argText){
            auto splittedText = StringUtils::split(argText, {":", "=", " "});
            for (auto c: argsAndFlags){
                if (
                    (c == argText) ||
                    (
                        splittedText.size() > 1 &&
                        splittedText[0] == argText
                    )
                )
                    return c;
                
            }

            return string("");
        };

        vector<GetValueR> result;
        _lastError = "";
        for (int i = 0; i < args.size(); i++)
        {
            auto c = args[i];
            if (auto match = matchAnyPrefix(c); match != "") // != string::npos)
            {
                result.push_back(getValue(i, ""));
            }
        }

        return result;
    }
    
    struct SearchArgsResult { string arg; int index; };
    vector<SearchArgsResult> search(function<bool(string)> f)
    {
        vector<SearchArgsResult> result;
        int index = 0;
        for (auto c: this->args)
        {
            if (f(c))
                result.push_back({
                    .arg = c,
                    .index = index
                });
        }
        return result;
    }

    
    /**
     * @brief Makes a search bu the argument in the current argument list. The argument is 
     * considered valid if the 'argPrefix' is present at begin of the any argument.
     * For example: if you run you the app with --myargument and you use 
     * 'contains("--myargument")', the contains will return true. If you use 
     * 'contains("--myarg")', the 'contais' function will also return true, because '--myarg' 
     * are present at begining of the '--myargument'. If you need a exact match, use the 
     * 'containsExact' method.
     * 
     * @param arg 
     * @return true 
     * @return false 
     */
    bool containsArg(string arg)
    {
        for (int i = 0; i < this->args.size(); i++)
        {
            auto c = this->args[i];
            auto splittedC = StringUtils::split(c, {":", "=", " "});
            if (isArg(c)){
                if (
                    (c == arg) || 
                    (
                        splittedC.size() > 0 &&
                        splittedC[0] == arg
                    )
                ){
                    return true;
                }
            }
        }

        //try find in flags list
        return false;
    }

    /**
     * @brief checks if at least one of the passed arguments is present in the command line
     * 
     * @param anyOfTheseArgs the argument list
     * @return true 
     * @return false 
     */
    bool containsArg(vector<string> anyOfTheseArgs)
    {
        for (auto &c: anyOfTheseArgs)
            if (containsArg(c))
                return true;

        return false;
    }


    /**
     * @brief this method is similar to 'contains', but is valid only for flag arguments (-a, -o, -ao). 
     * This is utils when you expect to receive composed flags like '-ao' (instead inform individually -a and -o).
     * 
     * @param flag 
     * @return true 
     * @return false 
     */
    bool containsFlag(string flag)
    {
        if (flag.find("-") == 0)
            flag = flag.substr(1);

        for (auto &c: this->args)
        {
            auto splittedC = StringUtils::split(c, {":", "=", " "});
            if (isFlag(c))
            {
                if (
                    (c == flag) || 
                    (
                        splittedC.size() > 0 &&
                        splittedC[0].find(flag) != string::npos
                    )
                )
                    return true;
            }
        }

        return false;
    }

    bool containsArgOrFlag(string argOrFlag)
    {
        return containsArg(argOrFlag) || containsFlag(argOrFlag);
    }

    bool containsArgOrFlag(vector<string> anyOfTheseArgs)
    {
        for (auto &c: anyOfTheseArgs)
            if (containsArgOrFlag(c))
                return true;

        return false;
    }

    string getLastError(){
        return this->_lastError;
    }

    vector<string> getArgs(){
        return this->args;
    }

    vector<SearchArgsResult> filter(function<bool(string)> f)
    {
        return search(f);
    }

    template <typename T>
    vector<T> map(function<T(string)> f)
    {
        vector<T> result;
        for (auto &c: this->args)
            result.push_back(f(c));
        return result;
    }

    template<typename T>
    T reduce(function<T(T, string)> f, T initialValue = T())
    {
        T result = initialValue;
        for (auto &c: this->args)
            result = f(result, c);
        return result;
    }

    void forEach(function<void(string)> f)
    {
        for (auto &c: this->args)
            f(c);
    }

};
