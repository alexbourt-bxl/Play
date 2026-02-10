#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "NetConnection.h"

#include <Containers\Array.h>

class NetServer 
	: public Thread, public EventSender
{
public:
	NetServer();
	~NetServer();

	uint	ThreadProc();

	__declspec(property(get = GetConnections))
	Array<NetConnection*>& Connections;
	Array<NetConnection*>& GetConnections();

private:
	Array<NetConnection*> m_connections;
	void connection_RemoveEvent(Container::EventArgs& e);

	SOCKET	m_listenSocket;

public:
	event<NetConnection::EventArgs&> RequestConnectionEvent;
	event<NetConnection::EventArgs&> AcceptConnectionEvent;
};

#endif