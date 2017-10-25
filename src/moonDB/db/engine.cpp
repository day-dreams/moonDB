#include "db/engine.h"
#include "logging.h"
#include <string>

using std::string;
using namespace moon::logging;

namespace moon {

string StorageMachine::NO_SUCH_ELE("nil");
const string BaseEngine::NO_SUCH_KEY(
    "magic string,849u3qrkjijfdsf1e312e"); /* magic string,用于标识null */

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

shared_ptr<BaseEngine> BaseEngine::onlyInstance(new BaseEngine());

bool BaseEngine::add(string &key, string &value) {
  auto succeeded = storage.add(key, value);
  if (succeeded) {
    log(string("添加元素") + key + string(":") + value, INFO);
  } else {
    log(string("添加错误") + key + string(":") + value, ERROR);
  }
  return succeeded;
}

bool BaseEngine::del(string &key) {
  auto succeeded = storage.del(key);
  if (succeeded) {
    log(string("删除元素\t") + key, INFO);
  } else {
    log(string("删除失败\t") + key, ERROR);
  }
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