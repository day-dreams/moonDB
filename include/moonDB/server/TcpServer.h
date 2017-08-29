#ifndef TCPSERVER
#define TCPSERVER

#include "InetAddr.h"
#include "types.h"
#include <functional>
#include <string>
#include <sys/socket.h>

using std::string;
using std::function;

namespace moon {

class TcpServer {
public:
  enum STATE_CODE {
    ERROR_NoCallbackSet,
    ERROR_SocketCreateFail,
    ERROR_SetPortReuseFail,
    ERROR_SetAddrReuseFail,
    ERROR_SocketBindFail,
    ERROR_SocketListenFail,
    ERROR_SocketRecvFail,
    ERROR_SocketCloseFail,
    ERROR_SocketSendFail,
  };

  typedef function<string(const char *const, const Ipv4Addr &addr)>
      CallbackType;        /* 回调函数类型 */
  int start_loop();        /* 开启服务器循环 */
  void set_port(int port); /* 设置监听端口 */
  void set_max_pendding(int num);
  void set_address_reuse(bool reuse);
  void set_bind_addr(const string &addr);
  /*
  ` 设置回调函数

    要求函数接受两个参数(resp请求字符串,链接信息引用),
    返回值为resp响应字符串.
  */
  void set_on_request_recv(const CallbackType &callback);

private:
  u16 port_to_listen = 8000;
  CallbackType callback;
  int server_sock;         /* 监听套接字 */
  int max_pending_con = 5; /* 最大排队连接数 */
  int buffer_size = 1024;
  bool address_reuse = false;
  bool ignore_recv_error = true;
  bool ignore_close_error = true;
  bool ignore_send_error = false;
  string bind_addr = "0.0.0.0";
};
}
#endif