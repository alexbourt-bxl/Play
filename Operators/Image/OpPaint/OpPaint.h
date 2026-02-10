#ifndef OP_PAINT_H
#define OP_PAINT_H

#include <Operators\Operator.h>
//#include <Operators\Image\ImageOperator.h>
#include <Properties\ColorProperty\ColorProperty.h>
#include <Types/Graphics\Image8.h>

#include "Paint Engine\Brush.h"

//class OpView;

class OpPaint
	: public Operator
{
	DECLARE_TYPE;

public:
	OpPaint();
	OpPaint(const OpPaint& op);
	~OpPaint();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	ColorProperty BrushColor;

	void	ConnectView(OpView* view);
	void	DisconnectView(OpView* view);

private:
	Input	m_in;

	Output	m_out;
	Image*	m_outImage;

	/*	The actual painting is done on a temp surface
		which is then composited with the input during
		an update. This way if an input is resized or
		changed, the painted pixels are not lost. (This
		is only an issue for raster paint.) */
	Image*	m_imgPaint;
	Image*	m_imgErase;

	Brush	m_brush;

	Brush*	m_currentBrush;

	LARGE_INTEGER m_perfFrequency;
	LARGE_INTEGER m_lastPerfCount;
	bool	m_moved;

	//

	void OnPaint(Graphics::Message& msg);
	void OnLButtonDown(Mouse::Message& msg);
	void OnLButtonUp(Mouse::Message& msg);
	void OnMouseMove(Mouse::Message& msg);
	void OnPenMove(Tablet::Message& msg);
	void OnKeyDown(Key::Message& msg);
	void OnKeyUp(Key::Message& msg);

	void brush_OnMoveStroke(StrokeMessage& msg);
	void brush_OnEndStroke(StrokeMessage& msg);

	void in_OnConnect(Input::Message& msg);
	//void in_OnDisconnect(Input::Message& msg);
};

#endif