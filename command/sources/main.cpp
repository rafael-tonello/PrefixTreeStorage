#include "main.h"

Main::Main()
{
    
}

Main::~Main()
{
    
}

int Main::run(vector<string> args)
{

    //args = { "pkv", "get", "test", "-f:./aa.txt" };
    //args = { "./pkv", "-H", "10101", "-V"};
    //args = { "./pkv", "-H", "10101", "-V"};
    //args = { "./pkv", "-h"};
    //args = {"/home/rafinha_tonello/.local/bin/pkv", "-H", "50654", "-f:\"/media/veracrypt/projects/rafinha_tonello/shellscript_utils/tests/tmp/tmpDbs/db4\""};
    // /home/rafinha_tonello/.local/bin/pkv get thekey -f:/media/veracrypt/projects/rafinha_tonello/shellscript_utils/tests/tmp/tmpDbs/db4
    //args = { "/home/rafinha_tonello/.local/bin/pkv",  "-H", "50938", "-f:./tmp/tmpDbs/testInit4.db", "-d", "2887652"};

    ArgUtils parser(args, true);

    checkVerbose(parser);

    //helper and vertion
    if (displayHelper(parser))
        return 0;

    if (displayVersion(parser))
        return 0;


    //check for conflicts in the command argument
    auto conflictError = checkConflicts(parser);
    if (conflictError != Errors::NoError)
    {
        cout << Errors::derivateError("Conflict between options: ", conflictError) << endl;
        return 1;
    }

    //determine the filename
    auto [ filename, error] = getFileName(parser);
    if (error != Errors::NoError)
    {
        cout << "Error resolving file name: " << error << endl;
        return 1;
    }

    //initialize the stroage and the prefixTree
    auto [ prefixTree, errInitStorage ] = initStorage(filename);
    if (errInitStorage != Errors::NoError)
    {
        cout << "Error initializing storage: " << errInitStorage << endl;
        return 1;
    }


    //detect and process commands
    auto errProcessCommands = processCommands(parser, prefixTree);
    if (errProcessCommands != Errors::NoError)
    {
        cout << "Error processing commands: " << errProcessCommands << endl;
        return 1;
    }

    return 0;
}

tuple<PrefixTree<string>*, Error> Main::initStorage(string filename)
{
    debug("Initing storage with filename " + filename);
    this->storage = new FileStorage(64, filename);
    
    auto initResult = storage->init();
    if (initResult != Errors::NoError)
    {
        auto error = Errors::derivateError("Error initializing storage", initResult);
        return { nullptr, error };
    }

    this->p = new PrefixTree<string>(storage, [](string v){return v;}, [](string v){return v;});
    return { this->p, Errors::NoError };
}

Error Main::processCommands(ArgUtils &parser, PrefixTree<string> *p)
{
    for (int index = 1; index < parser.size(); index++)
    {
        auto c = parser.getText(index);

        if (c == "--get" || c == "-g" || c == "get")
        {
            auto key = parser.getText(index +1);
            auto ret = p->get(key);
            cout << ret << endl;

            index += 1;
        }
        else if (c == "--set" || c == "-s" || c == "set")
        {
            auto key = parser.getText(index +1);
            auto value = parser.getText(index +2);
            p->set(key, value);
            index += 2;
        }
        else if (c == "--remove" || c == "-r" || c == "remove" || c == "--delete" || c == "-d" || c == "delete" )
        {
            auto key = parser.getText(index +1);
            p->set(key, "");
            index += 1;
        }
        else if (c == "--search" || c == "-S" || c == "search")
        {
            auto key = parser.getValue(index + 1);
            auto ret = p->searchChilds(key.value);
            for (auto &it : ret)
            {
                cout <<  it << endl;
            }

            index += 1;
            if (key.isTheNextArgument)
                index += 1;
        }
        else if (c.find("--http") != string::npos || c.find("-H") != string::npos || c.find("startHttpServer") != string::npos)
        {
            auto port = parser.getText(index + 1);
            //check if the port is a number
            if (port.find_first_not_of("0123456789") != string::npos)
            {
                cout << "Port must be a number" << endl;
                return Error("Port must be a number");
            }

            
            cout << "Starting http server in port " << port << endl;
            int portI = stoi(port);
            return startHttpServer(parser, portI, p);
        }
        else
        {
            cout << "Unknown command: " << c << endl;
            return Error("Unknown command");
        }
    }
    

    return Errors::NoError;

}

void Main::checkVerbose(ArgUtils &parser)
{
    auto indexes = parser.findIndexes({"--verbose", "-V"});

    this->verbose=indexes.size() > 0;
    for (int c = indexes.size()-1; c >= 0; c--)
        parser.remove(indexes[c]);
}

void Main::debug(string message)
{
    if (this->verbose)
        cout << message << endl;
}

