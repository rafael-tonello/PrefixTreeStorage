package pkv

/*
#cgo LDFLAGS: -ldl
#include <stdlib.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>

typedef int (*FilterFunction)(char* key, char* value);

typedec void* (*pkv_new_t)(char* filename, uint tree_block_size);
typedec void (*pkv_free_t)(void* p);
typedec void (*pkv_set_t)(void* p, char* key, char* value);
typedec char* (*pkv_get_t)(void* p, char* key);
typedec void (*pkv_delete_t)(void* p, char* key);
typedec int (*exists_t)(void* p, char* key);
typedec char** (*searchChilds_t)(void* p, char* key, uint maxResults, FilterFunction filter, int processChildsOfNodesExcludedByFilter);
typedec void (*freeSearchChildsResult_t)(void *p);

pkv_new_t pkv_new_p
pkv_free_t pkv_free_p
pkv_set_t pkv_set_p
pkv_get_t pkv_get_p
pkv_delete_t pkv_delete_p
exists_t exists_p
searchChilds_t searchChilds_p
freeSearchChildsResult_t freeSearchChildsResult_p

int libStarted = 0;
int init(char* soPath){
	if (libStarted){
		return 0;
	}

	libStarted = 1;

	//check if soPath is empty
	if(soPath == NULL || soPath[0] == '\0'){
		soPath = "logger.so";
	}

	void* libp = dlopen(soPath, RTLD_LAZY);
	if (libp == 0){
		printf("Failed to load library: %s\n", dlerror());
		return 1;
	}

	pkv_new_t_p = (pkv_new_t)dlsym(libp, "pkv_new");
	pkv_free_t_p = (pkv_free_t)dlsym(libp, "pkv_free");
	pkv_set_t_p = (pkv_set_t)dlsym(libp, "pkv_set");
	pkv_get_t_p = (pkv_get_t)dlsym(libp, "pkv_get");
	pkv_delete_t_p = (pkv_delete_t)dlsym(libp, "pkv_delete");
	exists_t_p = (exists_t)dlsym(libp, "exists");
	searchChilds_t_p = (searchChilds_t)dlsym(libp, "searchChilds");
	freeSearchChildsResult_t_p = (freeSearchChildsResult_t)dlsym(libp, "freeSearchChildsResult");

	return 0;
}

void* pkv_new(char* filename, uint tree_block_size)
{
	return pkv_new_p(filename, tree_block_size);
}

void pkv_free(void* p)
{
	return pkv_free_p(p);
}

void pkv_set(void* p, char* key, char* value)
{
	return pkv_set_p(p, key, value);
}

char* pkv_get(void* p, char* key)
{
	return pkv_get_p(p, key);
}

void pkv_delete(void* p, char* key)
{
	return pkv_delete_p(p, key);
}

int exists(void* p, char* key)
{
	return exists_p(p, key);
}

char** searchChilds(void* p, char* key, uint maxResults, FilterFunction filter, int processChildsOfNodesExcludedByFilter)
{
	return searchChilds_p(p, key, maxResults, filter, processChildsOfNodesExcludedByFilter);
}

void freeSearchChildsResult(void *p)
{
	return freeSearchChildsResult_p(p);
}
*/
import "C"
import (
	"encoding/json"
	"errors"
	"fmt"
	"reflect"
)

type Pkv[T any] struct {
	instance *C.void
}

func NewPkv[T any](filename string) Pkv[T], error {
	return Pkv[T]{Key: filename}, nil
}

func (p *Pkv[T]) Set(key string, value T) {
	valueStr := anyToString(value, false)
	C.pkv_set(p.instance, C.CString(key), C.CString(valueStr))
}

func (p *Pkv[T]) Get(key string) (T, error) {
	var ret T

	valueStr := C.GoString(C.pkv_get(p.instance, C.CString(key)))
	if valueStr == "" {
		return ret, errors.New("Key not found")
	}

	ret = stringToAny[T](valueStr)
	return ret, nil
}

func (p *Pkv[T]) GetOrDefault(key string, defaultValue T) T {
	ret, error := p.Get(key)
	if error != nil {
		return defaultValue
	}
	return ret
}

func (p *Pkv[T]) Delete(key string) {
	C.pkv_delete(p.instance, C.CString(key))
}

func (p *Pkv[T]) SearchChilds(key string) []string {

}

// func (p *Pkv[T]) SearchChildsFiltering(key string, filter func(string,T) bool) []string{
func (p *Pkv[T]) SearchChildsFiltering(key string, filter func(string, T) bool) []string {
	//typedef int (*FilterFunction)(char* key, char* value);

	rawFilter := func(key string, value string) C.int {
		var tmp T;
		tmp = stringToAny[T](value);
		return C.int(filter(key, tmp))
	}

	//char** searchChilds(void* p, char* key, uint maxResults, FilterFunction filter, int processChildsOfNodesExcludedByFilter)

	result := C.searchChilds(p.instance, C.CString(key), 0, rawFilter, 1)

	//convert C char** to a golang slice
	var ret []string
	for i := 0; result[i] != nil; i++ {
		ret = append(ret, C.GoString(result[i]))
	}

	//free the C char**
	C.freeSearchChildsResult(result)
}

func anyToString(value interface{}, enableJsonOutputIdent bool) string {

	msgsType := reflect.TypeOf(value)

	valueStr := ""

	switch msgsType.Kind() {
	case reflect.String:
		valueStr = value.(string)
	//case reflect.Struct:
	//	valueStr = fmt.Sprintf("%v", value)
	default:
		var valueByte []byte
		var err error
		if enableJsonOutputIdent {
			valueByte, err = json.MarshalIndent(&value, "", "  ")
		} else {
			valueByte, err = json.Marshal(&value)
		}

		if err == nil {
			valueStr = string(valueByte)
		} else {
			valueStr = fmt.Sprintf("%v", value)
		}
	}

	//remove possible "[[" and "]]" from the valueString (begin and end)
	//if strings.HasPrefix(valueStr, "[[") && strings.HasSuffix(valueStr, "]]") {
	//	valueStr = valueStr[2 : len(valueStr)-2]
	//}

	return valueStr
}

//func stringToAny(valueStr string) interface{} {
//	var value interface{}
//	err := json.Unmarshal([]byte(valueStr), &value)
//	if err != nil {
//		value = valueStr
//	}
//	return value
//}

func stringToAny[T any](valueStr string) T {
	var value interface{}
	err := json.Unmarshal([]byte(valueStr), &value)
	if err != nil {
		value = valueStr
	}
	return value.(T)
}
