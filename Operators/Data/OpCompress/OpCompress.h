#ifndef OP_COMPRESS_H
#define OP_COMPRESS_H

#include <Operators\Operator.h>
#include <Types\Properties\IntProperty\IntProperty.h>

const uint Marker_CMPR = 0x52504D43;

class OpCompress
	: public Operator
{
	DECLARE_TYPE;

public:
	OpCompress();
	OpCompress(const OpCompress& op);
	~OpCompress();

	String	GetOpName();

	void	Init();
	Object*	Copy();

	bool	UsesSpec(Type specType, Type inType);
	bool	ForwardsSpec(Type specType);

	void	UpdateResult(SpecList& specs);

	IntProperty Strength;

private:
	Input	m_in;
	Output	m_out;
};

#endif