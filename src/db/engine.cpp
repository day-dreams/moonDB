#include "../../include/db/engine.h"
#include "../../include/easylogging++.h"

#include <string>

INITIALIZE_EASYLOGGINGPP

using std::string;

namespace moon {

bool StorageMachine::existed(string &key) {
  return storage.count(key) == 0 ? false : true;
}

bool StorageMachine::add(string &key, string &value) {
  // use set as add, can be better
  storage[key] = value;
  return true;
}

bool StorageMachine::del(string &key) {
  return storage.erase(key) == 1 ? true : false;
}

string &StorageMachine::get(string &key) {
  auto ite = storage.find(key);
  if (ite == storage.end())
    return StorageMachine::NO_SUCH_ELE;
  else
    return ite->second;
}

bool BaseEngine::add(string &key, string &value) {
  auto succeeded = storage.add(key, value);
  if (succeeded)
    LOG(INFO) << "添加元素\t" << key << ":" << value;
  else
    LOG(ERROR) << "添加错误\t" << key << ":" << value;
  return succeeded;
}

bool BaseEngine::del(string &key) {
  auto succeeded = storage.del(key);
  if (succeeded)
    LOG(INFO) << "删除元素\t" << key;
  else
    LOG(ERROR) << "删除失败\t" << key;
  return succeeded;
}

bool BaseEngine::existed(string &key) { return storage.existed(key); }

string BaseEngine::get(string &key) {
  auto value = storage.get(key);
  if (value == StorageMachine::NO_SUCH_ELE) {
    return BaseEngine::NO_SUCH_KEY;
  } else {
    return value; /* copy once */
  }
}
}