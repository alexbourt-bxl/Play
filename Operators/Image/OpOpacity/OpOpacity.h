#ifndef OP_OPACITY_H
#define OP_OPACITY_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>

#include <Types/Graphics\Image8.h>

class OpOpacity
	: public Operator
{
	DECLARE_TYPE;

public:
	OpOpacity();
	OpOpacity(const OpOpacity& op);
	~OpOpacity();

	void	Init();
	Object*	Copy();

	void	CopyWorkingValues();
	void	Process();

	FloatProperty Opacity;

private:
	Input	m_in;
	Output	m_out;

	/* Working values. */
	double m_opacity;
};

#endif