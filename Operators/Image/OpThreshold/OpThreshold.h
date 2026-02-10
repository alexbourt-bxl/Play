#ifndef OP_THRESHOLD_H
#define OP_THRESHOLD_H

#include <Operators\Operator.h>
#include <Properties\FloatProperty\FloatProperty.h>

#include <Types/Graphics\Image8.h>

class OpThreshold
	: public Operator
{
	DECLARE_TYPE;

public:
	OpThreshold();
	OpThreshold(const OpThreshold& op);
	~OpThreshold();

	Object*	Copy();

	void	Init();
	//void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	FloatProperty Threshold;

private:
	Input	m_in;
	Output	m_out;

	// working
	double m_threshold;
};

#endif