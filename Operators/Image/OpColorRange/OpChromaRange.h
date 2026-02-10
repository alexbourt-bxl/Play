#ifndef OP_CHROMA_RANGE_H
#define OP_CHROMA_RANGE_H

#include <Operators\Operator.h>
#include <Properties\ColorProperty\ColorProperty.h>
#include <Properties\FloatProperty\FloatProperty.h>

#include <Types/Graphics\Image8.h>

class OpChromaRange
	: public Operator
{
	DECLARE_TYPE;

	typedef Color _Color;

public:
	OpChromaRange();
	OpChromaRange(const OpChromaRange& op);
	~OpChromaRange();

	Object*	Copy();

	void	Init();
	//void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	ColorProperty Color;

	FloatProperty ChromaSpread;
	FloatProperty ChromaTolerance;
	FloatProperty LumaSpread;

private:
	Input	m_in;
	Output	m_out;

	// working
	_Color m_color;
	double m_chromaSpread;
	double m_chromaTolerance;
	double m_lumaSpread;
};

#endif