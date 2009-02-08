#include <winsock2.h>
#include <stdio.h>


#define PORT 5500
#define BUFFER_SIZE  100

#pragma comment (lib, "ws2_32.lib") 



int main()
{
	WSADATA wsdata;

	if( WSAStartup( MAKEWORD(2, 2), &wsdata ) != 0 )
	{
		printf("error : fail to initialize winsock");
		return false;
	}

	SOCKET serverSocket = socket( AF_INET, SOCK_STREAM, 0); // SOCK_STREAM( TCP ) , SOCK_DGRAM ( UDP )


	SOCKADDR_IN  serverAdd;
	ZeroMemory( &serverAdd, sizeof(serverAdd) );
	serverAdd.sin_family = AF_INET;						// socket's address type
	serverAdd.sin_port = htons( PORT );					// assign a port to socket
	serverAdd.sin_addr.s_addr = htons( INADDR_ANY );	// assign ip address

	
	if( bind(serverSocket, (SOCKADDR * )&serverAdd, sizeof( serverAdd )) == SOCKET_ERROR )
	{
		// bind() assign IP and PORT to socket, so we can listen to the comiming signal.
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

	char data[BUFFER_SIZE+1];
	recv( clientSocket, data, BUFFER_SIZE, 0);
	
	printf("connected to client, data:%s", data);
	strcpy_s(data, "server data");

	send( clientSocket, data, BUFFER_SIZE+1, 0);
	closesocket(clientSocket);
	WSACleanup();

	return true;
}