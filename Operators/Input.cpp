#include "Input.h"

#include <Operators/Operator.h>
#include <Operators/Output.h>
#include <Operators/Connection.h>
#include <Operators/Tree/Tree.h>
#include <Data/DataArray.h>

DataArray Input::s_nullDataArray;

DEFINE_TYPE(Input, Object);

Input::Input()
{
	m_permanent = true;

	ConnectEvent.Sender = this;
	DisconnectEvent.Sender = this;

	m_connection = null;
	m_operator = null;
}

Input::Input(const Input& in)
{
	m_permanent = true;

	ConnectEvent.Sender = this;
	DisconnectEvent.Sender = this;

	m_connection = null;
	m_operator = null;

	for (int i = 0; i < ((Input&)in).AcceptedTypes.Count; i++)
		AcceptedTypes.Add(((Input&)in).AcceptedTypes[i]);
}

Object* Input::Copy()
{
	return new Input(*this);
}

Connection* Input::GetConnection()
{
	return m_connection;
}

//Output* Input::GetOutput() const
//{
//	return 
//		m_connection
//		? m_connection->Out
//		: null;
//}

Array<__Type*>& Input::GetAcceptedTypes()
{
	return m_acceptedTypes;
}

bool Input::AcceptsType(Type type)
{
	for (int i = 0; i < AcceptedTypes.Count; i++)
	{
		if (*AcceptedTypes[i] == type)
			return true;
	}

	return false;
}

Operator* Input::GetOperator() const
{
	return m_operator;
}

Connection* Input::Connect(Output* out)
{
	assert(m_operator);
	assert(out);

	if (m_operator == out->Operator)
		return null;

	assert(!m_connection);

	m_connection = out->Connect(this);

	/*	When an input is connected, 
		the operator becomes dirty. */
	if (m_operator)
		m_operator->MarkDirty();
	
	Message msg(this);
	ConnectEvent.Send(msg);

	return m_connection;
}

bool Input::Disconnect()
{
	if (!m_connection)
		return false;

	Output* out = m_connection->Out;

	/*	When Output::Disconnect() asks, 
		nothing should be connected. */
	m_connection = null; 

	out->Disconnect(this);

	/*	When an input is disconnected, 
		the operator becomes dirty. */
	m_operator->MarkDirty();
	
	Message msg(this);
	DisconnectEvent.Send(msg);

	return true;
}

DataArray& Input::GetData()
{
	if (!m_connection)
		return s_nullDataArray;

	m_connection->Out->Operator->Update();

	return m_connection->Out->Data;
}