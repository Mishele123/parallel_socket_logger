#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define defaultPort "7777"


int main()
{
	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		std::cout << "Error code: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}

	struct addrinfo* result = nullptr, * ptr = nullptr, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo("127.0.0.1", defaultPort, &hints, &result);

	if (iResult != 0)
	{
		std::cout << "Error: " << iResult << std::endl;
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		std::cout << "Error create socket: " << WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	

	while (ptr)
	{
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult != SOCKET_ERROR) break;
		ptr = ptr->ai_next;
	}
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "Error connect socket: " << WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	while (true)
	{
		char* s = "qwerty";
		iResult = send(ConnectSocket, s, (int)sizeof(s), 0);
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "Error while sending" << WSAGetLastError();
		}
		std::cout << "Bytes send: " << iResult << std::endl;
		Sleep(5000);
	}
	return 0;
}