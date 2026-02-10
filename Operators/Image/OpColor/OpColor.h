#ifndef OP_COLOR_H
#define OP_COLOR_H

#include <Operators\Operator.h>
#include <Properties\IntProperty\IntProperty.h>
#include <Properties\ColorProperty\ColorProperty.h>
#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Controls\ColorFan\ColorFan.h>
#include <Controls\ColorButton\ColorButton.h>

#include <Types/Graphics\Image8.h>

class OpColor
	: public Operator
{
	DECLARE_TYPE;

	typedef Color _Color;

public:
	OpColor();
	OpColor(const OpColor& op);
	~OpColor();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	ColorProperty Color;

private:
	Input	m_in;

	Output	m_out;
	Image*	m_outImage;

	/* Working values. */
	_Color m_color;
};

#endif