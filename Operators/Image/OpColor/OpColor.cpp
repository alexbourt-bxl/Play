#include "OpColor.h"

#include <Graphics\Graphics.h>
#include <System\System.h>
#include <Math\Math.h>
#include <Controls\Valuebox\Valuebox.h>
#include <Controls\ColorFan\ColorFan.h>

DEFINE_TYPE(OpColor, Operator);
SET_TYPE_DISPLAY_NAME(OpColor, L"Color");

OpColor::OpColor() 
{
	// Generate random color

	Math::Randomize(System::GetTickCount());

	Color = _Color(
		Math::Random(),
		Math::Random(),
		Math::Random());

	Init();
}

OpColor::OpColor(const OpColor& op)
{
	Color  = op.Color;

	Init();
}

OpColor::~OpColor()
{
	if (m_outImage)
		delete m_outImage;
}

Object* OpColor::Copy()
{
	return new OpColor(*this);
}

void OpColor::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outImage = null;

	Color.Name = L"Color";
	Properties.Add(&Color);
}

void OpColor::LayoutControls(LineLayout* layout)
{
	layout->Controls.Add(Color.Control);
}

void OpColor::CopyWorkingValues()
{
	m_color = Color;
}

void OpColor::Process()
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

	/*	Paint the color. */

	Graphics g(m_outImage);
	
	g.FillRect(
		0,
		0,
		outSize.Width,
		outSize.Height,
		m_color);

	m_out.Data = m_in.Data;
	m_out.Data.Set(Tag::Image, m_outImage);
	m_out.Data.Remove(Tag::ImagePreview);
}