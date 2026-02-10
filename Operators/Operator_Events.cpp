#include "Operator.h"

#include <Types/Graphics\Font.h>
#include <Operators\Tree\Tree.h>
#include <Controls\Views\OpView\OpView.h>

void Operator::property_OnAdd(Container::Message& msg)
{
	Property* prop = *((Property**)msg.Item);
	prop->PreChangeEvent += EventHandler(this, &Operator::PrePropertyChange);
	prop->ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
}

void Operator::property_OnRemove(Container::Message& msg)
{
	Property* prop = *((Property**)msg.Item);
	prop->PreChangeEvent -= EventHandler(this, &Operator::PrePropertyChange);
	prop->ChangeEvent -= EventHandler(this, &Operator::OnPropertyChange);
}

void Operator::PrePropertyChange(Property::Message& /*msg*/)
{
	m_propLock.Lock(); // to prevent property changes during updates
}

void Operator::OnPropertyChange(Property::Message& /*msg*/)
{
	m_propLock.Unlock(); // to prevent property changes during updates

	MarkDirty();

	if (Tree)
		Tree->Update(this);
}

void Operator::Inputs_OnAdd(Container::Message& msg)
{
	Input* in = *((Input**)msg.Item);
	in->m_operator = this;
}

void Operator::Outputs_OnAdd(Container::Message& msg)
{
	Output* out = *((Output**)msg.Item);
	out->m_operator = this;
}

//void Operator::Views_OnAdd(Container::Message& msg)
//{
//	OpView* view = *((OpView**)msg.Item);
//	//ConnectView(view);
//}
//
//void Operator::Views_OnRemove(Container::Message& msg)
//{
//	OpView* view = *((OpView**)msg.Item);
//	//DisconnectView(view);
//}