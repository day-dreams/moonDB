#include "client/TcpClient.h"
#include "exception"
#include "translater.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <unistd.h>
using std::cin;
using std::count;
using std::cout;
using std::endl;
using std::exception;
using std::list;
using std::string;
using std::to_string;

enum {
  state_0, /* init */
  state_1,
  state_2,
  state_3 /* end */
};

list<string> remove_spaces(string raw) {
  list<string> rv;
  auto begin = raw.begin();
  auto state = state_0;
  auto str_begin = raw.begin();

  while (begin != raw.end()) {
    switch (state) {
    case state_0:
      if (*begin == ' ')
        state = state_1;
      else
        state = state_2;
      break;
    case state_1:
      if (*begin == ' ')
        ;
      else {
        str_begin = begin;
        state = state_2;
      }
      break;
    case state_2:
      if (*begin == ' ') { // we got a str!
        rv.push_back(string(str_begin, begin));
        state = state_1;
      } else {
      }
      break;
    }
    ++begin;
  }
  rv.push_back(string(str_begin, begin));
  return rv;
}

string format_resp(const string &token) {
  return "$" + to_string(token.length()) + "\r\n" + token + "\r\n";
}

/*
  format format string from stdin into resp string
 */
string format(string raw) {
  string rv = "*1\r\n";
  auto tokens = remove_spaces(raw);
  auto size = tokens.size();
  if (size == 0)
    throw exception();
  else {
    for (auto &x : tokens)
      rv += format_resp(x);
  };

  return rv;
}

int main(int argc, char **argv) {
  moon::TcpClient client("127.0.0.1", 8888);
  client.build_connection();
  moon::Translater t;
  string buffer;
  while (true) {
    std::getline(cin, buffer);
    auto resp = format(buffer);
    auto num = client.send_request(resp);
    auto response = client.recv_response();
    cout << response << endl;
  }

  return 0;
}