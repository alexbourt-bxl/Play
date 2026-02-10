#ifndef INPUT_H
#define INPUT_H

#include <Types/Object.h>
#include <Events/Event.h>
#include <Containers/Array.h>
#include <Types/Graphics\GdiImage8.h>
#include <Data/DataArray.h>

class Output;
class InputArray;
class Operator;
class Connection;
class DataArray;

class Input
	: public Object,
	  public EventSender
{
	DECLARE_TYPE;

	friend class Operator;
	friend class InputArray;

	typedef Connection _Connection;
	typedef Operator   _Operator;
	typedef Object     _Object;
	typedef Data       _Data;

public:
	Input();
	Input(const Input& in);

	_Object* Copy();

	__declspec(property(get = GetConnection))
	_Connection* Connection;
	_Connection* GetConnection();

	//__declspec(property(get = GetOutput))
	//Output* Out;
	//Output* GetOutput() const;

	__declspec(property(get = GetAcceptedTypes))
	Array<__Type*>& AcceptedTypes;
	Array<__Type*>& GetAcceptedTypes();

	bool AcceptsType(Type type);

	__declspec(property(get = GetOperator))
	_Operator* Operator;
	_Operator* GetOperator() const;

	_Connection* Connect(Output* out);
	bool Disconnect();

	__declspec(property(get = GetData))
	DataArray& Data;
	DataArray& GetData();
	static DataArray s_nullDataArray;

protected:
	_Connection* m_connection;

private:
	bool m_permanent;

	Array<__Type*> m_acceptedTypes;

	_Operator* m_operator;

public:
	class Message;

	Event<Input::Message&> ConnectEvent;
	Event<Input::Message&> DisconnectEvent;
};

class Input::Message
	: public ::Message
{
	typedef Input _Input;

public:
	_Input* Input; // ugly, but must be so

	Message(_Input* in)
		: Input(in) {}
};

#endif