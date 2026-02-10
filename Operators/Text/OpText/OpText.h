#ifndef OP_TEXT_H
#define OP_TEXT_H

#include <Operators\Operator.h>
#include <Types/Basic\String.h>

/*	TODO: I don't know if there should be a separate Text type
which holds separate lines, or String should hold separate lines,
or OpText should hold text in separate lines and then give it out
ad a single string object. 

OpText should store font and formatting information, and respond to
Process(TYPEOF(GdiImage)) appropriately.

Should have Align.

Should also use Move, Scale, Resize, Offset, etc., also Bounds for drawing the text inside a rect.

Also <value></value> (or whatever encoding) to place values for real time display.

*/

class OpText
	: public Operator
{
	DECLARE_TYPE;

public:
	OpText();
	OpText(const OpText& op);
	~OpText();

	void	Init();
	Object*	Copy();

	bool	UsesSpec(Type specType, Type inType);
	bool	ForwardsSpec(Type specType);

	void	Process();

	String	Text;

private:
	Output	m_out;
};

#endif