#include "OpGradient.h"

#include <Graphics\Graphics.h>
#include <System\System.h>
#include <Math\Math.h>
#include <Controls\Valuebox\Valuebox.h>
#include <Controls\ColorFan\ColorFan.h>
#include <Controls\Views\OpView\OpView.h>

DEFINE_TYPE(OpGradient, Operator);
SET_TYPE_DISPLAY_NAME(OpGradient, L"Gradient");

OpGradient::OpGradient() 
{
	Init();
}

OpGradient::OpGradient(const OpGradient& /*op*/)
{
	Init();
}

OpGradient::~OpGradient()
{
	if (m_outImage)
		delete m_outImage;
}

Object* OpGradient::Copy()
{
	return new OpGradient(*this);
}

void OpGradient::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outImage = null;

	m_colorRadius = 10.5;
	m_colorBorder = 1;
	m_selectedBorder = 3;

	Type.Name = L"Type";
	Type.Button.AddItem(L"Linear");
	Type.Button.AddItem(L"Radial");
	Type.Button.AddItem(L"Angle");
	Type.Button.SelectedIndex = 0;
	Properties.Add(&Type);

	X0 = 0.5; Y0 = 0.5;
	X1 = 0.75; Y1 = 0.75;

	m_colors.Add(_Color::White); m_positions.Add(0);
	m_colors.Add(_Color(0, 0.5, 0)); m_positions.Add(0.3);
	m_colors.Add(_Color::Blue);   m_positions.Add(0.4);
	m_colors.Add(_Color(1, 0.3, 0));   m_positions.Add(0.405);
	m_colors.Add(_Color::Orange); m_positions.Add(0.75);
	m_colors.Add(_Color::Black); m_positions.Add(1);

	m_selected = 0;
	m_held = -2; // -2 is always none, -1 can be the center for angle gradients
}

void OpGradient::LayoutControls(LineLayout* layout)
{
	Type.Button.Width = 100;

	layout->Controls.Add(Type.Control);
	//layout->Controls.Add(Color.Control);
	layout->Gap = 30;
}

void OpGradient::CopyWorkingValues()
{
	m_type = Type.Button.SelectedIndex;
	//m_color  = Color; // TODO: replace with array
	m_p0 = FPoint((double)X0, (double)Y0);
	m_p1 = FPoint((double)X1, (double)Y1);
}

void OpGradient::Process()
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

	Image8* out8 = (Image8*)m_outImage;

	/*	Paint the gradient. */

	m_p0.X *= (double)outSize.Width;
	m_p0.Y *= (double)outSize.Height;
	m_p1.X *= (double)outSize.Width;
	m_p1.Y *= (double)outSize.Height;

	switch (m_type)
	{
	case 0:
		Update_Linear(
			(int*)out8->Buffer,
			out8->Width, 
			out8->Height);
		break;

	case 1:
		Update_Radial(
			(int*)out8->Buffer,
			out8->Width, 
			out8->Height);
		break;

	case 2:
		Update_Angle(
			(int*)out8->Buffer,
			out8->Width, 
			out8->Height);
		break;
	}

	m_out.Data.Set(Tag::Image, m_outImage);
}

void OpGradient::ConnectView(OpView* view)
{
	view->PaintEvent       += EventHandler(this, &OpGradient::OnPaint);
	view->LButtonDownEvent += EventHandler(this, &OpGradient::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpGradient::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpGradient::OnMouseMove);
}

void OpGradient::DisconnectView(OpView* view)
{
	view->PaintEvent       -= EventHandler(this, &OpGradient::OnPaint);
	view->LButtonDownEvent -= EventHandler(this, &OpGradient::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpGradient::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpGradient::OnMouseMove);
}