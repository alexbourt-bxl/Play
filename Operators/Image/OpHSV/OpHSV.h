#ifndef OP_HSV_H
#define OP_HSV_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>
#include <Controls\Layout\LineLayout\LineLayout.h>

#include <Types/Graphics\Image8.h>

class OpHSV
	: public Operator
{
	DECLARE_TYPE;

public:
	OpHSV();
	OpHSV(const OpHSV& op);
	~OpHSV();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	FloatProperty Hue;
	FloatProperty Saturation;
	FloatProperty Value;

private:
	Input	m_in;
	Output	m_out;

	LineLayout m_layoutVertical;

	// working
	double m_hue;
	double m_saturation;
	double m_value;
};

#endif