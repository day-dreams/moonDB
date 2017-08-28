#include "vm/vm.h"
#include "opcode.h"

#include <iostream>
#include <list>
#include <mutex>

using std::cout;
using std::list;
using std::mutex;
using std::lock_guard;
using std::unique_lock;

namespace moon {

VmMessage::VmMessage(MessageType type, string &details) {
  this->type = type;
  this->details = details;
}

VmMessage::VmMessage(MessageType type, string &&details) {
  this->type = type;
  this->details = details;
}
VmMessage::VmMessage(MessageType type, char *details) {
  this->type = type;
  this->details = details;
}

const string &VmMessage::get_details() { return this->details; }
const VmMessage::MessageType &VmMessage::get_type() { return this->type; }

bool VmMessage::is_error() {
  if (type == ERROR || type == ERROR_DEL || type == ERROR_SET ||
      type == ERROR_NOSUCHKEY)
    return true;
  else
    return false;
}
bool VmMessage::is_info() {
  if (type == INFO || type == INFO_KEYEXIST || type == INFO_KEYEXIST ||
      type == INFO_KEYNOTEXIST || type == INFO_SETSUCCEEDED) {
    return true;
  } else
    return false;
}

VmMessage VirtualMachine::execute(VdbOp &operation) {
  cout << "42:in_multy_execute:" << in_multy_execute << '\n';
  unique_lock<mutex> guard(db_mutex, std::defer_lock);
  cout << "42:in_multy_execute:" << in_multy_execute << '\n';
  if (!in_multy_execute)
    guard.lock();
  auto parameters = operation.get_parameters();
  switch (operation.get_opcode()) {
  case OPCODE::SET: {
    auto succeeded = db.get()->add(parameters[0], parameters[1]);
    if (succeeded) {
      return VmMessage(VmMessage::MessageType::INFO_SETSUCCEEDED,
                       string("Ok."));
    } else {
      return VmMessage(VmMessage::MessageType::ERROR_SET,
                       string("fail to set key."));
    }
  }
  case OPCODE::GET: {
    auto value = db.get()->get(parameters[0]);
    if (value == BaseEngine::NO_SUCH_KEY) {
      return VmMessage(VmMessage::MessageType::ERROR_NOSUCHKEY,
                       string("key dont exist."));
    } else {
      return VmMessage(VmMessage::MessageType::INFO, std::move(value));
    }
  }
  case OPCODE::DEL: {
    auto succeeded = db.get()->del(parameters[0]);
    if (succeeded) {
      return VmMessage(VmMessage::MessageType::INFO, string("Ok."));
    } else {
      return VmMessage(VmMessage::MessageType::ERROR_DEL,
                       string("fail to del key."));
    }
  }
  case OPCODE::EXISTED: {
    auto existed = db.get()->existed(parameters[0]);
    if (existed)
      return VmMessage(VmMessage::MessageType::INFO_KEYEXIST,
                       string("key exists"));
    else
      return VmMessage(VmMessage::MessageType::INFO_KEYNOTEXIST,
                       string("key not exist"));
  }
  }
}

list<VmMessage> VirtualMachine::execute(list<VdbOp> &operations) {
  lock_guard<mutex> guard(in_multy_execute_mutex);
  in_multy_execute = true;
  cout << "89:in_multy_execute:" << in_multy_execute << '\n';
  list<VmMessage> r;
  for (auto &op : operations) {
    auto message = execute(op);
    r.push_back(message);
    if (message.is_error()) {
      return r;
    }
  }
  in_multy_execute = false;
  return r;
}
}