#ifndef OUTPUT_H
#define OUTPUT_H

#include <Types/Object.h>
#include <Containers/Array.h>
#include <Threads/ThreadLock.h>
#include <Data/DataArray.h>

class Operator;
class Input;
class InputArray;
class Connection;

class Output
	: public Object,
	  public ThreadLock
{
	DECLARE_TYPE;

	friend class Operator;

	typedef Operator _Operator;
	typedef Object _Object;

public:
	Output();
	Output(const Output& out);
	~Output();

	_Object* Copy();

	__declspec(property(get = GetOperator))
	_Operator* Operator;
	_Operator* GetOperator() const;

	__declspec(property(get = GetConnections))
	Array<Connection*>& Connections;
	Array<Connection*>& GetConnections();

	Connection* Connect(Input* in);
	bool Disconnect(Input* in);

	//Connection* operator >> (Input* in);
	//Connection* operator >> (InputArray& inputs);

	void connections_OnRemove(Container::Message& msg);

	//__declspec(property(get = GetObject))
	//_Object* Object;
	//_Object* GetObject() const;
	//void     SetObject(_Object* obj);

	__declspec(property(get = GetData, put = SetData))
	DataArray& Data;
	DataArray& GetData();
	void       SetData(const DataArray& data);

private:
	_Operator* m_operator;

	Array<Connection*> m_connections;

	/*	The output object is stored only as a pointer
		and the operator must manage and delete it. */
	//_Object* m_object;
	DataArray m_data;
};

#endif