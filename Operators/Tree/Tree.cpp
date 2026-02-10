#include "Tree.h"

#include <Operators\Operator.h>
#include <Controls\Views\ImageView\ImageView.h>

DEFINE_TYPE(Tree, Object);

Tree::Tree()
{
	PreUpdateEvent.Sender = this;
	UpdateEvent.Sender = this;

	m_op = null;

	*AddEvent    += EventHandler(this, &Tree::OnAddOperator);
	*RemoveEvent += EventHandler(this, &Tree::OnRemoveOperator);

	*m_selected.AddEvent    += EventHandler(this, &Tree::selected_OnAdd);
	*m_selected.RemoveEvent += EventHandler(this, &Tree::selected_OnRemove);
}

Tree::~Tree()
{
	m_selected.Clear();

	/*	Deleting a tree deletes 
		all its operators. */

	for (int i = Count - 1; i >= 0; i--)
	{
		GetItem(i)->m_tree = null;
		delete GetItem(i);
	}
}

Object* Tree::Copy()
{
	return null;
}

Array<Operator*>& Tree::GetSelected()
{
	return m_selected;
}

Array<View*>& Tree::GetViews()
{
	return m_views;
}

Array<OpView*> Tree::GetOpViews(Operator* op)
{
	Array<OpView*> opViews;

	for (int i = 0; i < Views.Count; i++)
	{
		if (typeof(Views[i]) != TYPEOF(OpView))
			continue;

		OpView* view = (OpView*)Views[i];
		Operator* _op = view->ViewedOperator;

		if (_op == op || _op->IsAfter(op))
			opViews.Add(view);
	}

	return opViews;
}

void Tree::Update(Operator* op)
{
	m_opLock.Lock();

	m_op = op;
	Start();

	m_opLock.Unlock();
}

uint Tree::ThreadProc()
{
	/*	NOTE: m_opLock is locked before the loop and unlocked after the 
		loop, and the same is done in reverse on the inside of the loop. */

	m_opLock.Lock();

	while (!WasKilled && m_op)
	{
		Operator* _op = m_op;
		m_op = null;

		m_opLock.Unlock();

		//

		/*	Determine which viewed ops to update. */
		
		Array<OpView*> views = GetOpViews(_op);

		/*	Update the viewed ops. */

		for (int i = 0; i < views.Count; i++)
		{
			OpView* view = (OpView*)views[i];
			Operator* op = view->ViewedOperator;

			while (op->IsDirty)
			{
				op->Update();
				view->UpdateView();
			}
		}

		//

		m_opLock.Lock();
	}

	m_opLock.Unlock();

	//       TREE
	return 0x72EE;
}