#include "InputArray.h"

#include "Connection.h"

InputArray::InputArray()
{
	m_operator = null;
	m_fixedCount = true;
	m_newInput = null;
}

InputArray::~InputArray()
{
	if (m_newInput)
		delete m_newInput;
}

bool InputArray::GetCountIsFixed()
{
	return m_fixedCount;
}

void InputArray::SetCountIsFixed(bool fixed)
{
	m_fixedCount = fixed;

	if (m_fixedCount && m_newInput)
	{
		delete m_newInput;
		m_newInput = null;
	}
	else if (!m_fixedCount && !m_newInput)
	{
		m_newInput = CreateNewInput();
	}
}

Input* InputArray::GetNewInput()
{
	return m_newInput;
}

Array<__Type*>& InputArray::GetAcceptedTypes()
{
	return m_acceptedTypes;
}

bool InputArray::AcceptsType(Type type)
{
	for (int i = 0; i < AcceptedTypes.Count; i++)
	{
		if (*AcceptedTypes[i] == type)
			return true;
	}

	return false;
}

InputArray::operator Input* ()
{
	return Count > 0
		? GetItem(0)
		: null;
}

Connection* InputArray::Connect(Output* out)
{
	/*	If the input count is fixed, try to
		connect the output to the first input. */

	if (m_fixedCount)
	{
		if (Count == 0)
			return null;

		/*	TODO:

			Connect to the next empty output,
			of if all are connected, then to
			the next one from the last connected (cycle).
			This might become annoying, so need to test. */

		return GetItem(0)->Connect(out);
	}

	/*	Otherwise create an input that takes all
		the accepted types, and try to connect 
		the output to it. */

	else if (m_newInput)
	{
		/*	If the connection is successful, 
			add	it to the array. */

		return m_newInput->Connect(out);
	}

	return null;
}

bool InputArray::Disconnect(Output* out)
{
	bool result = false;

	for (int i = 0; i < Count; i++)
	{
		if (GetItem(i)->Connection->Out == out)
			result |= GetItem(i)->Disconnect();
	}

	return result;
}

bool InputArray::IsConnected(Output* out)
{
	for (int i = 0; i < Count; i++)
	{
		if (GetItem(i)->Connection->Out == out)
			return true;
	}

	return false;
}

Input* InputArray::CreateNewInput()
{
	Input* in = new Input();

	in->m_operator = m_operator;
	in->m_permanent = false;
	in->ConnectEvent += EventHandler(this, &InputArray::Input_OnConnect);
	in->DisconnectEvent += EventHandler(this, &InputArray::Input_OnDisconnect);

	for (int i = 0; i < AcceptedTypes.Count; i++)
		in->AcceptedTypes.Add(AcceptedTypes[i]);

	return in;
}