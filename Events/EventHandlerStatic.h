#ifndef EVENT_HANDLER_STATIC_H
#define EVENT_HANDLER_STATIC_H

#include "EventHandlerBase.h"

template <class Message>
class EventHandlerStatic
	: public EventHandlerBase<Message>
{
protected:
	typedef void (*Procedure)(Message);

public:
	EventHandlerStatic(Procedure proc)	
		: EventHandlerBase(null, proc)
	{

	}

	EventHandlerStatic(const EventHandlerStatic& staticHandler)
		: EventHandlerBase(staticHandler)
	{

	}

	virtual EventHandlerBase<Message>* Copy()
	{
		return new EventHandlerStatic(*this);
	}

protected:
	virtual void Call(Message msg)
	{
		Procedure proc = (Procedure)GetProcedure();
		(*proc)(msg); 
	}
};

#endif