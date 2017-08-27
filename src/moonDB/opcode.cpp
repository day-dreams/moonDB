#include "opcode.h"
#include "types.h"

namespace moon {
u8 OPCODE::str_to_opcode(const char *begin, const char *end, i32 &size_read) {
  auto ite = begin;
  size_read = 0;
  while (*ite != '\r') {
    ++size_read;
    ++ite;
  }
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