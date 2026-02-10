#ifndef OP_DESERIALIZE_H
#define OP_DESERIALIZE_H

#include <Operators\Operator.h>

class OpDeserialize
	: public Operator
{
	DECLARE_TYPE;

public:
	OpDeserialize();
	OpDeserialize(const OpDeserialize& op);
	~OpDeserialize();

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