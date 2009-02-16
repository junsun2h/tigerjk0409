#include "Socket.h"

#pragma comment (lib, "common.lib") 

int main()
{
	SocketMgr socketMgr;
	socketMgr.Init();

	SOCKET clientSocket = socket( AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN  serverAdd;
	socketMgr.SetAddress( inet_addr("127.0.0.1"), htons( SERVER_PORT ), serverAdd );

	if( connect( clientSocket, (SOCKADDR*)(&serverAdd), sizeof(serverAdd)) != 0 )
	{
		printf("error : fail to connect server\n");
		return false;
	}

	printf("connected to server\n");

	char data[BUFFER_SIZE];
	while(1)
	{
		ZeroMemory( &data, sizeof(data) );
		gets(data);
		send( clientSocket, data, 5, 0);
		recv( clientSocket, data, 5, 0);

		if( strcmp( data , "exit" ) == 0 )
		{
			break;
		}
	}

	closesocket(clientSocket);
	WSACleanup();

	return true;
}