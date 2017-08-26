#ifndef VM
#define VM

#include "VdbOp.h"
#include "db/engine.h"

#include <list>
#include <mutex>
#include <string>

using std::string;
using std::mutex;
using std::list;

namespace moon {

class VmMessage {
  /*
    虚拟机返回的消息类
    INFO:       普通字符串
    ERROR:      有特定格式的error字符串
    INTEGERS:   数字字符串
  */
public:
  enum MessageType {
    INFO,
    ERROR,
    INTEGERS,
    INFO_KEYEXIST,
    INFO_KEYNOTEXIST,
    INFO_SETSUCCEEDED,
    ERROR_NOSUCHKEY,
    ERROR_SET,
    ERROR_DEL
  };
  VmMessage(MessageType type, string &details);  /* constructor */
  VmMessage(MessageType type, string &&details); /* constructor,steal details */
  VmMessage(MessageType type, char *details);    /* constructor */
  bool is_error();                               /* 判断是否为error消息*/
  const string &get_details();                   /* 获取details */
  const MessageType &get_type();                 /* 获取type */
private:
  MessageType type;
  string details;
};

class VirtualMachine {
  /*
    虚拟机类，抽象层次处于server(translater)和DataBase之间
  */
public:
  VmMessage
  execute(VdbOp &operation); /* 保证operation是原子操作，执行操作，返回消息*/
  list<VmMessage>
  execute(list<VdbOp>
              &operations); /*保证本次operations是原子操作，执行操作，返回消息*/

private:
  BaseEngine db;
  mutex db_mutex;                /* 线程安全 */
  bool in_multy_execute = false; /* 由execute函数负责修改 */
  mutex in_multy_execute_mutex;  /* 线程安全 */
};
}

#endif