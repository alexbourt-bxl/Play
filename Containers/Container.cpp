#include "Container.h"

#include <Events/Event.h>

Container::Container(bool createMessages)
{
	/*	The container can be created without events because
		events contain a List, which derives from Container.
		This recursion causes a stack overflow. */

	if (createMessages)
	{
		AddEvent = new Event<Container::Message&>();
		AddEvent->Sender = this;

		RemoveEvent = new Event<Container::Message&>();
		RemoveEvent->Sender = this;
	}
	else
	{
		AddEvent = null;
		RemoveEvent = null;
	}

	Owner = null; // can be changed
}

Container::Container(const Container& container)
{
	if (container.AddEvent)
	{
		AddEvent = new Event<Container::Message&>();
		AddEvent->Sender = this;
	}
	else
	{
		AddEvent = null;
	}

	if (container.RemoveEvent)
	{
		RemoveEvent = new Event<Container::Message&>();
		RemoveEvent->Sender = this;
	}
	else
	{
		RemoveEvent = null;
	}

	Owner = container.Owner;
}

Container::~Container()
{
	if (AddEvent)
		delete AddEvent;

	if (RemoveEvent)
		delete RemoveEvent;
}