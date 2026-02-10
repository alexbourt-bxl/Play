#include "OpView.h"

#include <Operators/Tree/Tree.h>
#include <Interface/PlayWindow/PlayWindow.h>

DEFINE_ABSTRACT_TYPE(OpView, View);

OpView::OpView()
{
	SetViewedOperatorEvent.Sender = this;
	SetActiveOperatorEvent.Sender = this;

	m_opViewed = null;
	m_opActive = null;
}

OpView::~OpView()
{

}

Operator* OpView::GetViewedOperator() const
{
	return m_opViewed;
}

bool OpView::SetViewedOperator(_Operator* op)
{
	/*	First unassign the current op. */

	if (m_opViewed)
	{
		m_opViewed->Tree->Views.Remove(this);
		m_opViewed->Views.Remove(this);
	}

	/*	Now assign the new op. */

	m_opViewed = op;

	if (m_opViewed)
	{
		m_opViewed->Tree->Views.Add(this);
		m_opViewed->Views.Add(this);
	}

	// Fire event

	_Operator::Message msg;
	
	msg.Object = this;
	msg.Operator = m_opViewed;

	SetViewedOperatorEvent.Send(msg);

	// Set view cursor to default
	//Cursor = Cursor::Arrow;

	return true;
}

Operator* OpView::GetActiveOperator() const
{
	return m_opActive;
}

bool OpView::SetActiveOperator(_Operator* op)
{
	/*	First unassign the current op. */

	if (m_opActive)
	{
		m_opActive->DisconnectView(this);
		//m_opActive->ActiveViews.Remove(this);
	}

	/*	Now assign the new op. */

	m_opActive = op;

	if (m_opActive)
	{
		m_opActive->ConnectView(this);
		//m_opActive->ActiveViews.Add(this);

		PlayWindow* wnd = (PlayWindow*)Window;
		wnd->m_propView.SetPropertyContainer(op, typeof(op).DisplayName);
	}

	// Fire event

	_Operator::Message msg;
	
	msg.Object = this;
	msg.Operator = m_opActive;

	SetActiveOperatorEvent.Send(msg);

	// Set view cursor to default
	Cursor = Cursor::Arrow;

	return true;
}