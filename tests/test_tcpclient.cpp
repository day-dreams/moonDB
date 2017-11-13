#include "client/TcpClient.h"
#include <iostream>
#include <string>

using namespace std;
using namespace moon;

int main(int argc, char **argv) {
  TcpClient client("127.0.0.1", 8000);
  int flag;
  flag = client.build_connection();
  if (argc < 2)
    client.send_request("hello world");
  else
    client.send_request(string(argv[1]));
  auto back_words = client.recv_response();
  cout << "from server: " << back_words << endl;

  return 0;
}