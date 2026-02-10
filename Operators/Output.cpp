#include "Output.h"

#include "Operator.h"
#include "Input.h"
#include "InputArray.h"
#include "Connection.h"

DEFINE_TYPE(Output, Object);

Output::Output()
{
	m_operator = null;
	*m_connections.RemoveEvent += EventHandler(this, &Output::connections_OnRemove);

	//m_object = null;
}

Output::Output(const Output& /*out*/)
{
	m_operator = null;
	*m_connections.RemoveEvent += EventHandler(this, &Output::connections_OnRemove);

	//m_object = null;
}

Output::~Output()
{

}

Object* Output::Copy()
{
	return new Output(*this);
}

Operator* Output::GetOperator() const
{
	return m_operator;
}

Array<Connection*>& Output::GetConnections()
{
	return m_connections;
}

Connection* Output::Connect(Input* in)
{
	if (m_operator == in->Operator)
		return null;

	/*	Check that the input 
		is not already connected. */
	for (int i = 0; i < m_connections.Count; i++)
	{
		if (m_connections[i]->In == in)
			return m_connections[i];
	}

	/*	Create the connection. */
	Connection* conn = new Connection(this, in);
	m_connections.Add(conn);

	/*	Now connect this output to the input; when the input tries to connect back
		it will fail, because it is already connected, and the loop will be broken. */
	in->Connect(this);

	return conn;
}

bool Output::Disconnect(Input* in)
{
	bool result = false;

	for (int i = m_connections.Count - 1; i >= 0; i--) // must be done backwards as items are removed
	{
		if (m_connections[i]->In == in)
		{
			result |= in->Disconnect();
			m_connections.RemoveAt(i);
		}
	}

	return result;
}

//Object* Output::GetObject() const
//{
//	// TODO: Operators need a cheaper way of getting the output type.
//	// For exmple, for OpFile, the type could be decided from the file's extension.
//
//	return m_object;
//}
//
//void Output::SetObject(_Object* obj)
//{
//	Lock();
//
//	m_object = obj;
//
//	Unlock();
//}

DataArray& Output::GetData()
{
	return m_data;
}

void Output::SetData(const DataArray& data)
{
	Lock();
	m_data = data;
	Unlock();
}

//Connection* Output::operator >> (Input* in)
//{
//	return Connect(in);
//}
//
//Connection* Output::operator >> (InputArray& inputs)
//{
//	return inputs.Connect(this);
//}