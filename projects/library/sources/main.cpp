#include "main.h"

void* pkv_new(char* filename, uint tree_block_size)
{
    auto ret = new InstanceInfo();

    ret->storage = new FileStorage(64, filename);
    
    auto initResult = ret->storage->init();
    if (initResult != Errors::NoError)
    {
        auto error = Errors::derivateError("Error initializing storage", initResult);
        return 0 ;
    }

    ret->tree = new PrefixTree<string>(ret->storage, [](string v){return v;}, [](string v){return v;});
    return (void*)ret;
}

void pkv_free(void* p)
{
    auto instance = (InstanceInfo*)p;
    delete instance->tree;
    delete instance->storage;
    freeSearchChildsResult(p);
    delete[] instance->getBuffer;
    delete instance;
}

void pkv_set(void* p, char* key, char* value)
{
    auto instanceInfo = (InstanceInfo*)p;
    try{
        //log.debug({"pkv_set called with key", key, "and value", value});
        instanceInfo->tree->set(string(key), string(value));
        //log.debug("pkv_set done");
    }catch(exception e){
        log.error({"pkv_set failed with exception", string(e.what())});
    }
}


char* pkv_get(void* p, char* key)
{
    //log.debug({"pkv_get called with key", key});
    auto instanceInfo = (InstanceInfo*)p;
    auto ret = instanceInfo->tree->get(string(key));

    delete[] instanceInfo->getBuffer;
    instanceInfo->getBuffer = new char[ret.size()+1];
    
    memcpy(instanceInfo->getBuffer, ret.c_str(), ret.size());
    instanceInfo->getBuffer[ret.size()] = 0;

    //log.debug({"pkv_get returning", ret});
    return instanceInfo->getBuffer;
}

int exists(void* p, char* key)
{

}

void pkv_delete(void* p, char* key)
{
    auto instanceInfo = (InstanceInfo*)p;
    instanceInfo->tree->remove(string(key));
}


void freeSearchChildsResult(void* p)
{
    auto instanceInfo = (InstanceInfo*)p;
    if (instanceInfo->searchChildsBuffer != nullptr)
    {
        for (int c = 0; instanceInfo->searchChildsBuffer[c] != nullptr; c++)
            delete[] instanceInfo->searchChildsBuffer[c];
        delete[] instanceInfo->searchChildsBuffer;
    }
}
char** searchChilds(void* p, char* key, uint maxResults, FilterFunction filter, int processChildsOfNodesExcludedByFilter)
{
    auto instanceInfo = (InstanceInfo*)p;

    freeSearchChildsResult(p); //clears the buffer

    vector<string> ret;

    if (filter != nullptr && filter != NULL)
    {
        ret = instanceInfo->tree->searchChilds(string(key), maxResults);
    }
    else{
        ret = instanceInfo->tree->searchChilds(string(key), maxResults, [&](Node &node){
            return filter((char*)node.key.c_str(), (char*)node.data.c_str()) == 1;
        }, processChildsOfNodesExcludedByFilter);
    }

    instanceInfo->searchChildsBuffer = new char*[ret.size()+1];
    for (int c = 0; c < ret.size(); c++)
    {
        instanceInfo->searchChildsBuffer[c] = new char[ret[c].size()+1];
        memcpy(instanceInfo->searchChildsBuffer[c], ret[c].c_str(), ret[c].size());
        instanceInfo->searchChildsBuffer[c][ret[c].size()] = 0;
    }
    instanceInfo->searchChildsBuffer[ret.size()] = nullptr;

    return instanceInfo->searchChildsBuffer;
}
