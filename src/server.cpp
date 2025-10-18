#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define defaultPort "7777"


void* workWithClient(SOCKET ListenSocket)
{
	// threads
	SOCKET ClientSocket;
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) 
	{
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return nullptr;
	}
	
	char buffer[512];
	std::string normal_string;
	int iResult;
	while (true)
	{
		iResult = recv(ClientSocket, buffer, sizeof(buffer), 0);
		if (iResult > 0)
		{
			std::cout << "Bytes received: " << iResult << std::endl;
			normal_string = buffer;
			std::cout << normal_string << std::endl;
		}
	}
	closesocket(ClientSocket);
	WSACleanup();
}


int main() 
{
	WSADATA wsaData;
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		std::cout << "Error code: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	iResult = getaddrinfo(NULL, defaultPort, &hints, &result);
	if (iResult != 0) 
	{
		std::cout << "Get addrinfo error: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}
	
	// Create a SOCKET for the server to listen for client connections.
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) 
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);
	
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// threads
	workWithClient(ListenSocket);

	return 0;
}
