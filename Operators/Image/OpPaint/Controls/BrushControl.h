#ifndef BRUSH_CONTROL_H
#define BRUSH_CONTROL_H

//#include <Controls\Control.h>
//
//#include <Types/Graphics\Image8.h>
//
//#include <Properties\FloatProperty\FloatProperty.h>
//#include <Properties\IntProperty\IntProperty.h>
//#include <Animation\Animation.h>
//
//class Brush;
//class BrushStroke;
//
//class BrushControl
//	: public Control
//{
//	DECLARE_TYPE;
//
//public:
//	Brush*			brush;
//	BrushStroke*	stroke;
//
//	Color			strokeColor;
//
//	Image*			imgBrush;
//	Image*			imgBrushSmall;
//
//	FloatProperty	Selected;
//	FloatProperty	StrokeOpacity;
//	IntProperty		ClipOffsetY;
//	FloatProperty	Deleted;
//	void			value_OnChange(Message msg);
//
//	bool			canDelete;
//
//public:
//	BrushControl();
//	~BrushControl();
//
//	Rect			GetPaintRect();
//	bool			GetIgnoreParentClip();
//
//	void			SetBrush(Brush* brush);
//	void			SetBrushImage(Image* imgBrush, Image* imgBrushSmall);
//
//	void			UpdateStroke();
//
//	void			BreakOutOfPalette();
//	void			BreakIntoPalette();
//
//private:
//	bool			m_pressed;
//	bool			m_dragged;
//	Point			m_posStart;
//	Point			m_posOffset;
//	bool			m_markedForDelete;
//
//private:
//	static Image	s_imgDelete;
//	static Image	s_imgStylus;
//	static Image	s_imgStylusSmall;
//	static Image	s_imgPencil;
//	static Image	s_imgPencilSmall;
//	static Image	s_imgBallpointPen;
//	static Image	s_imgBallpointPenSmall;
//	static Image	s_imgCaligraphyPen;
//	static Image	s_imgCaligraphyPenSmall;
//	static Image	s_imgMarker;
//	static Image	s_imgMarkerSmall;
//	static Image	s_imgChalk;
//	static Image	s_imgChalkSmall;
//	static Image	s_imgBrush;
//	static Image	s_imgBrushSmall;
//	static Image	s_imgAirbrush;
//	static Image	s_imgAirbrushSmall;
//	static Image	s_imgEraser;
//	static Image	s_imgEraserSmall;
//
//public:
//	void			OnPaint(Control::PaintEvent msg);
//	void			OnMouseEnter(Mouse::Message msg);
//	void			OnMouseLeave(Mouse::Message msg);
//	void			OnLButtonDown(Mouse::Message msg);
//	void			OnLButtonUp(Mouse::Message msg);
//	void			OnMouseMove(Mouse::Message msg);
//
//	void			DeleteBrush(AnimationMessage msg);
//};

#endif