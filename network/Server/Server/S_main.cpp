#include "Socket.h"

#pragma comment (lib, "common.lib") 

int main()
{
	SocketMgr socketMgr;
	socketMgr.Init();

	SOCKET serverSocket = socket( AF_INET, SOCK_STREAM, 0);
	
	SOCKADDR_IN  serverAdd;
	socketMgr.SetAddress( htons( INADDR_ANY ), htons( SERVER_PORT ), serverAdd );
	
	if( bind(serverSocket, (SOCKADDR * )&serverAdd, sizeof( serverAdd )) == SOCKET_ERROR )
	{
		printf("error : socket binding");
		return false;
	}

	listen( serverSocket, SOMAXCONN );

	SOCKET clientSocket;
	if( (clientSocket = accept( serverSocket, NULL, NULL) ) == INVALID_SOCKET)
	{
		printf( "error : accept socket");
		return false;
	}

	printf("connected to client\n");

	char data[BUFFER_SIZE+1];
	while(1)
	{
		ZeroMemory( &data, sizeof(data) );
		recv( clientSocket, data, 5, 0);
		
		if( strcmp( data , "exit" ) == 0 )
		{
			printf( "exit");
			strcpy_s( data, "exit" );
			send( clientSocket, data, 5, 0);
			break;
		}
		else
		{
			printf( "%s\n", data );
			strcpy_s( data, "again" );
			send( clientSocket, data, 5, 0);
		}
	}
	
	closesocket(clientSocket);
	WSACleanup();

	return true;
}