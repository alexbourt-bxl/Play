#include "NetConnection.h"

#include <ws2tcpip.h>
#include <stdio.h>

NetConnection::NetConnection()
{
	OpenEvent.Sender = this;
	ReceiveDataEvent.Sender = this;
	CloseEvent.Sender = this;
	  
	m_clientSocket = INVALID_SOCKET;
}

NetConnection::~NetConnection()
{
	Stop();
}

uint NetConnection::ThreadProc()
{
	/***/ wprintf(L"Connection created\n");

	OpenEvent.Send(NetConnection::EventArgs(this, null, 0));

	const uint bufferSize = 512;
	char buffer[bufferSize];

	int bytesReceived = 1;

	while (!WasKilled && bytesReceived > 0)
	{
		bytesReceived = ::recv(
			m_clientSocket, 
			buffer, 
			bufferSize, 
			0);

		if (bytesReceived > 0)
			ReceiveDataEvent.Send(NetConnection::EventArgs(this, buffer, bytesReceived));

		if (bytesReceived == 0)
		{
			// Close the connection

			/***/ wprintf(L"Closing connection...\n");
		}

		if (bytesReceived < 0)
		{
			// Error

			/***/ wprintf(L"Error 0x%X\n", bytesReceived);
		}
	}

	::closesocket(m_clientSocket);
	::WSACleanup();

	CloseEvent.Send(NetConnection::EventArgs(this, null, 0));

	return 0;
}

bool NetConnection::Connect(const String& wcAddress)
{
	WSADATA wsaData;
	int socketResult;

	socketResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (socketResult != 0)
		return false;

	/***/ wprintf(L"Environment initialized\n");

	ADDRINFOT* result = null;
	ADDRINFOT* ptr    = null;
	ADDRINFOT  hints;

	::ZeroMemory(&hints, sizeof(hints));

	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port

	socketResult = ::GetAddrInfo(
		wcAddress,
		PLAY_PORT, 
		&hints, 
		&result);

	if (socketResult != 0)
	{
		::WSACleanup();
		return false;
	}

	/***/ wprintf(L"Resolved server address and port\n");

	ptr = result;

	// Attempt to connect to the first address returned

	m_clientSocket = ::socket(
		ptr->ai_family, 
		ptr->ai_socktype, 
		ptr->ai_protocol);

	if (m_clientSocket == INVALID_SOCKET)
	{
		::FreeAddrInfo(result);
		::WSACleanup();
		return false;
	}

	/***/ wprintf(L"Connection socket created\n");

	do
	{
		// Try to connect to the server

		/***/ wprintf(L"Trying to establish connection...");

		socketResult = ::connect(
			m_clientSocket, 
			ptr->ai_addr, 
			(int)ptr->ai_addrlen);

		if (socketResult == SOCKET_ERROR)
		{
			::closesocket(m_clientSocket);
			m_clientSocket = INVALID_SOCKET;

			// Move on to the next address

			ptr = ptr->ai_next;

			/***/ wprintf(L"\n...next address...");
		}
	}
	while (socketResult == SOCKET_ERROR && ptr);

	// Free the resources

	::FreeAddrInfo(result);

	if (m_clientSocket == INVALID_SOCKET)
	{
		::WSACleanup();
		return false;
	}

	/***/ wprintf(L"done\n");

	Start();

	return true;
}

int NetConnection::Send(char* data, int dataSize)
{
	int socketResult;

	socketResult = ::send(
		m_clientSocket, 
		data, 
		dataSize, 
		0);

	if (socketResult == SOCKET_ERROR) 
	{
		::closesocket(m_clientSocket);
		::WSACleanup();
		return -1;
	}

	return socketResult;
}

//void NetConnection::SetSocket(SOCKET socket)
//{
//	m_clientSocket = socket;
//}