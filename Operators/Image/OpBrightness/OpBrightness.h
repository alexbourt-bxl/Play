#ifndef OP_BRIGHTNESS_H
#define OP_BRIGHTNESS_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>

#include <Types/Graphics\Image8.h>

class OpBrightness
	: public Operator
{
	DECLARE_TYPE;

public:
	OpBrightness();
	OpBrightness(const OpBrightness& op);
	~OpBrightness();

	Object*	Copy();

	void	Init();
	//void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	FloatProperty Gain;
	FloatProperty Offset;

private:
	Input	m_in;

	Output	m_out;
	Image*	m_outImage;

	// working
	double m_gain;
	double m_offset;
};

#endif