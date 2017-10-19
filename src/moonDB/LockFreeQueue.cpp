#include "LockFreeQueue.h"
#include <mutex>
#include <queue>
#include <semaphore.h>
using std::queue;
using std::mutex;

namespace moon {

LockFreeQueue::LockFreeQueue() {
  sem_init(&client_arrived, 0, 0);
  maxnum = 30;
}
LockFreeQueue::LockFreeQueue(int maxnum) {
  sem_init(&client_arrived, 0, 0);
  this->maxnum = maxnum;
}

void LockFreeQueue::push(const T &ele) {

  full.lock(); // if queue is full,this will block until pop() unlcok it

  lock.lock();
  _queue.push(ele);
  if (_queue.size() == maxnum) { // queue is full,so hold full mutex
  } else {                       // or,just unlock it
    full.unlock();
  }
  lock.unlock();

  sem_post(&client_arrived);
}
T LockFreeQueue::pop() {
  sem_wait(&client_arrived);

  lock.lock();

  if (_queue.size() == maxnum) // queue is not full anymore,so unlock it
    full.unlock();
  auto result = _queue.front();
  _queue.pop();

  lock.unlock();

  return result;
}
size_t LockFreeQueue::size() {
  lock.lock();
  auto size = _queue.size();
  lock.unlock();
  return size;
}
}