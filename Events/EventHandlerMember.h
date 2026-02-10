#ifndef EVENT_HANDLER_MEMBER_H
#define EVENT_HANDLER_MEMBER_H

#include "EventHandlerBase.h"

template <class Event, class Handler, class Message>
class EventHandlerMember
	: public EventHandlerBase<Message>
{
protected:
	typedef void (Handler::*HandlerProc)(Message);

public:
	EventHandlerMember(Event* obj, HandlerProc proc)
		: EventHandlerBase(obj, &proc)
	{
		m_memberHandlerProc = proc;
	}

	EventHandlerMember(const EventHandlerMember& memberHandler)
		: EventHandlerBase(memberHandler) 
	{
		m_memberHandlerProc = memberHandler.m_memberHandlerProc;
	}

	virtual EventHandlerBase* Copy()
	{
		return new EventHandlerMember(*this);
	}

	bool operator == (const EventHandlerBase& handlerBase)
	{
		EventHandlerMember& handlerMember = (EventHandlerMember&)handlerBase;

		return 
			m_object == handlerMember.m_object
			&& m_memberHandlerProc == handlerMember.m_memberHandlerProc;
	}

protected:
	virtual void Call(Message msg)
	{
		Event* obj = (Event*)Object;
		(obj->*m_memberHandlerProc)(msg);
	}

private:
	HandlerProc m_memberHandlerProc;
};

#endif