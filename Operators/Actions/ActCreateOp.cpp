#include "ActCreateOp.h"

#include <Operators/Operator.h>
#include <Operators/Tree/Tree.h>
#include <Controls/Views/TreeView/TreeView.h>
#include <Controls/Views/OpView/OpView.h>
#include "ActConnect.h"

#include <assert.h>

DEFINE_TYPE(ActCreateOp, Action);

ActCreateOp::ActCreateOp()
{
	m_tree = null;
	m_type = null;

	m_newOp = null;
	m_undone = false;

	m_prevOp = null;
	m_actConnect = null;
}

ActCreateOp::ActCreateOp(_Tree* tree, __Type* type)
{
	m_tree = tree;
	m_type = type;

	m_newOp = null;
	m_undone = false;

	m_prevOp = null;
	m_actConnect = null;
}

ActCreateOp::~ActCreateOp()
{
	if (m_undone)
	{
		if (m_newOp)
			delete m_newOp;

		if (m_actConnect)
			delete m_actConnect;
	}
}

Tree* ActCreateOp::GetTree()
{
	return m_tree;
}

void ActCreateOp::SetTree(_Tree* tree)
{
	m_tree = tree;
}

__Type* ActCreateOp::GetOpType() const
{
	return m_type;
}

void ActCreateOp::SetOpType(_Type type)
{
	m_type = type;
}

Operator* ActCreateOp::GetConnectAfter()
{
	return m_prevOp;
}

void ActCreateOp::SetConnectAfter(Operator* op)
{
	m_prevOp = op;
}

bool ActCreateOp::Perform()
{
	assert(m_tree);

	if (!m_newOp) // first time
		m_newOp = (Operator*)m_type->CreateObject();

	// Prepare event
	Array<Operator*> ops;
	ops.Add(m_newOp);

	Tree::Message te;
	te.Operators = &ops;

	// Fire pre event
	m_tree->PreUpdateEvent.Send(te);

	// Add operator to the tree
	m_tree->Add(m_newOp);

	// Fire post event
	m_tree->UpdateEvent.Send(te);

	// Connect to the previous op if necessary

	if (m_prevOp && m_newOp->In.Count > 0)
	{
		if (!m_actConnect)
			m_actConnect = new ActConnect(m_prevOp->Out[0], m_newOp->In[0]);

		m_actConnect->Perform();
	}

	m_tree->Selected.Clear();
	m_newOp->IsSelected = true;

	m_undone = false;
	return true;
}

void ActCreateOp::Restore()
{
	// Prepare event
	Array<Operator*> ops;
	ops.Add(m_newOp);

	Tree::Message msg;
	msg.Operators = &ops;

	// Fire pre event
	m_tree->PreUpdateEvent.Send(msg);

	/*	If an automatic connection was made,
		remove it. This must be done before
		the new op is removed from the tree. */
	if (m_actConnect)
		m_actConnect->Restore();

	/*	Disconnect op from views. */

	for (int i = m_newOp->Views.Count - 1; i >= 0; i--)
	{
		if (m_newOp->Views[i]->ActiveOperator == m_newOp)
			m_newOp->Views[i]->ActiveOperator = null; // TODO: show previous or next active op

		if (m_newOp->Views[i]->ViewedOperator == m_newOp)
			m_newOp->Views[i]->ViewedOperator = null; // TODO: show previous or next viewed op
	}

	// Remove operator from the tree
	m_tree->Remove(m_newOp);

	// Fire post event
	m_tree->UpdateEvent.Send(msg);

	m_undone = true;
}