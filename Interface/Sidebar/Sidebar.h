#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <Controls\Layout\LineLayout\LineLayout.h>

#include <Graphics\Graphics.h>
#include <Types/Graphics\GdiImage8.h>

class Sidebar
	: public LineLayout
{
	DECLARE_TYPE;

public:
	Sidebar();

private:
	static GdiImage8 s_imgShadow;

	void OnPaint(Graphics::Message& msg);
};

#endif