#ifndef REACTION_H
#define REACTION_H

#include "EventHandlerMember.h"
#include "EventHandlerStatic.h"

template <class Object, class Class, class Message>
EventHandlerMember<Object, Class, Message> EventHandler(Object* obj, void (Class::*proc)(Message))
{
	return EventHandlerMember<Object, Class, Message>(obj, proc);
}

template <class Message>
EventHandlerStatic<Message> EventHandler(void (*proc)(Message))
{
	return EventHandlerStatic<Message>(proc); 
}

#endif