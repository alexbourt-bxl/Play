#include "OpOpacity.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpOpacity, Operator);
SET_TYPE_DISPLAY_NAME(OpOpacity, L"Opacity");

OpOpacity::OpOpacity()
{
	Opacity = 1.0;

	Init();
}

OpOpacity::OpOpacity(const OpOpacity& op) 
{
	Opacity = op.Opacity;

	Init();
}

OpOpacity::~OpOpacity()
{

}

void OpOpacity::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);

	Opacity.Name = L"Opacity";
	Opacity.Valuebox->Relative = true;
	Opacity.Valuebox->MinValue = 0.0;
	Opacity.Valuebox->MinVisible = 0.0;
	Opacity.Valuebox->MaxValue = 1.0;
	Opacity.Valuebox->MaxVisible = 1.0;
	Opacity.Valuebox->Granularity = 0.01;
	Opacity.Valuebox->DisplayMultiplier = 100.0;
	Opacity.Valuebox->AdjustSpeed = 0.0025;
	Opacity.Valuebox->DecimalPlaces = 0;
	Opacity.Valuebox->Suffix = L"%";
	Properties.Add(&Opacity);
}

Object* OpOpacity::Copy()
{
	return OpOpacity(*this);
}

void OpOpacity::CopyWorkingValues()
{
	m_opacity = Opacity;
}

void OpOpacity::Process()
{
	Image* in = (Image*)m_in.GetInputObject();

	if (!in)
	{
		m_out.SetResult(null);
		return; 
	}

	/*	Create the output image. */

	Size outSize = in->Size;

	Image8* out = 
		m_out.Result && typeof(m_out.Result) == TYPEOF(Image8) && ((Image8*)m_out.Result)->Size == outSize
		? (Image8*)m_out.Result
		: new Image8(outSize);

	/*	Change the opacity. */

	Graphics g(out);

	g.ClearRect(
		0, 0,
		outSize.Width, 
		outSize.Height);

	g.DrawImage(
		0, 0,
		in,
		m_opacity);

	m_out.SetResult(out);
}