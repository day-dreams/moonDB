package main

import (
	"fmt"

	"translater"
	"vm"
)

func testTranslater() {
	var str string
	str = string("*2\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n")
	ops := translater.RespToOperation(str)
	fmt.Printf("len=%d,cap=%d,ele=%v", len(ops), cap(ops), ops)

}

func testVM() {
	set := string("*1\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n")
	get := string("*1\r\n$3\r\nGET\r\n$5\r\nHELLO\r\n")
	existed := string("*1\r\n$7\r\nEXISTED\r\n$5\r\nHELLO\r\n")
	del := string("*1\r\n$3\r\nDEL\r\n$5\r\nHELLO\r\n")
	vm := vm.GenerateVM()
	fmt.Println(vm.Execute(set))
	fmt.Println(vm.Execute(get))
	fmt.Println(vm.Execute(existed))
	fmt.Println(vm.Execute(del))
	fmt.Println(vm.Execute(existed))
}

func main() {
	testTranslater()
	// tests.TestVM()
}
