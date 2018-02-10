#include "translater.h"
#include "logging.h"
#include "opcode.h"
#include "types.h"
#include <cstdlib>
#include <cstring>/* for strlen */
#include <iostream>
#include <string>
using std::cout;
using std::string;
using std::to_string;
using namespace moon::logging;

namespace moon
{
string Translater::message_to_resp_response(VmMessage &message)
{
	/* 利用RVO机制避免复制 */
	string words;

	if (message.is_error()) { /* error */
		words = "- " + to_string(message.get_type()) + " "
			+ message.get_details() + "\r\n";
	} else if (message.get_type()
		   == VmMessage::MessageType::INTEGERS) { /* integers */
		words = ": " + message.get_details() + "\r\n";
	} else if (message.is_info()) { /* info */
		words = "+ " + message.get_details() + "\r\n";
	} else {
		/* TODO: bulk strings $,arrays **/
		words = "TODO";
	}

	return words;
}

i32 str_to_int(const char *begin, const char *end, i32 *size_read)
{
	/*
	  base=10;
	  如果遇到非数字字符串,退出
	 */
	i32 x = 0;
	if (size_read != nullptr)
		size_read = 0;
	while (begin != end && *begin <= '9' && *begin >= '0') {
		if (size_read != nullptr)
			++size_read;
		x = x * 10 + (*begin - '0');
		++begin;
	}
	return x;
}

list<VdbOp> Translater::resp_request_to_vdbop(const char *const request)
{
	list<VdbOp> rv;
	size_t begin = 0;
	size_t array_length = 0;
	size_t operator_length = 0;
	size_t parameter_length = 0;

	/* begin of length used for array_len ,operator_len ,parameter_len */
	size_t length_begin = 0;
	/* begin of length used for operator,parameter */
	size_t string_begin = 0;

	while (state != State::over) {

		switch (state) {

		case State::init:
			if (request[begin] != '*')
				return rv;
			else {
				length_begin = ++begin;
				state = State::array_len;
			}
			break;

		case State::array_len:
			if (request[begin]
			    == '\r') { /* reach end of array_len */
				array_length =
					str_to_int(request + length_begin,
						   request + begin, nullptr);
				state = State::operator_len;
				begin += 3; /* \r\n$ */
				length_begin = begin;
			} else {
				++begin;
			}
			break;

		case State::operator_len:
			if (request[begin] == '\r') {
				operator_length =
					str_to_int(request + length_begin,
						   request + begin, nullptr);
				state = State::operator_str;
				begin += 2;
				string_begin = begin;
			} else {
				++begin;
			}
			break;

		case State::operator_str:
			if (begin - string_begin < operator_length) {
				++begin;
			} else { // TODO:we got a whole operator
				 // string,[string_begin,begin)

				rv.push_back(VdbOp());
				u8 opcode = OPCODE::str_to_opcode(
					request + string_begin, request + begin,
					nullptr);
				rv.back().set_opcode(opcode);

				begin += 3;
				state = State::parameter_len;
				length_begin = begin;
			}
			break;

		case State::parameter_len:
			if (request[begin] == '\r') {
				parameter_length =
					str_to_int(request + length_begin,
						   request + begin, nullptr);
				begin += 2;
				state = State::parameter_str;
				string_begin = begin;
			} else {
				++begin;
			}
			break;

		case State::parameter_str:
			if (begin - string_begin < parameter_length) {
				++begin;
			} else { // TODO:we got a whole parameter
				 // string,[string_begin,begin)
				rv.back().add_parameters(
					string(request + string_begin,
					       request + begin));
				begin += 2;
				state = State::branch;
			}
			break;

		case State::branch:
			if (request[begin] == '\0')
				state = State::over;
			else if (request[begin] == '$') {
				// begin of another paramter len
				++begin;
				length_begin = begin;
				state = State::parameter_len;
			} else if (request[begin] == '*') {
				// another array, we dont support this at now
				++begin;
				state = State::array_len;
			} else {
				// wrong format of resp string, impossible
				state = State::over;
			}
			break;

		case State::over:
		default:
			break;
		}
	}

	state = State::init; /* for next format */
	return rv;
}

} // namespace moon