#ifndef LOCKFREEQUEUE
#define LOCKFREEQUEUE

#include <memory>
#include <mutex>
#include <queue>
#include <semaphore.h>
using std::make_shared;
using std::mutex;
using std::queue;
using std::shared_ptr;

namespace moon
{
/*
    无锁队列,简单封装std::queue

*/
template <typename T> class LockFreeQueue
{
      private:
	queue<T> _queue;
	mutex lock, full;
	sem_t client_arrived;
	int maxnum;

      public:
	LockFreeQueue()
	{
		sem_init(&client_arrived, 0, 0);
		maxnum = 30;
	}
	LockFreeQueue(int maxnum)
	{
		sem_init(&client_arrived, 0, 0);
		this->maxnum = maxnum;
	}

	/*
	    push 向队列中加入一个元素
	*/
	void push(const T &ele)
	{

		full.lock(); // if queue is full,this will block until pop()
			     // unlcok it

		lock.lock();
		_queue.push(ele);
		if (_queue.size()
		    == maxnum) { // queue is full,so hold full mutex
		} else {	 // or,just unlock it
			full.unlock();
		}
		lock.unlock();

		sem_post(&client_arrived);
	}

	/*
	  pop 从队列中取出一个元素,如果队列为空则陷入等待
	*/
	T pop()
	{
		sem_wait(&client_arrived);

		lock.lock();

		if (_queue.size()
		    == maxnum) // queue is not full anymore,so unlock it
			full.unlock();
		auto result = _queue.front();
		_queue.pop();

		lock.unlock();

		return result;
	}

	/*
	  try_pop 尝试从队列中取出一个元素,如果队列为空则立即返回
	*/
	shared_ptr<T> try_pop()
	{
		auto rvalue = sem_trywait(&client_arrived);
		if (rvalue == 0) {
			lock.lock();
			if (_queue.size()
			    == maxnum) // queue is not full anymore,so unlock it
				full.unlock();
			auto result = make_shared<T>(_queue.front());
			_queue.pop();
			lock.unlock();
			return result;
		} else {
			return shared_ptr<T>();
		}
	}

	/*
	  size 返回队列元素个数
	*/
	size_t size()
	{
		lock.lock();
		auto size = _queue.size();
		lock.unlock();
		return size;
	}
};
}
#endif
