#ifndef INPUT_ARRAY_H
#define INPUT_ARRAY_H

#include <Containers/Array.h>
#include "Input.h"
#include <Graphics\Graphics.h>
#include <Types/Graphics\GdiImage8.h>
#include <Controls\Layout\GridLayout\GridLayout.h>
#include <Controls\ImageControl\ImageControl.h>

class Connection;

class InputArray
	: public Array<Input*>
{
	friend class Operator;

public:
	InputArray();
	~InputArray();

	__declspec(property(get = GetCountIsFixed, put = SetCountIsFixed))
	bool CountIsFixed;
	bool GetCountIsFixed();
	void SetCountIsFixed(bool fixed);

	__declspec(property(get = GetNewInput))
	Input* NewInput;
	Input* GetNewInput();

	__declspec(property(get = GetAcceptedTypes))
	Array<__Type*>& AcceptedTypes;
	Array<__Type*>& GetAcceptedTypes();

	bool AcceptsType(Type type);

	operator Input* ();

	Connection* Connect(Output* out);
	bool Disconnect(Output* out);

	bool IsConnected(Output* out);

private:
	Operator* m_operator;
	bool m_fixedCount;

	/*	If the input count is not fixed
		a permanent "new" input is kept
		so that it can be referenced.
		This input will then be added
		to the list. */
	Input* m_newInput;
	Input* CreateNewInput();

	Array<__Type*> m_acceptedTypes;

	GridLayout m_layout;

	void OnAddInput(Container::Message& msg);
	void Input_OnConnect(Input::Message& msg);
	void Input_OnDisconnect(Input::Message& msg);
};

#endif