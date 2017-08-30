#include "InetAddr.h"
#include "server/TcpServer.h"
#include <functional>
#include <iostream>
#include <string>
using namespace std;
using namespace moon;

string echo(const char *words, Ipv4Addr &addr) {
  cout << "from client:"
       << addr.get_ip() + ":" + to_string(addr.get_port()) + " " + words
       << endl;
  return words;
}

int main(int argc, char **argv) {
  TcpServer server;
  server.set_port(atoi(argv[1]));
  server.set_on_request_recv(echo);
  auto r = server.start_loop();
  cout << TcpServer::decode_state_code(r) << endl;
  return 0;
}