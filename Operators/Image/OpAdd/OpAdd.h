#ifndef OP_ADD_H
#define OP_ADD_H

#include <Operators\Operator.h>

#include <Types/Graphics\Image8.h>

class OpAdd
	: public Operator
{
	DECLARE_TYPE;

public:
	OpAdd();
	OpAdd(const OpAdd& op);
	~OpAdd();

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