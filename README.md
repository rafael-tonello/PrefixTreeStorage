# About

PrefixThreeStorage is a key-value storage library that stores key-value pairs in a file. The data is stored in a prefix tree struct, which allows a fast storage and retrieval of key-value pairs. The library is written in C++ and is 'self-contained', meaning that easy to use in C++ projects, just include the header file and link the source file.

If you are not programmin in C++, as parte of this project, there is a library project that provides a C interface to the PrefixThreeStorage library. The C interface is a shared library that can be used in any programming language that can call C functions.

The project also provides a command line tool that allow you to use the PrefixThreeStorage library from the command line, from shell script and from other programming languages that can call shell commands. The command line tool also has an HTTP server, so you can use the PrefixThreeStorage library from any programming language that can make HTTP requests.

Also, there is a Go lang wrapper for the C interface, so you can use the PrefixThreeStorage library in Go lang projects.

# C++ example of use

```cpp
#include <prefixtree.h>

int main(){
    PrefixTree<string> p("./database.bin", [](string v){return v;}, [](string v){return v;});

    p.set("this is a key", "this is a value");
    auto ret = p.get("this is a key");

    cout << "returned value: " << ret << endl;

    //this will print "returned value: this is a value"


    return 0;
}
```

# The dynmic library (.so)

the dynamic library (.so) has the following functions:

```c
//create a new PrefixTree object. Returns a pointer to the object that should be used in the other functions
extern "C" void* pkv_new(char* filename, uint tree_block_size);

//free a PrefixTree object. 'p' is the pointer to the object (returned by pkv_new)
extern "C" void pkv_free(void* p);

//set a key-value pair. 'p' is the pointer to the object, 'key' is the key and 'value' is the value. Key an value are C strings, so they must be null terminated
extern "C" void pkv_set(void* p, char* key, char* value);

//get a value from a key. 'p' is the pointer to the object, 'key' is the key. Key is a C string, so it must be null terminated. Returns the value as a C string
extern "C" char* pkv_get(void* p, char* key);
```

A example of use (using dlfcn.h):

```c
#include <dlfcn.h>
#include <stdio.h>

int main(){
    void* handle = dlopen("pkv.so", RTLD_LAZY);

    void* (*pkv_new)(char*, uint) = dlsym(handle, "pkv_new");
    void (*pkv_free)(void*) = dlsym(handle, "pkv_free");
    void (*pkv_set)(void*, char*, char*) = dlsym(handle, "pkv_set");
    char* (*pkv_get)(void*, char*) = dlsym(handle, "pkv_get");

    void* p = pkv_new("./database.bin", 64);

    pkv_set(p, "this is a key", "this is a value");
    char* ret = pkv_get(p, "this is a key");

    printf("returned value: %s\n", ret);

    pkv_free(p);

    dlclose(handle);

    return 0;
}
```


# The command line tool
### about
The command line tool allow you to use the PrefixThreeStorage library from the command line, from shell script and from other programming languages that can call shell commands. An HTTP server is also provided, so you can use the PrefixThreeStorage library from any programming language that can make HTTP requests.

### helper text
```
$ pkv --help
PrefixTree Key-Value storage. Version 0.1.0
Usage:./pkv action [options]
Actions:
  set, -s <key> <value>: set a value in the storage
  get, -g <key>: get a value from the storage
  delete, -d <key>: remove a value from the storage
  search, -S <key>: search for keys in the storage
  startHttpServer, -H <port> [options]: start a http server in the port <port> to access the storage
    Options:
      --dependendsOnPID, -d <pid>: set a PID to be monitored. If the process ends, this program exists. You can specify multiple dependentpids
Options:
  --help, -h: display this help
  --version, -v: display the version
  --file, -f <file>: file to be used as stroage. If no one is provided, a default will be used (/home/rafinha_tonello/.local/pkv_default_db)
  --blocksize, -b <size>: block size to be used in the storage. If no one is provided, a default will be used (64)
  --verbose, -V: pkv will print debug information to stdout
```

### http server

The http server is a REST server that can be used to access the PrefixThreeStorage library.

to start the pkv http server, use the following command:
```shell
pkv startHttpServer 8080
```

 The server has the following endpoints
``` shell

#setting a key-value
curl -sS -X POST http://localhost:8080/the/key/name




#retrieving a value
curl -sS -X GET http://localhost:8080/the/key/name
#prints "the value"

#deleting a value
curl -sS -X DELETE http://localhost:8080/the/key/name
```

One or more PIDs can be monitored by the server. If any of the monitored PIDs ends, the server will also end. This is useful to make the server automatically end when the process that started it ends. To monitor a PID, use the -d option:

```shell
    # $$ returns the current process PID
    pkv startHttpServer 8080 -d $$

    #if the current process ends, the server will also end
```

---
---
### takslists

    [ ] add a header to the file storage
