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
  TcpClient(string host, u16 port); /* 构造函数不建立连接 */
  ~TcpClient();                     /* 析构函数负责关闭连接 */

  int build_connection();         /* 与服务器建立连接 */
  int send_request(string words); /* 发送请求 */
  string recv_response();         /* 接收响应 */

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