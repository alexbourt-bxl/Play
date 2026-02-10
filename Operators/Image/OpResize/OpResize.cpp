#include "OpResize.h"

#include <Math\Math.h>
#include <Controls\Valuebox\Valuebox.h>

#include <assert.h>

DEFINE_TYPE(OpResize, Operator);
SET_TYPE_DISPLAY_NAME(OpResize, L"Resize");

OpResize::OpResize() 
{
	Width  = 0;
	Height = 0;

	Init();
}

OpResize::OpResize(const OpResize& op)
{
	Width  = op.Width;
	Height = op.Height;
	
	Init();
}

OpResize::~OpResize()
{

}

void OpResize::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	m_in.ConnectEvent += EventHandler(this, &OpResize::in_OnConnect);
	In.Add(&m_in);

	Out.Add(&m_out);

	m_btnProportional.PressEvent += EventHandler(this, &OpResize::btnProportional_OnPress);

	Width.Name = L"Width";
	Width.Valuebox->PreValueChangeEvent += EventHandler(this, &OpResize::Width_Valuebox_PreValueChange);
	Width.Valuebox->ValueChangeEvent += EventHandler(this, &OpResize::Width_Valuebox_OnValueChange);
	Properties.Add(&Width);

	Height.Name = L"Height";
	Height.Valuebox->PreValueChangeEvent += EventHandler(this, &OpResize::Height_Valuebox_PreValueChange);
	Height.Valuebox->ValueChangeEvent += EventHandler(this, &OpResize::Height_Valuebox_OnValueChange);
	Properties.Add(&Height);

	Interpolation.Name = L"Interpolation";
	Interpolation.Button.AddItem(L"Nearest");
	Interpolation.Button.AddItem(L"Bilinear");
	Interpolation.Button.AddItem(L"Bicubic");
	//Interpolation.SelectItemEvent += EventHandler(this, &OpResize::Interpolation_OnSelectItem);
	Interpolation.Button.SelectedIndex = 1;
	Properties.Add(&Interpolation);

	Subpixel.Name = L"Subpixel";
	Subpixel.Button->Width = 80;
	Subpixel = true;
	Properties.Add(&Subpixel);

	m_init = false;
}

void OpResize::LayoutControls(LineLayout* layout)
{
	m_btnProportional.SetIcon(&s_imgProportional);
	m_btnProportional.Dock = Control::DockFillVertical;
	m_btnProportional.SetSize(31, 30);
	m_btnProportional.Highlight = true;

	Width.Valuebox->AdjustSpeed = 0.5;
	Width.Valuebox->MinValue = 1.0;
	Width.Valuebox->Relative = true;
	Width.Valuebox->Suffix = L"px";

	Height.Valuebox->AdjustSpeed = 0.5;
	Height.Valuebox->MinValue = 1.0;
	Height.Valuebox->Relative = true;
	Height.Valuebox->Suffix = L"px";

	Interpolation.Button.Width = 100;

	m_layoutDimensions2.Orientation = LineLayout::Vertical;
	m_layoutDimensions2.Gap = 5;
	m_layoutDimensions2.AutoSize = true;
	m_layoutDimensions2.Controls.Add(Width.Valuebox);
	m_layoutDimensions2.Controls.Add(Height.Valuebox);

	m_layoutDimensions.Orientation = LineLayout::Horizontal;
	m_layoutDimensions.Gap = 5;
	m_layoutDimensions.AutoSize = true;
	m_layoutDimensions.Controls.Add(&m_btnProportional);
	m_layoutDimensions.Controls.Add(&m_layoutDimensions2);

	layout->Gap = 30;
	layout->Controls.Add(&m_layoutDimensions);
	layout->Controls.Add(Interpolation.Control);
	layout->Controls.Add(Subpixel.Control);

	if (!m_init)
	{
		Width.Valuebox->DisableEvents();
		Width.Valuebox->SetNA();
		Width.Valuebox->EnableEvents();

		Height.Valuebox->DisableEvents();
		Height.Valuebox->SetNA();
		Height.Valuebox->EnableEvents();
	}
}

Object* OpResize::Copy()
{
	return OpResize(*this);
}

void OpResize::CopyWorkingValues()
{
	m_width = Width;
	m_height = Height;
	m_interpolation = _interpolation;
	m_subpixel = Subpixel;
}

void OpResize::Process()
{
	Image* in = (Image*)m_in.GetInputObject();

	if (!in)
	{
		m_out.SetResult(null);
		return; 
	}

	/*	Create the output image. */

	Size outSize(m_width, m_height);

	Image8* out = 
		m_out.Result && typeof(m_out.Result) == TYPEOF(Image8) && ((Image8*)m_out.Result)->Size == outSize
		? (Image8*)m_out.Result
		: new Image8(outSize);

	/*	Resize the image. */

	Graphics g(out);

	g.Interpolation = m_interpolation;
	g.Subpixel = m_subpixel;

	g.ClearRect(
		0, 0,
		outSize.Width,
		outSize.Height);

	g.DrawImage(
		0, 0,
		outSize.Width,
		outSize.Height,
		in,
		0, 0,
		in->Width,
		in->Height);

	/*	Optionally offset the result to match 
		the top left corner of the crop. */
	out->Offset -= Point(
		(outSize.Width - in->Width) / 2,
		(outSize.Height - in->Height) / 2);

	m_out.SetResult(out);
}