#ifndef OP_NULL_H
#define OP_NULL_H

/*	The Null operator just passes whatever input it gets downstream. 
	
	It is useful, for example, for connecting an impersonated input 
	of an OpGraph to many internal inputs. This way the OpGraph's
	input is really OpNull's input, and opNull's real output is then
	passed along to as many inputs as necessary.
*/

#include <Operators\Operator.h>

class OpNull
	: public Operator
{
	DECLARE_TYPE;

public:
	OpNull();
	OpNull(const OpNull& op);
	~OpNull();

	void	Init();
	Object*	Copy();

	void	CopyWorkingValues();
	void	Process();

private:
	Input	m_in;
	Output	m_out;
};

#endif