#ifndef LOCKFREEQUEUE
#define LOCKFREEQUEUE

#include <memory>
#include <mutex>
#include <queue>
#include <semaphore.h>
using std::queue;
using std::mutex;
using std::shared_ptr;

namespace moon {
/*
    无锁队列,简单封装std::queue

*/
template <typename T> class LockFreeQueue {
private:
  queue<T> _queue;
  mutex lock, full;
  sem_t client_arrived;
  int maxnum;

public:
  LockFreeQueue();
  LockFreeQueue(int maxnum);

  /*
      push 向队列中加入一个元素
  */
  void push(const T &ele);

  /*
    pop 从队列中取出一个元素,如果队列为空则陷入等待
  */
  T pop();

  /*
    pop 尝试从队列中取出一个元素,如果队列为空则立即返回
  */
  shared_ptr<T> try_pop();

  /*
    size 返回队列元素个数
  */
  size_t size();
};
}
#endif
