#include <winsock2.h>
#include <stdio.h>

#define BUFFER_SIZE 5
#define SERVER_PORT 5500

#pragma comment (lib, "ws2_32.lib") 


class SocketMgr
{
public:
	WSADATA		m_wsdata;

	WSADATA*	Init();
	void		SetAddress(ULONG ip, int fort, SOCKADDR_IN& address);
};
