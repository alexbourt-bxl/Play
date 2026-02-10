#ifndef VIEW_H
#define VIEW_H

#include <Controls\Control.h>
#include <Threads\ThreadLock.h>

//Move navigation and scale/navigation to this class, and expose it to all derived View controls.

class Operator;

class View 
	: public Control
{
	DECLARE_ABSTRACT_TYPE;

public:
	class Message;

public:
	View();
	~View();
};

//

class View::Message
	: public ::Message
{
	typedef View _View;
	typedef Type _Type;

public:
	_View* View;
	
	__declspec(property(get = GetType))
	_Type Type;
	_Type GetType()	{ return *m_type; }

	Message(__Type* type)
		: View(null)
	{
		assert(type);
		m_type = type;
	}

private:
	__Type* m_type;
};

#endif