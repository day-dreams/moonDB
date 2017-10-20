/*
    worker.h定义了一个worker逻辑

[INFO]
    1.从连接队列中取出连接，单独负责连接代表的客户业务。

    2.一个worker可以负责有限个客户连接

    3.worker为每个客户连接设置超时时限，超时则关闭连接

    4.worker使用select处理所管辖的客户socket事件


[BUG]
    5. 由于无法确定客户的业务规模，会造成多个worker负载不均衡，
    进而导致cpu负载不均衡，需要调整

    6.由于TCP是流式的，每次recv的数据都要做解析工作，
    当前的实现假定每次recv拿到的数据包含一个独立请求。
    将来需要添加解析逻辑。
*/
#include "LockFreeQueue.h"
#include "VdbOp.h"
#include "vm/vm.h"
#include <chrono>
#include <list>
#include <memory>
#include <sys/poll.h>
using std::shared_ptr;
using std::list;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::seconds;

namespace moon {
class ClientSock {
public:
  ClientSock();
  ClientSock(int sock);
  ~ClientSock();

  int get_sock();                         /* get internal sockfd */
  bool is_timeout(const seconds &period); /* timeout or not */
  void refresh_timeout();                 /* reset last refresh time */

private:
  int sock_fd;                                /* 用户socket描述符 */
  time_point<system_clock> last_refresh_time; /* 最近一次活动时间 */
};

class Worker {
public:
  Worker(LockFreeQueue<int> &external, int maxclients, VirtualMachine &vm);
  ~Worker();

  /* event loop:

    while(true){

    if(external中有更多的客户连接待服务)
        pick some into internal

    if(internal中有超时客户连接)
        destroy them

    select readysocket from internal
    serve them,and refresh timeout
    }
  */
  void run();

private:
  list<ClientSock> internal_client_sockets;
  LockFreeQueue<int> &external_clints;
  VirtualMachine &vm;      /* 全局唯一的虚拟机对象  */
  int maxclients;          /* max num of client connection to serve */
  seconds timeout_periods; /* timeout  */
  pollfd *old = nullptr;   /* sockfds to poll */

  /* TODO: return multy mesages
  serve operations from  client
  */
  list<VmMessage> serve(list<VdbOp> operations);

  /* udpate poolfd */
  pollfd *get_poolfds();
};
}