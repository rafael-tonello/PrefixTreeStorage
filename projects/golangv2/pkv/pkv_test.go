package pkv_tests

import (
	"pkv"
	"testing"
)

func TestNewPkv(t *testing.T) {
	_, error := pkv.NewPkv[int]("test")
	if error != nil {
		t.Error("Expected instance to be non-nil")
	}
}
