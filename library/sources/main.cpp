#include "main.h"

void* pkv_new(char* filename, uint tree_block_size)
{
    auto storage = new FileStorage(64, filename);
    
    auto initResult = storage->init();
    if (initResult != Errors::NoError)
    {
        auto error = Errors::derivateError("Error initializing storage", initResult);
        return 0 ;
    }

    auto p = new PrefixTree<string>(storage, [](string v){return v;}, [](string v){return v;});
    return (void*)p;
}

void pkv_free(void* p)
{
    auto storage = ((PrefixTree<string>*)p)->getStorage();
    delete (PrefixTree<string>*)p;
    delete storage;
}

void pkv_set(void* p, char* key, char* value)
{
    try{
        log.debug({"pkv_set called with key", key, "and value", value});
        ((PrefixTree<string>*)p)->set(string(key), string(value));
        log.debug("pkv_set done");
    }catch(exception e){
        log.error({"pkv_set failed with exception", string(e.what())});
    }
}

char *retBuffer = new char[1];
char* pkv_get(void* p, char* key)
{
    log.debug({"pkv_get called with key", key});
    auto ret = ((PrefixTree<string>*)p)->get(string(key));

    delete[] retBuffer;
    retBuffer = new char[ret.size()+1];
    
    memcpy(retBuffer, ret.c_str(), ret.size());
    retBuffer[ret.size()] = 0;

    log.debug({"pkv_get returning", ret});
    return retBuffer;
}

//main that receives command line arguments
int main(int argc, char *argv[])
{
    cout << "Hello world" << endl; 
    return 0;
}
