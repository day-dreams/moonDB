#ifndef INETADDR
#define INETADDR

#include "types.h"
#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>

using std::string;

namespace moon {

class Ipv4Addr {

public:
  Ipv4Addr();
  Ipv4Addr(const char *const ip, u16 port);
  Ipv4Addr(const sockaddr &address);

  /*
    convert_to_sockaddr convert inner ipv4 address(ip,port) to sockaddr
    structure
   */
  sockaddr convert_to_sockaddr();

  /*
    get_ip return an ip string
   */
  string get_ip();

  /*
    get_port return a port in u16
   */
  u16 get_port();

private:
  sockaddr_in addr;
};
}

#endif