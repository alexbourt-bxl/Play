#include "NetServer.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma warning(disable:4355)

NetServer::NetServer()
{
	RequestConnectionEvent.Sender = this;
	AcceptConnectionEvent.Sender = this;

	*m_connections.RemoveEvent += EventHandler(this, &NetServer::connection_RemoveEvent);

	m_listenSocket = INVALID_SOCKET;

	Start();
}

NetServer::~NetServer()
{
	Stop();
}

uint NetServer::ThreadProc()
{
	// TODO: Send connection status updates through events

	/***/ wprintf(L"Server starting...\n");

	WSADATA wsaData;
	int socketResult;

	socketResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (socketResult != 0)
		return (unsigned)socketResult;
	
	/***/ wprintf(L"Environment initialized\n");

	ADDRINFOT* result = null;
	ADDRINFOT  hints;

	::ZeroMemory(&hints, sizeof(hints));

	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags    = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	
	socketResult = ::GetAddrInfo(
		null, 
		PLAY_PORT, 
		&hints, 
		&result);

	if (socketResult != 0)
	{
		::WSACleanup();
		return (unsigned)socketResult;
	}

	/***/ wprintf(L"Local address and port resolved\n");

	m_listenSocket = ::socket(
		result->ai_family,
		result->ai_socktype,
		result->ai_protocol);

	if (m_listenSocket == INVALID_SOCKET)
	{
		::FreeAddrInfo(result);
		::WSACleanup();
		return (unsigned)INVALID_SOCKET;
	}

	/***/ wprintf(L"Listening socket created\n");

	socketResult = ::bind(
		m_listenSocket, 
		result->ai_addr, 
		(int)result->ai_addrlen);

	if (socketResult == SOCKET_ERROR)
	{
		::FreeAddrInfo(result);
		::closesocket(m_listenSocket);
		::WSACleanup();
		return (unsigned)SOCKET_ERROR;
	}

	/***/ wprintf(L"Listening socket bound to the local address\n");

	// Listen for incoming connections

	while (!WasKilled)
	{
		/***/ wprintf(L"Listening for connections...\n");
		
		if (::listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			::closesocket(m_listenSocket);
			::WSACleanup();
			return (unsigned)SOCKET_ERROR;
		}

		NetConnection* connection = new NetConnection();
		
		NetConnection::EventArgs e(connection, null, 0);
		RequestConnectionEvent.Send(e);

		if (e.Result)
		{
			SOCKET clientSocket = INVALID_SOCKET;

			clientSocket = ::accept(m_listenSocket, null, null);

			if (clientSocket == INVALID_SOCKET)
			{
				::closesocket(m_listenSocket);
				::WSACleanup();
				return (unsigned)INVALID_SOCKET;
			}
		
			/***/ wprintf(L"Connection accepted\n");
			
			connection->m_clientSocket = clientSocket;

			AcceptConnectionEvent.Send(e);

			Connections.Add(connection);

			connection->Start();
		}
		else
		{
			delete connection;
		}
	}

	// Close all the connections
	Connections.Clear();

	::closesocket(m_listenSocket);
	::WSACleanup();

	return 0;
}

Array<NetConnection*>& NetServer::GetConnections()
{
	return m_connections;
}