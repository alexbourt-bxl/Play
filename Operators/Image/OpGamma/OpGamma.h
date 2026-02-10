#ifndef OP_GAMMA_H
#define OP_GAMMA_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>
#include <Properties\BoolProperty\BoolProperty.h>

#include <Types/Graphics\Image8.h>

class OpGamma
	: public Operator
{
	DECLARE_TYPE;

public:
	OpGamma();
	OpGamma(const OpGamma& op);
	~OpGamma();

	Object*	Copy();

	void	Init();
	//void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	FloatProperty Gamma;
	BoolProperty  Invert;

private:
	Input	m_in;
	Output	m_out;

	// working
	double m_gamma;
	double m_invert;
};

#endif