#include "Tree.h"

#include <Operators\Operator.h>

void Tree::OnAddOperator(Container::Message& msg)
{
	Operator* op = *((Operator**)msg.Item);

	op->m_tree = this;

	if (op->IsSelected)
	{
		assert(!m_selected.Contains(op));
		m_selected.Add(op);
	}
}

void Tree::OnRemoveOperator(Container::Message& msg)
{
	Operator* op = *((Operator**)msg.Item);

	if (op->IsSelected)
	{
		m_selected.Remove(op);
		assert(!m_selected.Contains(op));
	}

	op->m_tree = null;
	//delete op;
}

void Tree::selected_OnAdd(Container::Message& msg)
{
	DisableEvents();

	Operator* op = *((Operator**)msg.Item);
	op->IsSelected = true;

	EnableEvents();
}

void Tree::selected_OnRemove(Container::Message& msg)
{
	DisableEvents();

	Operator* op = *((Operator**)msg.Item);
	op->IsSelected = false;

	EnableEvents();
}