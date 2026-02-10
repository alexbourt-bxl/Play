#ifndef MESSAGE_SOURCE_H
#define MESSAGE_SOURCE_H

// Do not include <Events/Event.h> here

class EventSender
{
public:
	EventSender();

	bool EnableEvents();
	void DisableEvents();
	
	__declspec(property(get = GetEventsEnabled))
	bool EventsEnabled;
	bool GetEventsEnabled();

private:
	int	 m_eventsEnabled;
};

#endif