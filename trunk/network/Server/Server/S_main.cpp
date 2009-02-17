#include "Socket.h"
#include <process.h>

#pragma comment (lib, "common.lib") 


unsigned __stdcall PrintWord(void* arg)
{
	SOCKET clientSocket = (SOCKET)arg;

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
	
	return true;
}


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

	while(1)
	{
		SOCKET clientSocket = accept( serverSocket, NULL, NULL);
		unsigned threadID;
		printf("client join\n");

		ULONG hTread = _beginthreadex(NULL, 0, PrintWord, (void*)clientSocket, 0, &threadID); 
	}

	WSACleanup();

	return true;
}