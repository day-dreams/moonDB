#include "opcode.h"

namespace moon {
u8 str_to_opcode(const char *begin, const char *end) {
  switch (*begin) {
  case 'S':
    return OPCODE::SET;
  case 'G':
    return OPCODE::GET;
  case 'D':
    return OPCODE::DEL;
  case 'E':
    return OPCODE::EXISTED;
  }
}
}