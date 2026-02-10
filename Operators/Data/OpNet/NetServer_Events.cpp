#include "NetServer.h"

void NetServer::connection_RemoveEvent(Container::EventArgs& e)
{
	NetConnection* connection = *((NetConnection**)e.Item);
	connection->Stop(true);
}