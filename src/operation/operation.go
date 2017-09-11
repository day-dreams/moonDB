/*
Package operation 定义了一个操作类，描述了操作的类型、参数
*/

package operation

const (
	//SET 代表set操作
	SET = iota
	//GET 代表get操作
	GET
	//DEL 代表del操作
	DEL
	//EXISTED 代表existed操作
	EXISTED
	//PingPong 代表PingPong操作
	PingPong
)

// Operation 是一个代表数据库操作的类，包含操作类型以及操作参数
type Operation struct {
	Type       int
	Parameters []string
}

//GetParaNum 返回操作的参数个数
func GetParaNum(op *string) int {
	switch *op {
	case "SET":
		return 2
	case "GET":
		return 1
	case "DEL":
		return 1
	case "EXISTED":
		return 1
	case "PingPong":
		return 0
	default:
		return -1
	}
}
