#ifndef EVENT_H
#define EVENT_H

/*	NOTE: Any class that wants to send events 
		  must inherit from EventSender. */

#include "EventHandlerBase.h"
#include "EventSender.h"
#include <Containers/Array.h>

#include <assert.h>

template <class Class>
class Array;

template <class Message>
class Event
{
public:
	typedef EventHandlerBase<Message> EventHandler;
	typedef void (*HandlerProc)(Message);

public:
	Event()
	{
		m_sender = null;
		m_handlers = new Array<EventHandler*>(false); // don't create events
		m_enabled = true;
		m_allowDuplicates = false;
	}

	~Event()
	{
		if (m_handlers)
			delete m_handlers;
	}
	
	void Clear()
	{
		for (int i = 0; i < m_handlers->Count; i++)
			delete (*m_handlers)[i];

		m_handlers->Clear();
	}

	void operator += (EventHandler& handler)
	{
		/*	The manual search is done here because the actual 
			handlers must be compared, not just their pointers. */

		for (int i = 0; i < m_handlers->Count; i++)
		{
			if (*(*m_handlers)[i] == handler
				&& !m_allowDuplicates)
				return;
		}

		m_handlers->Add(handler.Copy());
	}

	void operator += (const EventHandler& handler)
	{
		*this += (EventHandler&)handler;
	}

	void operator += (HandlerProc proc)
	{
		*this += EventHandler(proc);
	}

	void operator -= (EventHandler& handler)
	{
		/*	The manual search is done here because the actual 
			handlers must be compared, not just their pointers. */

		for (int i = 0; i < m_handlers->Count; i++)
		{
			if (*(*m_handlers)[i] == handler)
			{
				delete (*m_handlers)[i];
				m_handlers->RemoveAt(i);
				return;
			}
		}
	}

	void operator -= (const EventHandler& handler)
	{
		*this -= (EventHandler&)handler;
	}

	void operator -= (HandlerProc proc)
	{
		*this -= EventHandler(proc);
	}

	void Send(Message msg)
	{
		assert(m_sender);

		if (!m_enabled || !m_sender->EventsEnabled)
			return;

		for (int i = 0; i < m_handlers->Count; i++)
			(*(*m_handlers)[i])(msg);
	}

	__declspec(property(get = GetSender, put = SetSender)) 
	EventSender* Sender;
	EventSender* GetSender() const { return m_sender; }
	void SetSender(EventSender* sender) { m_sender = sender; }

	__declspec(property(get = GetHandlers))
	Array<EventHandler*>& Handlers;
	Array<EventHandler*>& GetHandlers() { return m_handlers; }

	__declspec(property(get = GetEnabled, put = SetEnabled))
	bool Enabled;
	bool GetEnabled() { return m_enabled; }
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	__declspec(property(get = GetAllowDuplicates, put = SetAllowDuplicates))
	bool AllowDuplicates;
	bool GetAllowDuplicates() { return m_allowDuplicates; }
	void SetAllowDuplicates(bool allow) { m_allowDuplicates = allow; }

private:
	EventSender* m_sender;
	Array<EventHandler*>* m_handlers;
	bool m_enabled;
	bool m_allowDuplicates; // should normally never be used
};

#include "Message.h"
#include "EventHandler.h"

#endif