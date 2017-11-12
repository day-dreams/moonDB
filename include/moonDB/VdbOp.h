/*
    virtual DataBase Operations(VdbOp)

        符合redis通信协议的request经过translater的翻译后，会生成相应的操作序列;
    每个操作序列都是VdbOp的有序列表，VM读取列表中的每一个VdbOp，执行相应的操作(调
    用db提供的函数)。

*/

#ifndef VDBOP
#define VDBOP

#include "opcode.h"
#include "types.h"

#include <string>
#include <vector>

using std::vector;
using std::string;

namespace moon {
class VdbOp {
public:
  /*
      get_opcode returns type of operation in u8.
   */
  const u8 &get_opcode();

  /*
      get_parameters returns a string vector,which holds parameters of operation
    in order
   */
  const vector<string> &get_parameters();

  void set_opcode(u8 &op);
  void add_parameters(string &parameter);
  void add_parameters(string &&parameter);

private:
  u8 opcode;
  vector<string> parameters;
};
}
#endif