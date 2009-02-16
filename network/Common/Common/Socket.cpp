#include "Socket.h"



WSADATA* SocketMgr::Init()
{
	if( WSAStartup( MAKEWORD(2, 2), &m_wsdata ) != 0 )
	{
		printf("error : fail to initialize winsock");
		return NULL;
	}

	return &m_wsdata;
}


void SocketMgr::SetAddress(ULONG ip, int fort, SOCKADDR_IN& address)
{
	ZeroMemory( &address, sizeof(address) );
	address.sin_family = AF_INET;
	address.sin_port = htons( fort );
	address.sin_addr.s_addr = ip;	
}
