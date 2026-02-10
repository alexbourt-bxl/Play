#include "Message.h"

#include <Types/Object.h>

Message::Message()
{
	Object = null;
	Result = false;
}

Message::Message(const Message& msg)
{
	Object = msg.Object;
	Result = msg.Result;
}

Message::Message(_Object* obj)
{
	Object = obj;
	Result = false;
}