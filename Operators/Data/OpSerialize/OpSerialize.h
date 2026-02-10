#ifndef OP_SERIALIZE_H
#define OP_SERIALIZE_H

#include <Operators\Operator.h>

class OpSerialize
	: public Operator
{
	DECLARE_TYPE;

public:
	OpSerialize();
	OpSerialize(const OpSerialize& op);
	~OpSerialize();

	String	GetOpName();

	void	Init();
	Object*	Copy();

	bool	UsesSpec(Type specType, Type inType);
	bool	ForwardsSpec(Type specType);

	void	UpdateResult(SpecList& specs);

private:
	Input	m_in;
	Output	m_out;
};

#endif