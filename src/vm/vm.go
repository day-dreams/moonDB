/*
Package vm 是一个抽象出来的虚拟机，接受来自服务器的请求，直接操作数据库引擎
*/

package vm

import (
	"sync"

	"db"
	"operation"
	"translater"
	"vmMessage"
)

// VirtualMachine 是虚拟机类
type VirtualMachine struct {
	database db.DbBase
	mutex    sync.Mutex
}

// GenerateVM 生产一个虚拟机，虚拟机内包含一个dbbase
func GenerateVM() *VirtualMachine {
	dbbase := db.GenerateDbBase()
	vm := VirtualMachine{database: dbbase}
	return &vm
}

func (vm *VirtualMachine) set(key, value string) vmMessage.VMMessage {
	vm.database.Set(key, value)
	return vmMessage.GenerateSimpleStringMessage("Ok.")
}

func (vm *VirtualMachine) get(key string) vmMessage.VMMessage {
	value, err := vm.database.Get(key)
	if err == nil {
		return vmMessage.GenerateBulkStringMessage(value)
	}
	return vmMessage.GenerateErrorMessage("fail to get value of " + key)
}

func (vm *VirtualMachine) existed(key string) vmMessage.VMMessage {
	flag := vm.database.Existed(key)

	if flag {
		return vmMessage.GenerateSimpleStringMessage("true")
	}
	return vmMessage.GenerateSimpleStringMessage("false")
}

func (vm *VirtualMachine) del(key string) vmMessage.VMMessage {
	vm.database.Del(key)
	return vmMessage.GenerateSimpleStringMessage("Ok.")
}
func (vm *VirtualMachine) pingpong() vmMessage.VMMessage {
	return vmMessage.GenerateSimpleStringMessage("Pong.")
}

// Execute 执行服务器传来的resp字节流请求,返回resp响应
func (vm *VirtualMachine) Execute(request string) string {
	// TODO:暂不支持pipelines，只是写出循环而已
	operations := translater.RespToOperation(request)
	var ret string
	for i := 0; i < len(operations); i++ {
		switch operations[i].Type {
		case operation.SET:
			ret = translater.VmMessageToResp(vm.set(operations[i].Parameters[0], operations[i].Parameters[1]))
		case operation.GET:
			ret = translater.VmMessageToResp(vm.get(operations[i].Parameters[0]))
		case operation.DEL:
			ret = translater.VmMessageToResp(vm.del(operations[i].Parameters[0]))
		case operation.EXISTED:
			ret = translater.VmMessageToResp(vm.existed(operations[i].Parameters[0]))
		case operation.PingPong:
			ret = translater.VmMessageToResp(vm.pingpong())
		default:
		}
	}
	return ret
}
