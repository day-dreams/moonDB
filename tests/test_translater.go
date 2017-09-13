package tests

import "../translater"
import "fmt"

func TestTranslater() {
	var str string
	str = string("*2\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n")
	ops := translater.RespToOperation(str)
	fmt.Printf("len=%d,cap=%d,ele=%v", len(ops), cap(ops), ops)

}
