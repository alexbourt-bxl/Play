#include "OpImage.h"

#include <Graphics\Graphics.h>
#include <System\System.h>
#include <Math\Math.h>
#include <Controls\Valuebox\Valuebox.h>
#include <Controls\Views\OpView\OpView.h>

DEFINE_TYPE(OpImage, Operator);
SET_TYPE_DISPLAY_NAME(OpImage, L"Image");

OpImage::OpImage() 
{
	Width = 640;
	Height = 480;

	Init();
}

OpImage::OpImage(const OpImage& op)
{
	Width  = op.Width;
	Height = op.Height;

	Init();
}

OpImage::~OpImage()
{
	if (m_outImage)
		delete m_outImage;
}

Object* OpImage::Copy()
{
	return new OpImage(*this);
}

void OpImage::Init()
{
	Out.Add(&m_out);
	m_outImage = null;

	Width.Name = L"Width";
	Properties.Add(&Width);

	Height.Name = L"Height";
	Properties.Add(&Height);

	m_grab = GrabNone;
	m_grabSize = 8;
}

void OpImage::LayoutControls(LineLayout* layout)
{
	Width.Valuebox->AdjustSpeed = 0.5;
	Width.Valuebox->MinValue = 1;
	Width.Valuebox->Suffix = L"px";
	Width.EnableAction = true;

	Height.Valuebox->AdjustSpeed = 0.5;
	Height.Valuebox->MinValue = 1;
	Height.Valuebox->Suffix = L"px";
	Height.EnableAction = true;

	m_layoutVertical.Orientation = LineLayout::Vertical;

	m_layoutVertical.Gap = 5;
	m_layoutVertical.AutoSize = true;
	m_layoutVertical.Controls.Add(Width.Valuebox);
	m_layoutVertical.Controls.Add(Height.Valuebox);

	layout->Controls.Add(&m_layoutVertical);
	layout->Gap = 30;
}

void OpImage::CopyWorkingValues()
{
	m_width = Width;
	m_height = Height;
}

void OpImage::Process()
{
	Image* outImage = m_out.Data.Get<Image>(Tag::Image);

	Size outSize(m_width, m_height);

	m_outImage =
		outImage && outImage->Size == outSize
		? outImage
		: new Image8(outSize);

	m_out.Data.Set(Tag::Image, m_outImage);
}

void OpImage::ConnectView(OpView* view)
{
	view->LButtonDownEvent += EventHandler(this, &OpImage::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpImage::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpImage::OnMouseMove);
}

void OpImage::DisconnectView(OpView* view)
{
	view->LButtonDownEvent -= EventHandler(this, &OpImage::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpImage::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpImage::OnMouseMove);
}