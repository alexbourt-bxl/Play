#ifndef ACT_CONNECT_H
#define ACT_CONNECT_H

#include <Actions/Action.h>

class Output;
class Input;
class Connection;

class ActConnect
	: public Action
{
	DECLARE_TYPE;

	typedef Output _Output;
	typedef Input _Input;

public:
	ActConnect();
	ActConnect(_Output* out, _Input* in);

	__declspec(property(get = GetOutput, put = SetOutput))
	_Output* Output;
	_Output* GetOutput() const;
	void    SetOutput(_Output* out);

	__declspec(property(get = GetInput, put = SetInput))
	_Input* Input;
	_Input* GetInput() const;
	void    SetInput(_Input* in);

	bool Perform();
	void Restore();

private:
	_Output* m_out;
	_Input*  m_in;

	_Output* m_oldOut;
	int m_oldConnCycles;
};

#endif