#ifndef TRANSLATER
#define TRANSLATER

/*

    resp(Redis Serialization Protocol)

    这里采用的redis的通信协议，协议主要包括request和response两个部分，
    具体参考官方文档(https://redis.io/topics/protocol)

*/

#include "VdbOp.h"
#include "vm/vm.h"

#include <list>
#include <string>
using std::string;
using std::list;

namespace moon {

/*
  translater，负责下列转换工作：
    resp(request) -> VdbOps
    VmMessages -> resp(response)
*/
class Translater {
public:
  /*
     convert message from vm to resp(response)
  */
  string message_to_resp_response(VmMessage &message);

  /*
     convert resp(request) to Vdbops
  */
  list<VdbOp> resp_request_to_vdbop(const char *const request);

  /* TODO:
    错误的resp协议字符串输入的处理逻辑
  */

private:
  enum State {
    init,
    array_len,
    operator_len,
    operator_str,
    parameter_len,
    parameter_str,
    branch,
    over
  };
  State state = init;
};
} // namespace moon

#endif