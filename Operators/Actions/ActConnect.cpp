#include "ActConnect.h"

#include <Operators/Output.h>
#include <Operators/Input.h>
#include <Operators/Operator.h>
#include <Operators/Tree/Tree.h>

DEFINE_TYPE(ActConnect, Action);

ActConnect::ActConnect()
{
	m_out = null;
	m_in  = null;
	m_oldOut = null;
}

ActConnect::ActConnect(_Output* out, _Input* in)
{
	m_out = out;
	m_in = in;
	m_oldOut = null;
}

Output* ActConnect::GetOutput() const
{
	return m_out;
}

void ActConnect::SetOutput(_Output* out)
{
	m_out = out;
}

Input* ActConnect::GetInput() const
{
	return m_in;
}

void ActConnect::SetInput(_Input* in)
{
	m_in = in;
}

bool ActConnect::Perform()
{
	assert(m_out);
	assert(m_in);

	Tree* tree = m_in->Operator->Tree;
	assert(tree);

	// Prepare tree events
	Tree::Message te;
	Array<Connection*> connections;
	te.Connections = &connections;

	/*	Disconnect any existing connection. */

	if (m_in->Connection)
	{
		// Store existing connection
		assert(!m_oldOut);
		m_oldOut = m_in->Connection->Out;
		//m_oldConnCycles = m_in->Connection->Cycles;

		// Fire tree pre event
		connections.Add(m_in->Connection);
		tree->PreUpdateEvent.Send(te);

		// Remove existing connection
		tree->DisableEvents();
		m_in->Disconnect();
		tree->EnableEvents();
	}

	// Make the connection
	Connection* conn = m_out->Connect(m_in);

	// Fire tree post event
	connections.Clear();
	connections.Add(conn);
	tree->UpdateEvent.Send(te);

	return true;
}

void ActConnect::Restore()
{
	Tree* tree = m_in->Operator->Tree;
	assert(tree);

	// Prepare tree events
	Tree::Message te;
	Array<Connection*> connections;
	te.Connections = &connections;

	// Fire tree pre event
	connections.Add(m_in->Connection);
	tree->PreUpdateEvent.Send(te);

	// Remove connection
	m_in->Disconnect();

	connections.Clear();

	/*	Restore any previous connection. */

	if (m_oldOut)
	{
		// Restore old connection
		tree->DisableEvents();
		Connection* conn = m_oldOut->Connect(m_in);
		tree->EnableEvents();

		//conn->Cycles = m_oldConnCycles;
		m_oldOut = null;

		connections.Add(conn);
	}

	// Fire tree post event
	tree->UpdateEvent.Send(te);
}