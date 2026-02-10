#include "Connection.h"

#include <Operators\Input.h>
#include <Operators\Output.h>

DEFINE_TYPE(Connection, Object, PropertyContainer);

Connection::Connection()
{
	m_out = null;
	m_in  = null;

	//Cycles = 2; // seems like a good default
	//Properties.Add(&Cycles);

	//m_cyclesLeft = Cycles;
}

Connection::Connection(const Connection& conn)
{
	m_out = conn.m_out;
	m_in  = conn.m_in;

	//Cycles = conn.Cycles;
	//Properties.Add(&Cycles);

	//m_cyclesLeft = Cycles;
}

Connection::Connection(Output* out, Input* in)
{
	m_out = out;
	m_in  = in;

	//Cycles = 3; // seems like a good default
	//Properties.Add(&Cycles);

	//m_cyclesLeft = Cycles;
}

Object* Connection::Copy()
{
	return new Connection(*this);
}

Input* Connection::GetInput()
{
	return m_in;
}

void Connection::SetInput(Input* in)
{
	m_in = in;
}

Output* Connection::GetOutput()
{
	return m_out;
}

void Connection::SetOutput(Output* out)
{
	m_out = out;
}

//int Connection::GetCyclesLeft()
//{
//	return m_cyclesLeft--;
//}
//
//void Connection::Reset()
//{
//	m_cyclesLeft = Cycles;
//}