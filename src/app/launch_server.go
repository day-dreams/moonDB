package main

import (
	"os"
	"server"
	"strconv"
)

func main() {
	port, _ := strconv.Atoi(os.Args[1])
	dbServer := server.GenerateTCPServer(port, "0.0.0.0", server.Echo)
	dbServer.Run()
}
