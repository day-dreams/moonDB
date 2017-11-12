#ifndef TCPCLIENT
#define TCPCLIENT

#include "InetAddr.h"
#include "types.h"
#include <string>

using std::string;

namespace moon {

class TcpClient {
public:
  enum StateCode {
    INFO_Ok,
    ERROR_SOCK_CreateFail,
    ERROR_SOCK_ConnectFail,
    ERROR_SEND_ConNotBuild,
    ERROR_SEND_Fail,
  };

  /*
   构造函数,不建立连接
   */
  TcpClient(string host, u16 port);

  /*
   析构函数,负责关闭连接
  */
  ~TcpClient();

  /*
   build_connection 与服务器建立连接
  */
  int build_connection();

  /*
    send_request 向连接发送请求
   */
  int send_request(string words);

  /*
    recv_response 从连接中接受响应
   */
  string recv_response();

private:
  Ipv4Addr server_addr;
  bool sock_shutdown = false;
  bool sock_create = false;
  bool conn_build = false;
  const int buffer_size = 1024;
  char *buffer = nullptr;
  int sock_fd = 0;

  char *create_buffer();
};
}

#endif