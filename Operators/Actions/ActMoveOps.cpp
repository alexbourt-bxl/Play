#include "ActMoveOps.h"

#include <Operators/Operator.h>
#include <Operators/Tree/Tree.h>

#include <assert.h>

DEFINE_TYPE(ActMoveOps, Action);

ActMoveOps::ActMoveOps()
{

}

ActMoveOps::ActMoveOps(Array<Operator*>& ops, FPoint offset)
{
	for (int i = 0; i < ops.Count; i++)
	{
		m_ops.Add(ops[i]);
		m_offsets.Add(offset);
	}
}

Array<Operator*>& ActMoveOps::GetOperators()
{
	return m_ops;
}

Array<FPoint>& ActMoveOps::GetOffsets()
{
	return m_offsets;
}

bool ActMoveOps::Perform()
{
	assert(m_ops.Count == m_offsets.Count);

	Tree* tree = m_ops[0]->Tree;
	assert(tree);

	/*	One tree should be common to all ops. */
	for (int i = 1; i < m_ops.Count; i++)
		assert(m_ops[i]->Tree == tree);

	// Prepare event
	Tree::Message msg;
	msg.Operators = &m_ops;

	// Fire pre event
	tree->PreUpdateEvent.Send(msg);

	// Move the operators
	for (int i = 0; i < m_ops.Count; i++)
		m_ops[i]->Position += m_offsets[i];

	// Fire ost event
	tree->UpdateEvent.Send(msg);

	return true;
}

void ActMoveOps::Restore()
{
	assert(m_ops.Count == m_offsets.Count);

	Tree* tree = m_ops[0]->Tree;
	assert(tree);

	/*	One tree should be common to all ops. */
	for (int i = 1; i < m_ops.Count; i++)
		assert(m_ops[i]->Tree == tree);

	// Prepare event
	Tree::Message msg;
	msg.Operators = &m_ops;

	// Fire pre event
	tree->PreUpdateEvent.Send(msg);

	// Move the operators
	for (int i = 0; i < m_ops.Count; i++)
		m_ops[i]->Position -= m_offsets[i];

	// Fire ost event
	tree->UpdateEvent.Send(msg);
}