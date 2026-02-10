#ifndef OP_CONVERT_H
#define OP_CONVERT_H

#include <Operators\Operator.h>

/*	An explicit OpConvert was added to be able to convert, for example, 16-bit input
	into a GDI view. In general these issues should be addressed with properly constructed
	chains of direct ops, but you never know, so OpConvert could serve some purpose. */

class OpConvert
	: public Operator
{
	DECLARE_TYPE;

public:
	OpConvert();
	OpConvert(const OpConvert& op);
	~OpConvert();

	String	GetOpName();

	void	Init();
	Object*	Copy();

	bool	UsesSpec(Type specType, Type inType);
	bool	ForwardsSpec(Type specType);

	bool	_CheckDirty(SpecList& updateSpecs);

	void	UpdateResult(SpecList& updateSpecs);

	__Type*	OutType;

	bool	CanConvert(Type fromType, Type toType);

private:
	Input	m_in;
	Output	m_out;

	/*	If the result is transitive then it should not be deleted by the output. */
	bool	m_transitive;

	void	_Update_Image8();
	void	_Update_GdiImage();
	void	_Update_GdiImage8();
};

#endif

/*	TODO: 
	
	In the GUI, if an operator is being connected to one which doesn't accept the first
	one's type, try to see if a convert op can be automatically insterted between them (maybe
	with a cool sliding apart animation :). */