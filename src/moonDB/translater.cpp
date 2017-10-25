#include "translater.h"
#include "logging.h"
#include "types.h"
#include <cstdlib>
#include <cstring>/* for strlen */
#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::to_string;
using namespace moon::logging;

namespace moon {
string Translater::message_to_resp_response(VmMessage &message) {
  /* words作为函数局部对象,返回时会被编译器优化,避免复制 */
  if (message.is_error()) { /* error */
    auto words = "- " + to_string(message.get_type()) + " " +
                 message.get_details() + "\r\n";
    return words;
  } else if (message.get_type() ==
             VmMessage::MessageType::INTEGERS) { /* integers */
    auto words = ": " + message.get_details() + "\r\n";
    return words;
  } else if (message.is_info()) { /* info */
    auto words = "+ " + message.get_details() + "\r\n";
    return words;
  } else {
    /* TODO: bulk strings $,arrays **/
    return "TODO";
  }
}

i32 str_to_int(const char *begin, const char *end, i32 &size_read) {
  /*
    base=10;
    如果遇到非数字字符串,退出
   */
  i32 x = 0;
  size_read = 0;
  while (begin != end && *begin <= '9' && *begin >= '0') {
    ++size_read;
    x = x * 10 + (*begin - '0');
    ++begin;
  }
  return x;
}

list<VdbOp> Translater::resp_request_to_vdbop(const char *const request) {
  /* 这里的输入参数是c_str,因为sockets只能支持c_str,再转化为string就太复杂了 */
  list<VdbOp> r;
  i32 index = 0;
  i32 array_len = 0;
  i32 size_read = 0; /* str 转 int时读取了多少字符 */
  i32 args_read = 0; /* 当前命令已经读了过少个参数 */
  bool is_reading_opcode = true;
  // cout << __LINE__ << index << "\n";

  while (request[index] != '\0') {
    // cout << __LINE__ << index << "\n";
    switch (request[index]) {
    case '*': { /* head of an array */
      auto begin = index + 1;
      auto end = begin;
      while (request[end] != '\r') {
        ++end;
      }
      // now the interger is at [begin,end], end+1 is at '\r'
      array_len = str_to_int(request + begin, request + end, size_read);
      index = end + 2; /* skip '\r\n' */
      /* now index is at begging of a command,or '\0'*/
      break;
    }
    case '$': { /* readign some part of a command */
      auto begin = index + 1;
      auto end = begin;
      while (request[end] != '\r') {
        ++end;
      }
      // now integer is in [begin,end], end+1 may not  at '\r'
      auto bulk_str_len = str_to_int(request + begin, request + end, size_read);
      index = begin + size_read +
              2; // now index is at a new bulk string(size:bulk_str_len)
      if (is_reading_opcode) {
        is_reading_opcode = false;
        r.push_back(VdbOp());
        auto opcode = OPCODE::str_to_opcode(
            request + index, request + index + bulk_str_len, size_read);
        r.back().set_opcode(opcode);
      } else {
        auto str = request + index;
        r.back().add_parameters(string(str, bulk_str_len));
      }
      index = index + bulk_str_len +
              2; // now index is at a new bulk string, or '\0'
      break;
    }
    default: { /* wrong input */
      logging::log("wrong input: pos(" + to_string(index) + ") " +
                       string(request),
                   logging::ERROR);
      return list<VdbOp>();
    }
    }
  }
  return r;
}
}