bool Main::displayHelper(ArgUtils &parser)
{
    if (parser.size() == 1 || parser.containsArgOrFlag("--help") || parser.containsArgOrFlag("-h"))
    {
        cout << "PrefixTree Key-Value storage. Version " << APP_VERSION << endl;
        cout << "Usage:" << parser.getText(0, "command") << " action [options]" << endl;
        cout << "Actions:" << endl;
        cout << "  set, -s <key> <value>: set a value in the storage" << endl;
        cout << "  get, -g <key>: get a value from the storage" << endl;
        cout << "  delete, -d <key>: remove a value from the storage" << endl;
        cout << "  search, -S <key>: search for keys in the storage" << endl;
        cout << "  startHttpServer, -H <port> [options]: start a http server in the port <port> to access the storage" << endl;
        cout << "    Options:" << endl;
        cout << "      --dependendsOnPID, -d <pid>: set a PID to be monitored. If the process ends, this program exists. You can specify multiple dependentpids" << endl;
        cout << "Options:" << endl;
        cout << "  --help, -h: display this help" << endl;
        cout << "  --version, -v: display the version" << endl;
        cout << "  --file, -f <file>: file to be used as stroage. If no one is provided, a default will be used (~/.local/pkv_default_db)" << endl;
        cout << "  --blocksize, -b <size>: block size to be used in the storage. If no one is provided, a default will be used (64)" << endl;
        cout << "  --verbose, -V: pkv will print debug information to stdout" << endl;
        return true;
    }
    return false;
}

bool Main::displayVersion(ArgUtils &parser)
{
    if (parser.containsArgOrFlag({string("--version"), string("-v")}))
    {
        cout << "Version: " << APP_VERSION << endl;
        return true;
    }
    return false;
}

tuple<string, Error> Main::getFileName(ArgUtils &parser)
{
    //auto defaultFile = string("~/.local/share/prefixtree_keyvaluedb/pkv_default_db");
    auto ret = string("/tmp/pkv_tmp_db");

    auto indexes = parser.findIndexes({"--file", "-f"});

    if (indexes.size() > 1)
    {
        return { "", Error("Multiple files (--file, -f) provided") };
    }

    if (indexes.size() != 0)
    {
        auto index = indexes[0];
        ret = parser.getValue(index, ret).value;
        if (parser.getValue(index).isTheNextArgument)
            parser.remove(index + 1);

        parser.remove(index);
    }
    

    if (ret == ret)
    {
        //create the directory if it does not exists
        auto dir = ret.substr(0, ret.find_last_of("/"));
        string command = "mkdir -p " + dir;
        system(command.c_str());
    }

    return { ret, Errors::NoError };
}

Error Main::checkConflicts(ArgUtils &parser)
{
    if (isConflicting(parser, {"--get", "-g"}, {"--http", "-H"}))
        return Error("Conflict between --set and --http");

    return Errors::NoError;
}

bool Main::isConflicting(ArgUtils &parser, vector<string> first, vector<string> second)
{
    for (auto f : first)
    {
        if (parser.containsArgOrFlag(f))
        {
            for (auto s : second)
            {
                if (parser.containsArgOrFlag(s))
                    return true;
            }
        }
    }
    return false;
}

Error Main::startHttpServer(ArgUtils &parser, int port, PrefixTree<string> *p)
{
    auto httpServer = new KWWebServerRouter(
        port,
        { }
    ); 

    auto getRoute = httpServer->routeElse();
    getRoute->onHttpRequest( [=](HttpData* in, HttpData* out, map<string, string> vars){
        if (in->method == "GET")
        {
            debug("GET request to " + in->resource);
            auto key = in->resource;
            //remove the first /
            key = key.substr(1);

            //if 'key' is empty, just replay with 'no content'. Client can use this to check if the server is up
            if (key == "")
            {
                out->httpStatus = 204;
                return;
            }

            if (!p->contains(key))
            {
                out->httpStatus = 404;
                return;
            }
            
            auto value = p->get(key, "");
            out->setContentString(value);
        }
        else if (in->method == "POST")
        {
            debug("POST request to '" + in->resource + "' with content '" + in->getContentString()+"'");
            auto key = in->resource;
            //remove the first /
            key = key.substr(1);

            auto value = in->getContentString();

            p->set(key, value);
        }
        else if (in->method == "DELETE")
        {
            debug("DELETE request to " + in->resource);
            auto key = in->resource;
            //remove the first /
            key = key.substr(1);

            p->set(key, "");
        }
    });

    //shared_ptr<KWSRequestRoute> route(string method, string resource);
    //shared_ptr<KWSWebsocketRoute> routeWebsocket(string resource);
    //shared_ptr<KWSRequestRoute> routeElse();
    //shared_ptr<KWSWebsocketRoute> routeWebsocketElse();
    //shared_ptr<KWTinyWebServer> getServer();
    //vector<ClientInfo*> getConnectedWebsockets(){return connectedWebsockets;}

    /*
        else if (c.find("--dependentPID") != string::npos || c.find("-d") != string::npos)
        {
            auto pid = parser.getValue();
            cout << "Setting dependent PID to " << pid << endl;
        }*/


    auto dependentPid = parser.getValues({"--dependendsOnPID", "-d"});
    if (dependentPid.size() > 0)
    {
        for (auto pid : dependentPid)
        {
            cout << "Process monitoring the pid " << pid.value << endl;
        }
    }

    cout << "Http server started and listening in port " << port << endl;
    while (true)
    {
        //che if one of dependent pid is not avalable.. if so, close the server
        for (auto pid : dependentPid)
        {
            debug("Checking if pid " + pid.value + " is running");
            //check if pid is running
            string command = "ps -p " + pid.value + " -o comm=";
            debug("running command '"+command+"'");
            auto result=Utils::ssystem(command);
            if (result == "")
            {
                cout << "Dependent process " << pid.value << " is not running. Closing the server" << endl;
                return Errors::NoError;
            }
        }

             
        usleep(500000);

    }
}



//main that receives command line arguments
int main(int argc, char *argv[])
{
    vector <string> args;
    for (int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    Main main_app;
    return main_app.run(args);
}
