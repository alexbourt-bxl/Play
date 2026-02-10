#ifndef NET_CONNECTION_H
#define NET_CONNECTION_H

#define PLAY_PORT L"7529" // "play" on an alphanumeric keypad :)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Threads\Thread.h>
#include <Events\Event.h>
#include <Types\Basic\String.h>

#include <winsock2.h>

class NetConnection 
	: public Thread, public EventSender
{
	friend class NetServer;

public:
	class EventArgs;

public:
	uint	ThreadProc();

public:
	NetConnection();
	~NetConnection();

	bool	Connect(const String& wcAddress);
	int		Send(char* data, int dataSize);

	//void	SetSocket(SOCKET socket);

private:
	SOCKET	m_clientSocket;

public:
	event<EventArgs&> OpenEvent;
	event<EventArgs&> ReceiveDataEvent;
	event<EventArgs&> CloseEvent;
};

//

class NetConnection::EventArgs
	: public ::EventArgs
{
public:
	NetConnection* Connection;
	char* Data;
	int   DataSize;

	EventArgs(NetConnection* connection, char* data, int dataSize)
		: Connection(connection), Data(data), DataSize(dataSize) {}
};

#endif