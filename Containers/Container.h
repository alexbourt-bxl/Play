#ifndef CONTAINER_H
#define CONTAINER_H

#include <Types/BaseTypes.h>
#include <Events/EventSender.h>
#include <Events/Message.h> // Must be in a separate file because of the circular dependency; see below.

template <class Message> class Event;

class Container
	: public EventSender
{
public:
	class Message;

public:
	void* Owner; // any object the container knows to be its owner

public:
	Container(bool createMessages = true);
	Container(const Container& container);
	virtual ~Container();

	//__declspec(property(get = GetCount)) 
	//uint Count;
	//virtual uint GetCount() = 0;

public:
	/*	I have to use pointers here because of the 
		circular dependency between Event and Container. */
	Event<Container::Message&>* AddEvent;
	Event<Container::Message&>* RemoveEvent;
	//Event<Container::Message&>* CompareMessage; // comparison result returned in msg.Result
};

class Container::Message
	: public ::Message
{
public:
	void* Owner;
	void* Item;

	Message(void* owner, void* item)
		: Owner(owner), Item(item) {}

	/*	When receiving Container::MessageArg, both Owner 
		and Item have to be dereferenced. */
};

#endif