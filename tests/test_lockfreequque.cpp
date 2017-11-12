#include "LockFreeQueue.h"
#include <iostream>

using namespace std;
using namespace moon;
int main(int argc, char **argv) {
  LockFreeQueue<int> queue;
  queue.push(1);
  cout << queue.try_pop() << endl;
  queue.push(2);
  cout << queue.try_pop() << endl;
  queue.push(3);
  cout << queue.try_pop() << endl;
  queue.push(4);
  cout << queue.try_pop() << endl;
  cout << queue.try_pop() << endl;
  return 0;
}