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
type CallbackT func([]byte, net.Conn)

// Echo 是一个默认的CallbackT类型的回调函数
func Echo(request []byte, conn net.Conn) {
	_, error := conn.Write(request)
	if error != nil {
		log.Print(error)
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
		defer conn.Close()
		if error != nil {
			log.Print(error)
		}
		buffer := make([]byte, 1024)
		n, error := conn.Read(buffer)
		if error != nil {
			log.Print(error)
			continue
		} else {
			log.Printf("recv %4d bytes from %s\n", n, conn.RemoteAddr())
			server.Callback(buffer, conn)
		}
	}

}
