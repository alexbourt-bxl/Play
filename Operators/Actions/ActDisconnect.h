#ifndef ACT_DISCONNECT_H
#define ACT_DISCONNECT_H

#include <Actions/Action.h>

class Output;
class Input;

class ActDisconnect
	: public Action
{
	DECLARE_TYPE;

	typedef Output _Output;
	typedef Input _Input;

public:
	ActDisconnect();
	ActDisconnect(_Input* in);

	__declspec(property(get = GetInput, put = SetInput))
	_Input* Input;
	_Input* GetInput() const;
	void    SetInput(_Input* in);

	bool Perform();
	void Restore();

private:
	_Input*  m_in;

	_Output* m_oldOut;
	int m_oldConnCycles;
};

#endif