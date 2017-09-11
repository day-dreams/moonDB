package main

import "../translater"
import "fmt"

func main() {
	var str string
	str = string("*1\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n")
	translater.ToOperation(str)
	fmt.Println("------")
	str = string("*2\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n")
	translater.ToOperation(str)
	fmt.Println("------")
	str = string("*2\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n$3\r\nSET\r\n$5\r\nHELLO\r\n$5\r\nWORLD\r\n")
	translater.ToOperation(str)
}
