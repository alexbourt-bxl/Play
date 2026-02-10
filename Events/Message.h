#ifndef MESSAGE_H
#define MESSAGE_H

class Object;

class Message
{
	typedef Object _Object;

public:
	Message();
	Message(const Message& msg);
	Message(_Object* obj);

	/*	The object is passed as a pointer 
		to allow for no object. */
	_Object* Object;

	bool Result;
};

#endif