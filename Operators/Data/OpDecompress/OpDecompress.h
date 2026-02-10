#ifndef OP_DECOMPRESS_H
#define OP_DECOMPRESS_H

#include <Operators\Operator.h>

class OpDecompress
	: public Operator
{
	DECLARE_TYPE;

public:
	OpDecompress();
	OpDecompress(const OpDecompress& op);
	~OpDecompress();

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