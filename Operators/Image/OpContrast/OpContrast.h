#ifndef OP_CONTRAST_H
#define OP_CONTRAST_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>

#include <Types/Graphics\Image8.h>

class OpContrast
	: public Operator
{
	DECLARE_TYPE;

public:
	OpContrast();
	OpContrast(const OpContrast& op);
	~OpContrast();

	Object*	Copy();

	void	Init();
	//void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	FloatProperty Contrast;
	FloatProperty Midpoint;

private:
	Input	m_in;
	Output	m_out;

	// working
	double m_contrast;
	double m_midpoint;
};

#endif