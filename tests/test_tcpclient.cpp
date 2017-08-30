#include "client/TcpClient.h"
#include <iostream>
#include <string>

using namespace std;
using namespace moon;

int main() {
  TcpClient client("127.0.0.1", 8000);
  int flag;
  flag = client.build_connection();
  client.send_request("hello world");
  auto back_words = client.recv_response();
  cout << "from server: " << back_words << endl;

  return 0;
}