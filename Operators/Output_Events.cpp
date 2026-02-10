#include "Output.h"

#include "Connection.h"

void Output::connections_OnRemove(Container::Message& msg)
{
	Connection* conn = *((Connection**)msg.Item);
	delete conn;
}