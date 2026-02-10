#ifndef OP_SATURATION_H
#define OP_SATURATION_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>

#include <Types/Graphics\Image8.h>

class OpSaturation
	: public Operator
{
	DECLARE_TYPE;

public:
	OpSaturation();
	OpSaturation(const OpSaturation& op);
	~OpSaturation();

	Object*	Copy();

	void	Init();
	//void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	FloatProperty Saturation;

private:
	Input	m_in;
	Output	m_out;

	// working
	double m_saturation;
};

#endif