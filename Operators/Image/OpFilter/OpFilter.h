#ifndef OP_FILTER_H
#define OP_FILTER_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>

#include <Types/Graphics\Image8.h>

class OpFilter
	: public Operator
{
	DECLARE_TYPE;

public:
	OpFilter();
	OpFilter(const OpFilter& op);
	~OpFilter();

	void	Init();
	Object*	Copy();

	void	CopyWorkingValues();
	void	Process();

	//FloatProperty Opacity;

private:
	Input	m_in;
	Output	m_out;

	/* Working values. */
	//double m_opacity;
};

#endif