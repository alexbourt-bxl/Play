#ifndef ALIGN_H
#define ALIGN_H

#include <Types/BaseTypes.h>

class Align
{
public:
	Align();
	Align(const Align& align);
	Align(int _align);

	operator bool();

	bool	operator == (const Align& align);
	bool	operator != (const Align& align);

	Align&	operator =  (const Align& align);

	Align&	operator &= (const Align& align);
	Align	operator &  (const Align& align);
	Align&	operator |= (const Align& align);
	Align	operator |  (const Align& align);

private:
	int	m_align;

public:
	static Align None;
	static Align Default;

	static Align Left;
	static Align Center;
	static Align Right;

	static Align Top;
	static Align Middle;
	static Align Bottom;
	static Align Baseline;
};

#endif