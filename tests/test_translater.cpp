#include "translater.h"
#include "vm/vm.h"

#include <glog/logging.h>
#include <iomanip>
#include <iostream>
using namespace std;
using namespace moon;

void resp_to_vdbop() {
  Translater t;
  auto command = "*1\r\n$3\r\nGET\r\n$5\r\nhello\r\n$5\r\nworld\r\n";
  auto x = t.resp_request_to_vdbop(command);
  for (auto &op : x) {
    cout << "opcode:" << char('0' + op.get_opcode()) << "| ";
    auto args = op.get_parameters();
    for (auto &arg : args)
      cout << arg << " ";
    cout << endl;
  }
}

void dbmessage_to_resp() {
  auto x =
      VmMessage(VmMessage::MessageType::ERROR_SET, string("fail to set key."));
  Translater t;
  auto words = t.message_to_resp_response(x);
  cout << words;
}

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  dbmessage_to_resp();
  return 0;
}