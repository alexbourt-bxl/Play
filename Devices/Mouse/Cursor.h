#ifndef CURSOR_H
#define CURSOR_H

#include <Native\Resource.h>
#include <Resources\resource.h>

class Cursor
{
public:
	Cursor();
	Cursor(WORD id, bool animated = false);
	~Cursor();

	HCURSOR			ToHCURSOR();

private:
	HCURSOR			m_hcursor;

public: // Static

	static Cursor 	Arrow;
	static Cursor	Ghost;
	static Cursor 	Aim;
	static Cursor	Hand;
	static Cursor	HandPressed;
	static Cursor	Magnify;
	static Cursor	Move;
	static Cursor	ScaleLR;
	static Cursor	ScaleTB;
	static Cursor	ScaleTLBR;
	static Cursor	ScaleBLTR;
	static Cursor	RotateTL;
	static Cursor	RotateT;
	static Cursor	RotateTR;
	static Cursor	RotateR;
	static Cursor	RotateBR;
	static Cursor	RotateB;
	static Cursor	RotateBL;
	static Cursor	RotateL;
	static Cursor	Dot;
	static Cursor	LargeDot;
	static Cursor	Add;
	static Cursor	Remove;
	static Cursor	Complete;
	static Cursor	Adjust;
};

#endif