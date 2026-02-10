#ifndef OP_MULTIPLY_H
#define OP_MULTIPLY_H

#include <Operators\Operator.h>

#include <Types/Graphics\Image8.h>

class OpMultiply
	: public Operator
{
	DECLARE_TYPE;

public:
	OpMultiply();
	OpMultiply(const OpMultiply& op);
	~OpMultiply();

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