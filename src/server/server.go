/*
Package server 实现moonDB的服务器逻辑
*/
package server

import (
	"log"
	"net"
	"strconv"
)

// CallbackT 是Server的callback类型
type CallbackT func(net.Conn)

// Echo 是一个默认的CallbackT类型的回调函数
func Echo(conn net.Conn) {
	readCache := make([]byte, 1024)
	for {
		n, error := conn.Read(readCache)
		readCache = readCache[0:n]
		if error != nil {
			log.Print(error)
			continue
		} else {
			log.Print("recv ", n, " bytes from ", conn.RemoteAddr(), "requests: ", string(readCache))
		}
		_, error = conn.Write(readCache)
		if error != nil {
			log.Print(error)
		}
	}
}

// TCPServer 是服务器类,实现基本的连接处理功能
type TCPServer struct {
	Port     int
	Address  string
	Callback CallbackT
}

// GenerateTCPServer 生成一个TCP服务器对象
func GenerateTCPServer(port int, address string, callback CallbackT) TCPServer {
	r := TCPServer{port, address, callback}
	return r
}

// Run 开启服务器,包括监听等一系列步骤
func (server *TCPServer) Run() {
	address := server.Address
	address += ":"
	address += strconv.Itoa(server.Port)
	listner, err := net.Listen("tcp", address)
	if err != nil {
		log.Fatal(err)
	}
	defer listner.Close()
	for {
		conn, error := listner.Accept()
		// defer conn.Close()
		if error != nil {
			log.Print(error)
		} else {
			go server.Callback(conn)
		}
	}

}
