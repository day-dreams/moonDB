#include "../include/VdbOp.h"

namespace moon {
void VdbOp::set_opcode(u8 &op) { this->opcode = op; }
void VdbOp::add_parameters(string &parameter) {
  this->parameters.push_back(parameter);
}
void VdbOp::add_parameters(string &&parameter) {
  this->parameters.push_back(parameter);
}
}