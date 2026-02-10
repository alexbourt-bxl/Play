#ifndef OP_CHANNELS_H
#define OP_CHANNELS_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>
#include <Types/Graphics\Image8.h>
#include "RewireControl.h"

class OpChannels
	: public Operator
{
	DECLARE_TYPE;

public:
	OpChannels();
	OpChannels(const OpChannels& op);
	~OpChannels();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

private:
	Input	m_in;

	Output	m_out;
	Image*	m_outImage;

	RewireControl m_control;
	void control_OnChange(Control::Message& msg);

	// working
	int	m_r, m_g, m_b, m_a; // 0 = nothing, 1 = Red, 2 = Green, 3 = Blue, 4 = Alpha
};

#endif