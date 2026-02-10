#ifndef OP_PREMULTIPLY_H
#define OP_PREMULTIPLY_H

#include <Operators\Operator.h>

#include <Types/Graphics\Image8.h>

class OpPremultiply
	: public Operator
{
	DECLARE_TYPE;

public:
	OpPremultiply();
	OpPremultiply(const OpPremultiply& op);
	~OpPremultiply();

	Object*	Copy();

	void	Init();
	//void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

private:
	Input	m_in;
	Output	m_out;
};

#endif