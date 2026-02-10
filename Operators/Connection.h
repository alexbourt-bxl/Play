#ifndef CONNECTION_H
#define CONNECTION_H

/*	The cycles control should only be visible if
	the connection is cyclical. */

#include <Types/Object.h>
#include <Properties\PropertyContainer.h>
#include <Properties\IntProperty\IntProperty.h>

class Input;
class Output;

class Connection
	: public Object, public PropertyContainer
{
	DECLARE_TYPE;

public:
	Connection();
	Connection(const Connection& conn);
	Connection(Output* out, Input* in);

	Object* Copy();

	__declspec(property(get = GetInput, put = SetInput))
	Input* In;
	Input* GetInput();
	void SetInput(Input* in);

	__declspec(property(get = GetOutput, put = SetOutput))
	Output* Out;
	Output* GetOutput();
	void SetOutput(Output* out);

	//IntProperty Cycles;

	//__declspec(property(get = GetCyclesLeft))
	//int	CyclesLeft;
	//int GetCyclesLeft();

	//void Reset();

private:
	Input*	m_in;
	Output* m_out;

	//int m_cyclesLeft;
};

#endif