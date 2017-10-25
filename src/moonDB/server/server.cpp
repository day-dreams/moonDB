/*
    server.cpp 包含moonDB的服务器主程序
*/

#include "InetAddr.h"
#include "LockFreeQueue.h"
#include "logging.h"
#include "server/TcpServer.h"
#include "server/worker.h"
#include "translater.h"
#include "vm/vm.h"
#include <thread>
using namespace moon;

LockFreeQueue<int> sockqueue;
VirtualMachine vm;
Worker worker(sockqueue, 5, vm);

void server_callback(int sockfd, Ipv4Addr addr) {
  logging::log("accept a connection:" + addr.get_ip() + " " +
                   std::to_string(addr.get_port()),
               logging::INFO);
  sockqueue.push(sockfd);
  ;
}

void worker_callback() { worker.run(); }

int main(int argc, char **argv) {
  logging::set_print(true);
  short port;
  if (argc < 2)
    port = 8888;
  else
    port = atoi(argv[2]);
  TcpServer myserver;
  myserver.set_bind_addr("127.0.0.1");
  myserver.set_port(port);
  myserver.set_address_reuse(true);
  myserver.set_on_request_recv(server_callback);

  std::thread t(worker_callback);
  myserver.start_loop();

  return 0;
}