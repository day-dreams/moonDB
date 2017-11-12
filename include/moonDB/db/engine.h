#ifndef ENGINE
#define ENGINE

#include <map>
#include <memory>
#include <string>

using std::string;
using std::shared_ptr;
using std::map;

namespace moon {

/*
  存储引擎
*/
class StorageMachine {
public:
  bool existed(string &key);
  bool add(string &key, string &value);
  bool del(string &key);
  string &get(string &key);

public:
  static string NO_SUCH_ELE; /* do not modify this in code! */
private:
  map<string, string> storage;
};

class BaseEngine {
public:
  const static string NO_SUCH_KEY;
  /*
    判断是否存在key;
    返回是否存在
    */
  bool existed(string &key);

  /*
    添加一个key-value，若key存在则覆盖;
    返回成功/失败
    */
  bool add(string &key, string &value);

  /*
    删除一个key-value;
    返回成功/失败
    */
  bool del(string &key);

  /*
    获取key对应的value，失败则返回空字符串;
    返回拷贝后的value
    */
  string get(string &key);

  /*
    获取单例
    */
  static shared_ptr<BaseEngine> getInstance() {
    // TODO:改进到线程安全版本
    if (onlyInstance == nullptr) {
      auto p = new BaseEngine();
      onlyInstance = shared_ptr<BaseEngine>(p);
    }
    return onlyInstance;
  }

private:
  StorageMachine storage;
  /*
    单例模式
  */
  BaseEngine() {}
  BaseEngine(const BaseEngine &) {}
  BaseEngine &operator=(const BaseEngine &) {}
  static shared_ptr<BaseEngine> onlyInstance;
};
}
#endif