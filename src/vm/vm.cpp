#include "../../include/vm/vm.h"
#include "../../include/opcode.h"

#include <list>
#include <mutex>

using std::list;
using std::mutex;
using std::lock_guard;
using std::unique_lock;

namespace moon {

VmMessage::VmMessage(MessageType type, string &details) {
  this->type = type;
  this->details = details;
}

VmMessage::VmMessage(MessageType type, string &&details) : details(details) {
  this->type = type;
}
VmMessage::VmMessage(MessageType type, char *details) {
  this->type = type;
  this->details = details;
}

VmMessage::VmMessage(VmMessage &&other) {
  this->type = other.type;
  this->details = std::move(other.details);
}

const string &VmMessage::get_details() { return this->details; }
const VmMessage::MessageType &VmMessage::get_type() { return this->type; }

VmMessage &VmMessage::operator=(VmMessage &&other) {
  this->type = other.type;
  this->details = std::move(other.details);
  return *this;
}

bool VmMessage::is_error() {
  if (type == ERROR || type == ERROR_DEL || type == ERROR_SET ||
      type == ERROR_NOSUCHKEY)
    return true;
  else
    return false;
}

VmMessage VirtualMachine::execute(VdbOp &operation) {
  unique_lock<mutex> guard(db_mutex);
  if (!in_multy_execute)
    guard.lock();
  auto parameters = operation.get_parameters();
  switch (operation.get_opcode()) {
  case OPCODE::SET: {
    auto succeeded = db.add(parameters[0], parameters[1]);
    if (succeeded) {
      return VmMessage(VmMessage::MessageType::INFO_SETSUCCEEDED, "Ok.");
    } else {
      return VmMessage(VmMessage::MessageType::ERROR_SET, "fail to set key.");
    }
  }
  case OPCODE::GET: {
    auto value = db.get(parameters[0]);
    if (value == BaseEngine::NO_SUCH_KEY) {
      return VmMessage(VmMessage::MessageType::ERROR_NOSUCHKEY,
                       "key dont exist.");
    } else {
      return VmMessage(VmMessage::MessageType::INFO, std::move(value));
    }
  }
  case OPCODE::DEL: {
    auto succeeded = db.del(parameters[0]);
    if (succeeded) {
      return VmMessage(VmMessage::MessageType::INFO, "Ok.");
    } else {
      return VmMessage(VmMessage::MessageType::ERROR_DEL, "fail to del key.");
    }
  }
  case OPCODE::EXISTED: {
    auto existed = db.existed(parameters[0]);
    if (existed)
      return VmMessage(VmMessage::MessageType::INFO_KEYEXIST, "key exists");
    else
      return VmMessage(VmMessage::MessageType::INFO_KEYNOTEXIST,
                       "key not exist");
  }
  }
}

list<VmMessage> VirtualMachine::execute(list<VdbOp> &operations) {
  lock_guard<mutex> guard(in_multy_execute_mutex);
  in_multy_execute = false;
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