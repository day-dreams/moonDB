#include "LockFreeQueue.h"
#include <memory>
#include <mutex>
#include <queue>
#include <semaphore.h>
using std::queue;
using std::mutex;
using std::make_shared;
using std::shared_ptr;

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

shared_ptr<T> LockFreeQueue::try_pop() {
  auto rvalue = sem_trywait(&client_arrived);
  if (rvalue != EGGAIN) {
    lock.lock();
    if (_queue.size() == maxnum) // queue is not full anymore,so unlock it
      full.unlock();
    auto result = new T(_queue.front());
    _queue.pop();
    return make_shared<T>(result);
  } else {
    return make_shared<T>();
  }
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