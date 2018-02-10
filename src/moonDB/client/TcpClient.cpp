#include "client/TcpClient.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using std::string;

namespace moon
{
TcpClient::TcpClient(string host, u16 port)
{
	server_addr = Ipv4Addr(host.c_str(), port);
}
TcpClient::~TcpClient()
{
	if (!sock_shutdown)
		close(sock_fd);
	if (!buffer)
		delete[](buffer);
}

int TcpClient::build_connection()
{
	this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
		return StateCode::ERROR_SOCK_CreateFail;
	sock_create = true;
	auto sock_addr = server_addr.convert_to_sockaddr();
	int succeed = 0;
	succeed = connect(sock_fd, &sock_addr, sizeof(sock_addr));
	if (succeed == -1)
		return StateCode::ERROR_SOCK_ConnectFail;
	else {
		conn_build = true;
		return StateCode::INFO_Ok;
	}
}

int TcpClient::send_request(string words)
{
	if (!conn_build)
		return ERROR_SEND_ConNotBuild;
	auto size = send(sock_fd, words.c_str(), words.size(), 0);
	if (size != words.size())
		return StateCode::ERROR_SEND_Fail;
	return StateCode::INFO_Ok;
}

char *TcpClient::create_buffer()
{
	// auto r = new char[buffer_size];
	auto r = (char *)malloc(buffer_size);
	return r;
}

string TcpClient::recv_response()
{
	if (buffer == nullptr)
		buffer = create_buffer();
	memset((void *)buffer, 0, buffer_size);
	auto size = recv(sock_fd, buffer, buffer_size, 0);
	return string(buffer);
}
}