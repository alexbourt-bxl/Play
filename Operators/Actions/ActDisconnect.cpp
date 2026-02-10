#include "ActDisconnect.h"

#include <Operators/Output.h>
#include <Operators/Input.h>
#include <Operators/Connection.h>
#include <Operators/Operator.h>
#include <Operators/Tree/Tree.h>

DEFINE_TYPE(ActDisconnect, Action);

ActDisconnect::ActDisconnect()
{
	m_in  = null;
	m_oldOut = null;
}

ActDisconnect::ActDisconnect(_Input* in)
{
	m_in = in;
	m_oldOut = null;
}

Input* ActDisconnect::GetInput() const
{
	return m_in;
}

void ActDisconnect::SetInput(_Input* in)
{
	m_in = in;
}

bool ActDisconnect::Perform()
{
	assert(m_in);

	Tree* tree = m_in->Operator->Tree;
	assert(tree);

	// Prepare tree events
	Tree::Message te;
	Array<Connection*> connections;
	te.Connections = &connections;

	m_oldOut = m_in->Connection->Out;
	//m_oldConnCycles = m_in->Connection->Cycles;

	// Fire tree pre event
	connections.Add(m_in->Connection);
	tree->PreUpdateEvent.Send(te);

	// Remove the connection
	m_in->Disconnect();

	// Fire tree post event
	connections.Clear();
	tree->UpdateEvent.Send(te);

	return true;
}

void ActDisconnect::Restore()
{
	assert(m_in);
	assert(m_oldOut);

	Tree* tree = m_in->Operator->Tree;
	assert(tree);

	// Prepare tree events
	Tree::Message te;
	Array<Connection*> connections;
	te.Connections = &connections;

	// No pre event necessary

	Connection* conn = m_oldOut->Connect(m_in);

	//conn->Cycles = m_oldConnCycles;
	m_oldOut = null;

	// Fire tree post event
	connections.Add(conn);
	tree->UpdateEvent.Send(te);
}