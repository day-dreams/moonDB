#include "VdbOp.h"

namespace moon {
const u8 &VdbOp::get_opcode() { return this->opcode; }
const vector<string> &VdbOp::get_parameters() { return this->parameters; }
void VdbOp::set_opcode(u8 &op) { this->opcode = op; }
void VdbOp::add_parameters(string &parameter) {
  this->parameters.push_back(parameter);
}
void VdbOp::add_parameters(string &&parameter) {
  this->parameters.push_back(parameter);
}
}