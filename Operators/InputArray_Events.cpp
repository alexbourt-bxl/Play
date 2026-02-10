#include "InputArray.h"

void InputArray::OnAddInput(Container::Message& )
{
	//Input* in = *((Input**)msg.Item);

	//in->AcceptedTypes.Clear();

	//for (int i = 0; i < AcceptedTypes.Count; i++)
	//	in->AcceptedTypes.Add(AcceptedTypes[i]);
}

void InputArray::Input_OnConnect(Input::Message& msg)
{
	if (msg.Input == NewInput)
	{
		Add(msg.Input);
		m_newInput = CreateNewInput();
	}
}

void InputArray::Input_OnDisconnect(Input::Message& msg)
{
	if (!msg.Input->m_permanent)
	{
		Remove(msg.Input);
		//msg.Input->MarkForDelete(); //Object::GetPool().MarkForDelete(msg.Input);
		msg.Input = null;
	}
}