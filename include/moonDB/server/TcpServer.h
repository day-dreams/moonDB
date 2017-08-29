#ifndef TCPSERVER
#define TCPSERVER

#include "types.h"
#include <functional>
#include <string>
#include <sys/socket.h>

using std::string;
using std::function;

namespace moon {

class TcpServer {
public:
  typedef function<char *(const char *const)> CallbackType; /* 回调函数类型 */
  int start_loop();        /* 开启服务器循环 */
  bool set_port(int port); /* 设置监听端口 */

  /*
  ` 设置回调函数

    要求函数接受两个参数(resp请求字符串,链接信息引用),
    返回值为resp响应字符串.
  */
  bool set_on_request_recv(CallbackType callback);

private:
  u16 port_to_listen = 8000;
  CallbackType callback;
};
}
#endif