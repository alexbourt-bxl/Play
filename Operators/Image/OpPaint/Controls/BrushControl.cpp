#include "BrushControl.h"

//#include "..\Brushes\Brush.h"
//#include "..\Brushes\BrushStroke.h"
////#include "PaintTool.h"
//
//DEFINE_TYPE(BrushControl, Control);
//
//BrushControl::BrushControl()
//{
//	brush = null;
//	stroke = null;
//	
//	strokeColor = Color::Black;
//
//	imgBrush = null;
//	imgBrushSmall = null;
//	
//	m_pressed = false;
//	m_dragged = false;
//	m_markedForDelete = false;
//
//	storeSize.height = 0;
//
//	Selected.data[0] = this;
//	Selected.m_value = 0.0f;
//	Selected.ChangeEvent += EventHandler(this, &BrushControl::value_OnChange);
//
//	StrokeOpacity.data[0] = this;
//	StrokeOpacity.m_value = 1.0f;
//	StrokeOpacity.ChangeEvent += EventHandler(this, &BrushControl::value_OnChange);
//
//	ClipOffsetY.data[0] = this;
//	ClipOffsetY.m_value = 0;
//	ClipOffsetY.ChangeEvent += EventHandler(this, &BrushControl::value_OnChange);
//
//	Deleted.data[0] = this;
//	Deleted.m_value = 0.0f;
//	Deleted.ChangeEvent += EventHandler(this, &BrushControl::value_OnChange);
//
//	canDelete = false;
//
//	PaintEvent		= EventHandler(this, &BrushControl::OnPaint);
//	MouseEnterEvent	= EventHandler(this, &BrushControl::OnMouseEnter);
//	MouseLeaveEvent	= EventHandler(this, &BrushControl::OnMouseLeave);
//	LButtonDownEvent= EventHandler(this, &BrushControl::OnLButtonDown);
//	LButtonUpEvent	= EventHandler(this, &BrushControl::OnLButtonUp);
//	MouseMoveEvent	= EventHandler(this, &BrushControl::OnMouseMove);
//
//	SetSize(44, 128);
//}
//
//BrushControl::~BrushControl()
//{
//	if (stroke)
//	{
//		delete stroke;
//		stroke = null;
//	}
//}
//
//Rect BrushControl::GetPaintRect()
//{
//	Rect paintRect = Control::GetPaintRect();
//
//	if (imgBrush)
//	{
//		int dw = imgBrush->GetSize().width - Width;
//
//		if (dw > 0)
//		{
//			paintRect.x -= dw / 2;
//			paintRect.width += dw;
//		}
//
//		int dh = imgBrush->GetSize().height - Height;
//
//		if (dh > 0)
//		{
//			paintRect.y -= dh / 2;
//			paintRect.height += dh;
//		}
//	}
//
//	// Allow a large brush stroke to be drawn
//
//	if (stroke && brush)
//	{
//		paintRect.x -= (int)(brush->Size.value.set * (1.0f + brush->ScatterAcross.value.set)) / 2;
//		paintRect.width += (int)(brush->Size.value.set * (1.0f + brush->ScatterAcross.value.set));
//	}
//
//	return paintRect;
//}
//
//bool BrushControl::GetIgnoreParentClip()
//{
//	return true;
//}
//
//void BrushControl::SetBrush(Brush* _brush)
//{
//	brush = _brush;
//
//	if (brush)
//	{
//		switch (brush->type)
//		{
//		case Brush::Stylus:
//		default:
//			SetBrushImage(&s_imgStylus, &s_imgStylusSmall);
//			break;
//
//		case Brush::Pencil:
//			strokeColor = Color(0xFF353730);
//			SetBrushImage(&s_imgPencil, &s_imgPencilSmall);
//			break;
//
//		case Brush::BallpointPen:
//			strokeColor = Color(0xFF080030);
//			SetBrushImage(&s_imgBallpointPen, &s_imgBallpointPenSmall);
//			break;
//
//		case Brush::CaligraphyPen:
//			SetBrushImage(&s_imgCaligraphyPen, &s_imgCaligraphyPenSmall);
//			break;
//
//		case Brush::Chalk:
//			strokeColor = Color::White;
//			SetBrushImage(&s_imgChalk, &s_imgChalkSmall);
//			break;
//
//		case Brush::Marker:
//			strokeColor = Color(0xFFFF6000);
//			SetBrushImage(&s_imgMarker, &s_imgMarkerSmall);
//			break;
//
//		case Brush::Paintbrush:
//			SetBrushImage(&s_imgBrush, &s_imgBrushSmall);
//			break;
//
//		case Brush::Airbrush:
//			SetBrushImage(&s_imgAirbrush, &s_imgAirbrushSmall);
//			break;
//
//		case Brush::Eraser:
//			SetBrushImage(&s_imgEraser, &s_imgEraserSmall);
//			break;
//		}
//	}
//}
//
//void BrushControl::SetBrushImage(Image* _imgBrush, Image* _imgBrushSmall)
//{
//	imgBrush = _imgBrush;
//	imgBrushSmall = _imgBrushSmall;
//}
//
//void BrushControl::UpdateStroke()
//{
//	if (stroke)
//	{
//		delete stroke;
//		stroke = null;
//	}
//
//	stroke = new BrushStroke(brush, strokeColor, Image::GdiMem);
//		stroke->targetSpace = &stroke->space;
//		stroke->AddPoint(new StrokePoint(4.0f, -20.0f, 1.0f, 90.0f, 45.0f, 0.0f, 100.0f));
//		stroke->AddPoint(new StrokePoint(0.0f, 0.0f, 1.0f, 0.0f, 90.0f, 0.0f, 100.0f));
//		stroke->AddPoint(new StrokePoint(-4.0f, 20.0f, 1.0f, -90.0f, 45.0f, 0.0f, 100.0f));
//		stroke->AddPoint(new StrokePoint(2.0f, 40.0f, 1.0f, 0.0f, 90.0f, 0.0f, 100.0f));
//		stroke->AddPoint(new StrokePoint(4.0f, 60.0f, 0.66f, 90.0f, 45.0f, 0.0f, 100.0f));
//		stroke->AddPoint(new StrokePoint(0.0f, 80.0f, 0.33f, 0.0f, 0.0f, 0.0f, 100.0f));
//		stroke->AddPoint(null);
//
//	brush->SetSeed(0x8CA2);
//
//	Brush::painter.DrawStroke(stroke);
//}
//
//void BrushControl::BreakOutOfPalette()
//{
//	PaintTool* paintTool = (PaintTool*)brush->data[0];
//
//	m_markedForDelete = true;
//
//	// Break the control out of the palette
//	// and make it independent.
//
//	Point po(0, 0);
//	paintTool->m_pnlBrushes.PointToWindow(&po);
//
//	m_posOffset -= po;
//	storePoint = po;
//
//	paintTool->m_pnlBrushes.window->SuspendUpdates();
//	{
//		paintTool->m_pnlBrushes.controls.Remove(this);
//		paintTool->m_pnlBrushes.window->dragged.Add(this, false);
//	}
//	paintTool->m_pnlBrushes.window->ResumeUpdates();
//
//	paintTool->emptyBrushIndex = -1;
//
//	data[1] = (void*)paintTool->GetBrushIndex(brush);
//
//	paintTool->RemoveBrush(brush);
//	paintTool->UpdateBrushPalette();
//
//	// Animate the transition
//
//	FloatAnimation* anim = new FloatAnimation();
//		anim->value = &Deleted;
//		anim->to = 1.0f;
//		anim->duration = 0.1f;
//		anim->curve = Animation::Sine;
//	anim->Start();
//}
//
//void BrushControl::BreakIntoPalette()
//{
//	PaintTool* paintTool = (PaintTool*)brush->data[0];
//
//	m_markedForDelete = false;
//
//	// Install the control back into the palette
//
//	//Point po(0, 0);
//	//paintTool->m_pnlBrushes.PointToWindow(&po);
//
//	m_posOffset += storePoint; // po
//
//	paintTool->m_pnlBrushes.window->SuspendUpdates();
//	{
//		paintTool->m_pnlBrushes.window->dragged.Remove(this);
//		paintTool->m_pnlBrushes.controls.Add(this, false);
//	}
//	paintTool->m_pnlBrushes.window->ResumeUpdates();
//
//	paintTool->AddBrush(brush);
//	//paintTool->SetBrush(brush);
//
//	// Animate the transition
//
//	FloatAnimation* anim = new FloatAnimation();
//		anim->value = &Deleted;
//		anim->to = 0.0f;
//		anim->duration = 0.1f;
//		anim->curve = Animation::Sine;
//	anim->Start();
//}