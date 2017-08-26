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

class Translater {
  /*
    translater，负责下列转换工作：
    resp(request) -> VdbOps
    VmMessages -> resp(response)
  */
public:
  string message_to_resp_response(
      VmMessage &message); /* convert message from vm to resp(response) */
  list<VdbOp> resp_request_to_vdbop(
      const char *const request); /* convert resp(request) to Vdbops */
};
}

#endif