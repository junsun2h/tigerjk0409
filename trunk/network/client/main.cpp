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

	SOCKET clientSocket = socket( AF_INET, SOCK_STREAM, 0); // SOCK_STREAM( TCP ) , SOCK_DGRAM ( UDP )


	SOCKADDR_IN  serverAdd;
	ZeroMemory( &serverAdd, sizeof(serverAdd) );
	serverAdd.sin_family = AF_INET;						// socket's address type
	serverAdd.sin_port = htons( PORT );					// assign a port to socket
	serverAdd.sin_addr.s_addr = inet_addr("127.0.0.1");	// assign ip address


	if( connect( clientSocket, (SOCKADDR*)(&serverAdd), sizeof(serverAdd)) != 0 )
	{
		printf("error : fail to connect server\n");
		return false;
	}

	char data[BUFFER_SIZE+1];
	strcpy_s( data, "client\n");          
	send( clientSocket, data, strlen(data) + 1, 0);

	recv( clientSocket, data, BUFFER_SIZE, 0);

	printf("connected to server, data:%s", data);
	closesocket(clientSocket);
	WSACleanup();

	return true;
}