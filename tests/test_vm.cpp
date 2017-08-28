#include "translater.h"
#include "vm/vm.h"

#include <glog/logging.h>
#include <iostream>

using namespace std;
using namespace moon;

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);

  VirtualMachine vm;
  Translater t;
  {
    auto insert = "*1\r\n$3\r\nSET\r\n$4\r\nname\r\n$8\r\nzhangnan\r\n";
    auto operations = t.resp_request_to_vdbop(insert);
    auto messages = vm.execute(operations);
    for (auto &message : messages) {
      auto words = t.message_to_resp_response(message);
      cout << words;
    }
  }
  {
    auto get = "*1\r\n$3\r\nGET\r\n$4\r\nname\r\n";

    auto opeations = t.resp_request_to_vdbop(get);
    auto messages = vm.execute(opeations);
    for (auto &message : messages) {
      auto words = t.message_to_resp_response(message);
      cout << words;
    }
  }
  return 0;
}