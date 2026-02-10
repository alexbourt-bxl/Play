#ifndef OP_NOISE_H
#define OP_NOISE_H

#include <Operators\Operator.h>
#include <Properties\IntProperty\IntProperty.h>
#include <Controls\Layout\LineLayout\LineLayout.h>

#include <Types/Graphics\Image8.h>

class OpNoise
	: public Operator
{
	DECLARE_TYPE;

public:
	OpNoise();
	OpNoise(const OpNoise& op);
	~OpNoise();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	IntProperty Seed;

private:
	Input	m_in;

	Output	m_out;
	Image*	m_outImage;

	uint	m_initSeed;

	// working
	int	m_seed;  
};

#endif