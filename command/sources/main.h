#ifndef __MAIN__H__
#define __MAIN__H__
#include <vector>
#include <string>
#include <argutils.h>

#include <prefixtree.h>
#include <filestorage.h>
#include <iostream>
#include <errors.h>
#include <KWWebServerRouter.h>
#include <HttpData.h>

using namespace std;
using namespace KWShared;

const string APP_VERSION = "0.1.0";

class Main{
private:
    FileStorage *storage;
    PrefixTree<string> *p;
    bool verbose=false;

    bool displayHelper(ArgUtils &parser);
    bool displayVersion(ArgUtils &parser);
    tuple<string, Error> getFileName(ArgUtils &parser);
    Error processCommands(ArgUtils &parser, PrefixTree<string> *p);
    tuple<PrefixTree<string>*, Error> initStorage(string filename);
    Error startHttpServer(ArgUtils &parser, int port, PrefixTree<string> *p);

    Error checkConflicts(ArgUtils &parser);
    bool isConflicting(ArgUtils &parser, vector<string> first, vector<string> second);
    void checkVerbose(ArgUtils &parser);
    void debug(string message);


    
public:
    Main();
    ~Main();

    int run(vector<string> args);

};

#endif
