#include "ActDeleteOps.h"

#include <Operators/Operator.h>
#include <Operators/Connection.h>
#include <Operators/Tree/Tree.h>
#include <Controls/Views/OpView/OpView.h>
#include <assert.h>

DEFINE_TYPE(ActDeleteOps, Action);

ActDeleteOps::ActDeleteOps()
{
	m_tree = null;
	m_undone = false;
}

ActDeleteOps::ActDeleteOps(Array<Operator*>& ops)
{
	m_tree = null;
	m_undone = false;

	for (int i = 0; i < ops.Count; i++)
		m_ops.Add(ops[i]);
}

ActDeleteOps::~ActDeleteOps()
{
	/*	Since deleted ops are kept in memory,
		when this action is deleted they also
		have to be. */

	if (!m_undone)
	{
		for (int i = 0; i < m_ops.Count; i++)
			delete m_ops[i];
	}
}

Array<Operator*>& ActDeleteOps::GetOperators()
{
	return m_ops;
}

bool ActDeleteOps::Perform()
{
	if (m_ops.Count == 0)
		return false;

	m_tree = m_ops[0]->Tree;
	assert(m_tree);

	/*	One tree should be common to all ops. */
	for (int i = 1; i < m_ops.Count; i++)
		assert(m_ops[i]->Tree == m_tree);

	//

	for (int i = 0; i < m_ops.Count; i++)
	{
		Operator* op = m_ops[i];

		bool selected = op->IsSelected; // store selected state

		/*	If the operator is preceded and followed,
			then new connections must be creaded in
			place of the old ones. 
			
			The first input's connection is always
			forwarded.
		*/

		if (op->In.Count >= 1 && op->Out.Count >= 1
			&& op->In[0]->Connection)
		{
			// TODO: different type connections (image, audio, etc.)
			// should be passed separately

			for (int j = 0; j < op->Out[0]->Connections.Count; j++)
			{
				Connection* conn = new Connection(
					op->In[0]->Connection->Out, 
					op->Out[0]->Connections[j]->In);
				//conn->Cycles = op->In[0]->Connection->Cycles;
				m_createdConnections.Add(conn);
			}
		}

		/*	Store and remove input connections. */

		for (int j = op->In.Count - 1; j >= 0; j--)
		{
			if (op->In[j]->Connection)
			{
				m_deletedConnections.Add(new Connection(*op->In[j]->Connection));
				op->In[j]->Disconnect();
			}
		}

		/*	Store and remove output connections. */

		for (int j = op->Out.Count - 1; j >= 0; j--)
		{
			for (int k = op->Out[j]->Connections.Count - 1; k >= 0; k--)
			{
				m_deletedConnections.Add(new Connection(*op->Out[j]->Connections[k]));
				op->Out[j]->Connections[k]->In->Disconnect();
			}
		}

		/*	Disconnect op from views. */

		for (int j = op->Views.Count - 1; j >= 0; j--)
		{
			if (op->Views[j]->ActiveOperator == op)
				op->Views[j]->ActiveOperator = null; // TODO: show previous or next active op

			if (op->Views[j]->ViewedOperator == op)
				op->Views[j]->ViewedOperator = null; // TODO: show previous or next viewed op
		}

		/*	Remove the operator from the tree. */

		m_tree->Remove(op);

		op->IsSelected = selected; // restore selected state
	}

	m_undone = false;

	/*	Create the new connections, deleting 
		and replacing the temporary ones. */

	for (int i = 0; i < m_createdConnections.Count; i++)
	{
		Connection conn(*m_createdConnections[i]);
		delete m_createdConnections[i];

		m_createdConnections[i] = conn.Out->Connect(conn.In);
		//m_createdConnections[i]->Cycles = conn.Cycles;
	}

	/*	Fire tree event. */

	Array<Connection*> connections;

	for (int i = 0; i < m_deletedConnections.Count; i++)
		connections.Add(m_deletedConnections[i]);

	for (int i = 0; i < m_createdConnections.Count; i++)
		connections.Add(m_createdConnections[i]);

	Tree::Message msg;
	msg.Operators = &m_ops;
	msg.Connections = &connections;
	m_tree->UpdateEvent.Send(msg);

	//

	return true;
}

void ActDeleteOps::Restore()
{
	assert(m_tree);

	/*	Delete replacement connections. */

	for (int i = 0; i < m_createdConnections.Count; i++)
		m_createdConnections[i]->In->Disconnect();

	/*	Restore operators. */

	for (int i = 0; i < m_ops.Count; i++)
		m_tree->Add(m_ops[i]);

	m_undone = true;

	/*	Restore previous connections. */

	for (int i = 0; i < m_deletedConnections.Count; i++)
	{
		/*Connection* conn =*/ m_deletedConnections[i]->Out->Connect(m_deletedConnections[i]->In);
		//conn->Cycles = m_deletedConnections[i]->Cycles;
	}

	/*	Fire tree event. */

	Array<Connection*> connections;

	for (int i = 0; i < m_deletedConnections.Count; i++)
		connections.Add(m_deletedConnections[i]);

	for (int i = 0; i < m_createdConnections.Count; i++)
		connections.Add(m_createdConnections[i]);

	Tree::Message msg;
	msg.Operators = &m_ops;
	msg.Connections = &connections;
	m_tree->UpdateEvent.Send(msg);

	/*	Clear stored connections. */

	for (int i = 0; i < m_deletedConnections.Count; i++)
		delete m_deletedConnections[i];
	
	m_deletedConnections.Clear();
	m_createdConnections.Clear();
}