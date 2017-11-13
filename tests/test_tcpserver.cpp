#include "InetAddr.h"
#include "server/TcpServer.h"
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;
using namespace moon;

void echo(int clientscok, Ipv4Addr addr) {
  char buffer[1024];
  memset(buffer, '\0', 1024);
  auto n = recv(clientscok, buffer, 1024, 0);
  cout << "from client:"
       << addr.get_ip() + ":" + to_string(addr.get_port()) + " " +
              string(buffer)
       << endl;
  send(clientscok, buffer, n, 0);
  close(clientscok);
}

int main(int argc, char **argv) {
  TcpServer server;
  server.set_port(atoi(argv[1]));
  server.set_on_request_recv(echo);
  auto r = server.start_loop();
  cout << TcpServer::decode_state_code(r) << endl;
  return 0;
}