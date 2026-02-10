#ifndef EVENT_HANDLER_BASE_H
#define EVENT_HANDLER_BASE_H

#include <Types/BaseTypes.h>

template <class Message>
class EventHandlerBase
{	
public:
	EventHandlerBase(void* obj, void* proc)
	{
		m_object = obj;
		m_procedure = proc;
	}

	EventHandlerBase(const EventHandlerBase& handlerBase)
	{
		m_object = handlerBase.m_object;
		m_procedure = handlerBase.m_procedure;
	}

	virtual	EventHandlerBase* Copy() = 0;

	void operator () (Message msg)
	{
		Call(msg); 
	}

	virtual bool operator == (const EventHandlerBase& handlerBase)
	{
		return 
			m_object == handlerBase.m_object
			&& m_procedure == handlerBase.m_procedure;
	}

	bool operator != (const EventHandlerBase& handlerBase)
	{
		return !(*this == handlerBase);
	}

protected:
	__declspec(property(get = GetObject))
	void* Object;
	void* GetObject()
	{
		return m_object;
	}

	__declspec(property(get = GetProcedure))
	void* Procedure;
	void* GetProcedure()
	{
		return m_procedure;
	}

	virtual void Call(Message msg) = 0;

protected:
	void* m_object;

private:
	void* m_procedure;
};

#endif