#include "OpCrop.h"

#include <Controls\Valuebox\Valuebox.h>
#include <Controls\Views\OpView\OpView.h>

DEFINE_TYPE(OpCrop, Operator);
SET_TYPE_DISPLAY_NAME(OpCrop, L"Crop");

OpCrop::OpCrop() 
{
	Init();
}

OpCrop::OpCrop(const OpCrop& op) 
{
	Left = op.Left;
	Top = op.Top;
	Right = op.Right;
	Bottom = op.Bottom;

	Init();
}

OpCrop::~OpCrop()
{
	if (m_outImage)
		delete m_outImage;
}

Object* OpCrop::Copy()
{
	return new OpCrop(*this);
}

void OpCrop::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	m_in.ConnectEvent += EventHandler(this, &OpCrop::in_OnConnect);
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outImage = null;
	m_outOffset = Point(0, 0);

	m_btnTrim.PressEvent += EventHandler(this, &OpCrop::btnTrim_OnPress);

	Left.Name = L"Left";
	Left.PreChangeEvent += EventHandler(this, &OpCrop::Left_PreChange);
	Properties.Add(&Left);

	Right.Name = L"Right";
	Right.PreChangeEvent += EventHandler(this, &OpCrop::Right_PreChange);
	Properties.Add(&Right);
	
	Top.Name = L"Top";
	Top.PreChangeEvent += EventHandler(this, &OpCrop::Top_PreChange);
	Properties.Add(&Top);

	Bottom.Name = L"Bottom";
	Bottom.PreChangeEvent += EventHandler(this, &OpCrop::Bottom_PreChange);
	Properties.Add(&Bottom);

	Hide = 0.75;
	Hide.Name = L"Hide";
	Hide.Valuebox->Relative = true;
	Hide.Valuebox->MinValue = 0;
	Hide.Valuebox->MinVisible = 0;
	Hide.Valuebox->MaxValue = 1;
	Hide.Valuebox->MaxVisible = 1;
	Hide.Valuebox->Granularity = 0.01;
	Hide.Valuebox->DisplayMultiplier = 100;
	Hide.Valuebox->AdjustSpeed = 0.0025;
	Hide.Valuebox->DecimalPlaces = 0;
	Hide.Valuebox->Suffix = L"%";
	Properties.Add(&Hide);

	// TODO: need Valuebox presets to easily specify something
	// like a percent box, all of which work the same way.

	m_grab = GrabNone;
	m_grabSize = 8;

	m_init = false;
}

void OpCrop::LayoutControls(LineLayout* layout)
{
	m_btnTrim.Text = L"Trim";
	m_btnTrim.Width = 60;
	layout->Controls.Add(&m_btnTrim);

	Left.Valuebox->Width = 80;
	Left.Valuebox->AdjustSpeed = 0.5;
	Left.Valuebox->Relative = true;
	Left.Valuebox->Suffix = L"px";

	Right.Valuebox->Width = 80;
	Right.Valuebox->AdjustSpeed = 0.5;
	Right.Valuebox->Relative = true;
	Right.Valuebox->Suffix = L"px";

	Top.Valuebox->AdjustSpeed = 0.5;
	Top.Valuebox->Relative = true;
	Top.Valuebox->Suffix = L"px";

	Bottom.Valuebox->AdjustSpeed = 0.5;
	Bottom.Valuebox->Relative = true;
	Bottom.Valuebox->Suffix = L"px";

	m_layoutHorizontal.Orientation = LineLayout::Horizontal;
	m_layoutHorizontal.Gap = 5;
	m_layoutHorizontal.AutoSize = true;
	m_layoutHorizontal.Controls.Add(Left.Valuebox);
	m_layoutHorizontal.Controls.Add(Right.Valuebox);

	Top.Valuebox->Dock = Control::DockFillHorizontal;
	Bottom.Valuebox->Dock = Control::DockFillHorizontal;

	m_layoutVertical.Orientation = LineLayout::Vertical;
	m_layoutVertical.Gap = 5;
	m_layoutVertical.AutoSize = true;
	m_layoutVertical.Controls.Add(Top.Valuebox);
	m_layoutVertical.Controls.Add(&m_layoutHorizontal);
	m_layoutVertical.Controls.Add(Bottom.Valuebox);

	layout->Controls.Add(&m_layoutVertical);

	layout->Controls.Add(Hide.Control);

	if (!m_init)
	{
		Left.Valuebox->DisableEvents();
		Left.Valuebox->SetNA();
		Left.Valuebox->EnableEvents();

		Right.Valuebox->DisableEvents();
		Right.Valuebox->SetNA();
		Right.Valuebox->EnableEvents();

		Top.Valuebox->DisableEvents();
		Top.Valuebox->SetNA();
		Top.Valuebox->EnableEvents();

		Bottom.Valuebox->DisableEvents();
		Bottom.Valuebox->SetNA();
		Bottom.Valuebox->EnableEvents();
	}
}

void OpCrop::CopyWorkingValues()
{
	m_left   = Left;
	m_top    = Top;
	m_right  = Right;
	m_bottom = Bottom;
	m_hide   = Hide;
}

void OpCrop::Process()
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

	/*	Determine the output dimensions.  */

	Rect inRect(
		m_left,
		m_top,
		m_right - m_left,
		m_bottom - m_top);

	inRect.Clip(Rect(
		0, 0,
		in->Width,
		in->Height));

	Size outSize(
		m_right - m_left,
		m_bottom - m_top);

	Rect outRect(
		max(0, -m_left), 
		max(0, -m_top),
		inRect.Width,
		inRect.Height);

	/*	Create the output image. */

	Image* outImage = m_out.Data.Get<Image>(Tag::Image);

	m_outImage = 
		outImage && typeof(outImage) == TYPEOF(Image8) && outImage->Size == outSize
		? (Image8*)outImage
		: new Image8(outSize);

	/*	Crop the image. */

	Graphics g(m_outImage);
	
	g.ClearRect(
		0, 
		0, 
		m_outImage->Width, 
		m_outImage->Height);

	g.DrawImage(
		outRect.X, 
		outRect.Y,
		outRect.Width,
		outRect.Height,
		in,
		inRect.X, 
		inRect.Y,
		inRect.Width,
		inRect.Height);

	/*	Offset the result to match 
		the top left corner of the crop. */
	m_outOffset = Point(m_left, m_top);

	/*	Take into account any previous offsets. */

	Point* offset = m_in.Data.Get<Point>(Tag::ImageOffset);

	if (offset) 
		m_outOffset += *offset;

	//

	m_out.Data.Set(Tag::Image, m_outImage);
	m_out.Data.Set(Tag::ImagePreview, in);
	m_out.Data.Set(Tag::ImageOffset, &m_outOffset);
}

void OpCrop::ConnectView(OpView* view)
{
	view->PaintEvent       += EventHandler(this, &OpCrop::OnPaint);
	view->LButtonDownEvent += EventHandler(this, &OpCrop::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpCrop::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpCrop::OnMouseMove);
}

void OpCrop::DisconnectView(OpView* view)
{
	view->PaintEvent       -= EventHandler(this, &OpCrop::OnPaint);
	view->LButtonDownEvent -= EventHandler(this, &OpCrop::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpCrop::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpCrop::OnMouseMove);
}