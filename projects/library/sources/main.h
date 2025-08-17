#ifndef __MAIN__H__
#define __MAIN__H__
#include <vector>
#include <string>

#include <prefixtree.h>
#include <filestorage.h>
#include <iostream>
#include <errors.h>
#include <logger.h>
#include <LoggerConsoleWriter.h>
#include <LoggerFileWriter.h>
#include <string>

using namespace std;

class InstanceInfo{ public:
    FileStorage* storage;
    PrefixTree<string> *tree;
    
    char *getBuffer = new char[1];
    char** searchChildsBuffer = nullptr;
};

const string APP_VERSION = "0.0.1";
ILogger* logger = new Logger({
    //new LoggerConsoleWriter(LOGGER_LOGLEVEL_TRACE),
    new LoggerFileWriter("prefixTreeLibrary.log", LOGGER_LOGLEVEL_TRACE)
});

auto log = logger->getNamedLogger("LibInterface"); 

typedef int (*FilterFunction)(char* key, char* value);

//Logger(vector<ILogWriter*> writers, bool intercepCoutCerrAndCLog = false, bool tryToIdentifyLogLevelOfStdoutMessages = false);

extern "C" void* pkv_new(char* filename, uint tree_block_size);
extern "C" void pkv_free(void* p);
extern "C" void pkv_set(void* p, char* key, char* value);
extern "C" char* pkv_get(void* p, char* key);
extern "C" void pkv_delete(void* p, char* key);

extern "C" int exists(void* p, char* key);
extern "C" char** searchChilds(void* p, char* key, uint maxResults, FilterFunction filter, int processChildsOfNodesExcludedByFilter);

//this function is automatically called by searchChilds in each call
extern "C" void freeSearchChildsResult(void *p);
#endif
