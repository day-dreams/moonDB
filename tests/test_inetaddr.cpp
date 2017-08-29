#include "InetAddr.h"
#include <iostream>
using namespace std;
int main() {
  moon::Ipv4Addr addr("1.2.4.8", 12345);
  auto x = addr.convert_to_sockaddr();
  moon::Ipv4Addr address(&x);
  cout << addr.get_ip() << ":" << addr.get_port() << endl;
  cout << address.get_ip() << ":" << address.get_port() << endl;
  return 0;
}