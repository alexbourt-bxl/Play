#include "OpPaint.h"

//#include <Controls\Views\ImageView\ImageView.h>
#include <Controls\Views\OpView\OpView.h>

DEFINE_TYPE(OpPaint, Operator);
SET_TYPE_DISPLAY_NAME(OpPaint, L"Paint");

OpPaint::OpPaint() 
{
	Init();
}

OpPaint::OpPaint(const OpPaint& /*op*/) 
{
	Init();
}

OpPaint::~OpPaint()
{
	if (m_outImage)
		delete m_outImage;

	if (m_imgPaint)
		delete m_imgPaint;

	if (m_imgErase)
		delete m_imgErase;
}

Object* OpPaint::Copy()
{
	return new OpPaint(*this);
}

void OpPaint::Init()
{
	m_in.ConnectEvent += EventHandler(this, &OpPaint::in_OnConnect);
	//m_in.DisconnectEvent += EventHandler(this, &OpPaint::in_OnDisconnect);

	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outImage = null;

	m_brush.Size = 20;
	m_brush.Hardness = 0;
	
	m_currentBrush = &m_brush;

	m_currentBrush->MoveStrokeEvent += EventHandler(this, &OpPaint::brush_OnMoveStroke);
	m_currentBrush->EndStrokeEvent  += EventHandler(this, &OpPaint::brush_OnEndStroke);

	m_imgPaint = null;
	m_imgErase = null;

	::QueryPerformanceFrequency(&m_perfFrequency);

	// Generate random color

	Math::Randomize(System::GetTickCount());

	BrushColor = Color(
		Math::Random(),
		Math::Random(),
		Math::Random());
}

void OpPaint::LayoutControls(LineLayout* layout)
{
	layout->Controls.Add(BrushColor.Control);
}

void OpPaint::CopyWorkingValues()
{

}

void OpPaint::ConnectView(OpView* view)
{
	view->PaintEvent       += EventHandler(this, &OpPaint::OnPaint);
	view->LButtonDownEvent += EventHandler(this, &OpPaint::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpPaint::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpPaint::OnMouseMove);
	view->PenMoveEvent     += EventHandler(this, &OpPaint::OnPenMove);
	view->KeyDownEvent     += EventHandler(this, &OpPaint::OnKeyDown);
	view->KeyUpEvent       += EventHandler(this, &OpPaint::OnKeyUp);
}

void OpPaint::DisconnectView(OpView* view)
{
	view->PaintEvent       -= EventHandler(this, &OpPaint::OnPaint);
	view->LButtonDownEvent -= EventHandler(this, &OpPaint::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpPaint::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpPaint::OnMouseMove);
	view->PenMoveEvent     -= EventHandler(this, &OpPaint::OnPenMove);
	view->KeyDownEvent     -= EventHandler(this, &OpPaint::OnKeyDown);
	view->KeyUpEvent       -= EventHandler(this, &OpPaint::OnKeyUp);
}

void OpPaint::Process()
{
	Image* in = m_in.Data.Get<Image>(Tag::Image);

	if (!in)
	{
		if (m_outImage)
		{
			delete m_outImage;
			m_outImage = null;
		}

		m_out.Data.Clear();
		return; 
	}

	/*	Create the output image. */

	Size outSize = in->Size;

	Image* outImage = m_out.Data.Get<Image>(Tag::Image);

	m_outImage = 
		outImage && typeof(outImage) == TYPEOF(Image8) && outImage->Size == outSize
		? (Image8*)outImage
		: new Image8(outSize);

	//Rect dirty = 
	//	DirtyRect.IsNull
	//	? Rect(0, 0, outSize.Width, outSize.Height)
	//	: DirtyRect;

	//dirty.Clip(Rect(0, 0, outSize.Width, outSize.Height));

	Graphics g(m_outImage);

	g.CopyImage(
		0, 
		0,
		in,
		0, 
		0,
		outSize.Width,
		outSize.Height);

	Point pOffset(0, 0);

	// Offset brush position by the offset
	Point* offset = m_in.Data.Get<Point>(Tag::ImageOffset);

	if (offset)
		pOffset += *offset;

	Rect paintRect(
		pOffset.X,
		pOffset.Y,
		outSize.Width,
		outSize.Height);

	paintRect.Clip(Rect(0, 0, m_imgPaint->Width, m_imgPaint->Height));

	g.DrawImage(
		0, 
		0,
		m_imgPaint,
		paintRect.X, 
		paintRect.Y,
		paintRect.Width,
		paintRect.Height);

	//g.CopyImage(
	//	dirty.X, 
	//	dirty.Y,
	//	in,
	//	dirty.X, 
	//	dirty.Y,
	//	dirty.Width,
	//	dirty.Height);

	//g.DrawImage(
	//	dirty.X,
	//	dirty.Y,
	//	m_imgPaint,
	//	dirty.X,
	//	dirty.Y,
	//	dirty.Width,
	//	dirty.Height);

	//g.DrawRect(
	//	dirty,
	//	Color::Red);

	m_out.Data = m_in.Data;
	m_out.Data.Set(Tag::Image, m_outImage);
	m_out.Data.Remove(Tag::ImagePreview);
}