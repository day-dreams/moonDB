#include "server/TcpServer.h"
#include "InetAddr.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using std::string;

namespace moon {
int TcpServer::start_loop() {
  // check settings, then create,set,bind,listen socket
  if (!callback)
    return TcpServer::ERROR_NoCallbackSet;
  this->server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (this->server_sock == 0)
    return TcpServer::ERROR_SocketCreateFail;
  if (address_reuse) {
    int on = 1;
    auto succeed = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR,
                              (void *)&on, sizeof(on));
    if (succeed == -1)
      return TcpServer::ERROR_SetAddrReuseFail;
  }
  if (port_reuse) {
    int on = 1;
    auto succeed = setsockopt(server_sock, SOL_SOCKET, SO_REUSEPORT,
                              (void *)&on, sizeof(on));
    if (succeed == -1)
      return TcpServer::ERROR_SetPortReuseFail;
  }
  Ipv4Addr local_address(this->bind_addr.c_str(), port_to_listen);
  auto sock_addr = local_address.convert_to_sockaddr();
  auto succeed = bind(server_sock, &sock_addr, sizeof(sock_addr));
  if (succeed != 0)
    return TcpServer::ERROR_SocketBindFail;
  succeed = listen(server_sock, max_pending_con);
  if (succeed != 0)
    return TcpServer::ERROR_SocketListenFail;

  //   waiting for connections and handle them
  sockaddr _client_addr;
  socklen_t size = sizeof(_client_addr);
  char buffer[buffer_size];
  int client_sock;
  while (true) {
    // 接受连接
    memset((void *)buffer, 0, buffer_size);
    client_sock = accept(server_sock, &_client_addr, &size);
    Ipv4Addr client_addr(_client_addr);
    // 接受数据
    auto num_recved = recv(client_sock, buffer, buffer_size, 0);
    if (num_recved == -1 && !ignore_recv_error)
      goto Handle_ERROR_SocketRecvFail;
    // 处理数据
    auto r_str = callback(buffer, client_addr);
    if (!r_str.empty()) { /* 处理后需要返回响应 */
      // 返回数据
      auto r = send(client_sock, r_str.c_str(), r_str.size(), 0);
      if (r == -1 && !ignore_send_error)
        goto Handle_ERROR_SocketSendFail;
    }
    // 默认关闭连接
    auto r = close(client_sock);
    if (r == -1 && !ignore_close_error)
      goto Handle_ERROR_SocketCloseFail;
  }

// try to handle errors
Handle_ERROR_SocketRecvFail:
  close(server_sock);
  close(client_sock);
  return TcpServer::ERROR_SocketRecvFail;
Handle_ERROR_SocketSendFail:
  close(server_sock);
  close(client_sock);
  return ERROR_SocketSendFail;
Handle_ERROR_SocketCloseFail:
  close(client_sock);
  close(server_sock);
  return TcpServer::ERROR_SocketCloseFail;
}

void TcpServer::set_on_request_recv(const CallbackType &callback) {
  this->callback = callback;
}

void TcpServer::set_port(int port) { this->port_to_listen = port; }
void TcpServer::set_max_pendding(int num) { this->max_pending_con = num; }
void TcpServer::set_address_reuse(bool reuse) { this->address_reuse = reuse; }
void TcpServer::set_bind_addr(const string &addr) { this->bind_addr = addr; }
string TcpServer::decode_state_code(int code) {
  switch (code) {
  case ERROR_NoCallbackSet:
    return "Server's callback not set yet";
  case ERROR_SocketCreateFail:
    return "Fail to create socket";
  case ERROR_SetPortReuseFail:
    return "Fail to set port reuse";
  case ERROR_SetAddrReuseFail:
    return "Fail to set address reuse";
  case ERROR_SocketBindFail:
    return "Fail to bind address";
  case ERROR_SocketListenFail:
    return "Fail to listen the socket";
  case ERROR_SocketRecvFail:
    return "Fail to recv data from socket";
  case ERROR_SocketCloseFail:
    return "Fail to close sockdet";
  case ERROR_SocketSendFail:
    return "Fail to send data";
  default:
    return "Unknown state code";
  }
}
}