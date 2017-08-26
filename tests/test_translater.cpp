#include "translater.h"

#include <glog/logging.h>
#include <iostream>

using namespace std;
using namespace moon;

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  Translater t;
  auto command = "*1\r\nSET\r\n$5\r\nhello\r\n$5\r\nworld\r\n";
  auto x = t.resp_request_to_vdbop(command);
  for (auto &op : x) {
    cout << op.get_opcode() << " ";
    auto args = op.get_parameters();
    for (auto &arg : args)
      cout << arg << " ";
    cout << endl;
  }
  return 0;
}