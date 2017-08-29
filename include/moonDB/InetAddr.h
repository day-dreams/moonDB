#include "types.h"
#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>

using std::string;

namespace moon {

class Ipv4Addr {

public:
  Ipv4Addr(const char *const ip, u16 port);
  Ipv4Addr(const sockaddr *const address);
  sockaddr convert_to_sockaddr();

  string get_ip();
  u16 get_port();

private:
  sockaddr_in addr;
};
}