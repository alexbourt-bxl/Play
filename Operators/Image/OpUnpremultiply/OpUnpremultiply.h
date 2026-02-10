#ifndef OP_UNPREMULTIPLY_H
#define OP_UNPREMULTIPLY_H

#include <Operators\Operator.h>

#include <Types/Graphics\Image8.h>

class OpUnpremultiply
	: public Operator
{
	DECLARE_TYPE;

public:
	OpUnpremultiply();
	OpUnpremultiply(const OpUnpremultiply& op);
	~OpUnpremultiply();

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