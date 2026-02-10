#ifndef OP_OVER_H
#define OP_OVER_H

#include <Operators\Operator.h>

#include <Types/Graphics\Image8.h>

class OpOver
	: public Operator
{
	DECLARE_TYPE;

public:
	OpOver();
	OpOver(const OpOver& op);
	~OpOver();

	void	Init();
	Object*	Copy();

	void	CopyWorkingValues();
	void	Process();

private:
	Input	m_inOver;
	Input	m_inUnder;

	Output	m_out;
	Image*	m_outImage;
};

#endif