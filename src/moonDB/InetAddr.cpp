#include "InetAddr.h"

namespace moon
{

Ipv4Addr::Ipv4Addr()
{
	this->addr.sin_family = AF_INET;
	auto ip = "0.0.0.0";
	auto succeed = inet_pton(AF_INET, ip, &this->addr.sin_addr);
	this->addr.sin_port = htons(8000);
}
Ipv4Addr::Ipv4Addr(const char *const ip, u16 port)
{
	this->addr.sin_family = AF_INET;
	auto succeed = inet_pton(AF_INET, ip, &this->addr.sin_addr);
	/* 暂时不考虑抛出异常 */
	this->addr.sin_port = htons(port);
}
Ipv4Addr::Ipv4Addr(const sockaddr &address)
{
	this->addr = *(sockaddr_in *)&address;
}

sockaddr Ipv4Addr::convert_to_sockaddr()
{
	sockaddr r = *(sockaddr *)&addr;
	return r;
}

string Ipv4Addr::get_ip()
{
	char unknown_ip[16];
	inet_ntop(AF_INET, &addr.sin_addr, unknown_ip, 16);
	return string(unknown_ip);
}

u16 Ipv4Addr::get_port()
{
	return ntohs(addr.sin_port);
}
}