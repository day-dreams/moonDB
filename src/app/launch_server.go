package main

import "server"

func main() {
	dbServer := server.GenerateTCPServer(8080, "0.0.0.0", server.Echo)
	dbServer.Run()
}
