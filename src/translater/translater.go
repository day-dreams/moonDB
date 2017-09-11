/*
Package translater 实现了一个翻译类，提供VmMessage向Resp协议，Resp协议向VitrualMachine.Operations的转换
*/

package translater

import (
	"fmt"

	"../operation"
)

// ToOperation 将Resp协议字符串转换成operation
func ToOperation(resp string) []operation.Operation {
	// fmt.Println(resp)
	begin := 1 /* 跳过 * 符号*/
	var newBegin, bulkStrLen, bulkStrNum, commandNum int
	var bulkStr, op string
	commandNum, newBegin = getNumber(resp, begin)
	newBegin += 2 /* index现在指向第一个bulkstring的$符号 */
	// operations := make([]operation.Operation, 2)
	for i := 0; commandNum > i; i++ {
		// 每次解析出一个操作，包括类型和参数
		newBegin += 1
		bulkStrLen, newBegin = getNumber(resp, newBegin)
		newBegin += 2
		op, newBegin = getBulkString(resp, newBegin, bulkStrLen)
		newBegin += 2
		fmt.Println(op)
		bulkStrNum = operation.GetParaNum(&op)
		for j := 0; j < bulkStrNum; j++ {
			newBegin += 1
			bulkStrLen, newBegin = getNumber(resp, newBegin)
			newBegin += 2
			bulkStr, newBegin = getBulkString(resp, newBegin, bulkStrLen)
			newBegin += 2
			fmt.Println(bulkStr)
		}
	}
	return make([]operation.Operation, 1)
}

//从resp的特定下标开始读取一个数字，并返回新的下标;index指向数字第一位
func getNumber(str string, index int) (number, nextIndex int) {
	begin := index
	number = 0
	for ; str[begin] <= '9' && str[begin] >= '0'; begin++ {
		// fmt.Printf("%v %v ", str[begin] <= '9', str[begin] >= '0')
		// fmt.Printf("str[%d]=%c\n", begin, str[begin])
		number *= 10
		number += (int)(str[begin] - '0')
	}
	return number, begin
}

// 从resp的特定下标开读取一个定长的bulkString,index指向 $ 符号
func getBulkString(str string, index int, len int) (bulkString string, nextIndex int) {
	return str[index : index+len], index + len
}
