package main

import (
	"pkv_golang/pkv"
)

func main() {

	obj := pkv.New("test.db", 1024)
	obj.Set("key", "value")
	obj.Get("key", "default value")
	obj.Set("key", "")

	result := obj.Get("key", "default value")
	println(result)
}
