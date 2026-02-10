#ifndef PROPERTY_H
#define PROPERTY_H

#include <Types/Object.h>
#include <Events/Event.h>
#include <Types/Basic/String.h>

class Control;
class PropertyContainer;

class Property
	: public Object, 
	  public EventSender
{
	friend class PropertyContainer;
	friend class Animation;

	DECLARE_ABSTRACT_TYPE;

	typedef Control _Control;

public:
	class Message;

public:
	Property();

	__declspec(property(get = GetName, put = SetName))
	String Name;
	String GetName();
	void   SetName(const String& name);

	__declspec(property(get = GetContainer))
	PropertyContainer* Container;
	PropertyContainer* GetContainer();

	__declspec(property(get = GetControl))
	_Control* Control;
	virtual _Control* GetControl() const = 0;

	__declspec(property(get = GetEnableAction, put = SetEnableAction))
	bool EnableAction;
	bool GetEnableAction();
	void SetEnableAction(bool enableAction);

	Event<Property::Message&> PreChangeEvent;
	Event<Property::Message&> ChangeEvent;

protected:
	virtual void PerformAction() = 0;

	void SuppressAction();

private:
	String m_name;
	PropertyContainer* m_container;
	bool m_enableAction;

	/*	This flag is used to do one-time 
		action suppression. */
	bool m_suppressAction;

	void OnChange(Property::Message& msg);
};

//

class Property::Message
	: public ::Message
{
public:
	void* Value;

	Message(void* value)
		: Value(value) {}

	Message(const Message& msg)
		: Value(msg.Value) {}
};

#endif