#include "EventSender.h"

EventSender::EventSender()
{
	m_eventsEnabled = 0;
}

bool EventSender::EnableEvents()
{
	if (m_eventsEnabled >= 0)
		return true;

	return ++m_eventsEnabled >= 0;
}

void EventSender::DisableEvents()
{
	m_eventsEnabled--;
}

bool EventSender::GetEventsEnabled()
{
	return m_eventsEnabled >= 0;
}