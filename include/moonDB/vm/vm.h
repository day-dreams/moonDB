#ifndef VM
#define VM

#include "VdbOp.h"
#include "db/engine.h"

#include <list>
#include <memory>
#include <mutex>
#include <string>

using std::list;
using std::mutex;
using std::shared_ptr;
using std::string;

namespace moon
{

/*
  虚拟机返回的消息类
    INFO:       普通字符串
    ERROR:      有特定格式的error字符串
    INTEGERS:   数字字符串
*/
class VmMessage
{
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

	/*
	  constructor
	 */
	VmMessage(MessageType type, string &details);

	/*
	  constructor,steal details
	 */
	VmMessage(MessageType type, string &&details);

	/*
	  constructor
	 */
	VmMessage(MessageType type, char *details);

	/*
	  is_error 判断本条消息是否为error消息
	 */
	bool is_error();

	/*
	  is_info 判断本条消息是否为info消息
	 */
	bool is_info();

	/*
	  get_details return const reference to inner details
	 */
	const string &get_details();

	/*
	  get_type return const reference to inner message type
	 */
	const MessageType &get_type();

      private:
	MessageType type;
	string details;
};

/*
  虚拟机类，抽象层次处于server(translater)和DataBase之间
 */
class VirtualMachine
{
      public:
	/*
	  唯一构造函数,在单例模式下获得对BaseEngine的智能指针
	 */

	VirtualMachine()
	{
		this->db = BaseEngine::getInstance();
	}

	/*
	  execute 执行传入的操作,返回消息,并保证本条operation的原子性
	*/
	VmMessage execute(VdbOp &operation);

	/*
	  execute 执行传入的一系列操作,返回消息,并保证每条operation的原子性
	*/
	list<VmMessage> execute(list<VdbOp> &operations);

      private:
	shared_ptr<BaseEngine> db;
	mutex db_mutex;		       /* 线程安全 */
	bool in_multy_execute = false; /* 由execute函数负责修改 */
	mutex in_multy_execute_mutex;  /* 线程安全 */
};
}

#endif