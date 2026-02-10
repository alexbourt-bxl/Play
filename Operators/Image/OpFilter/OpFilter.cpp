#include "OpFilter.h"

DEFINE_TYPE(OpFilter, Operator);
SET_TYPE_DISPLAY_NAME(OpFilter, L"Filter");

OpFilter::OpFilter() 
{
	//Opacity = 1.0;

	Init();
}

OpFilter::OpFilter(const OpFilter& /*op*/) 
{
	//Opacity = op.Opacity;

	Init();
}

OpFilter::~OpFilter()
{

}

void OpFilter::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);

	//Opacity.Name = L"Opacity";
	//Opacity.Valuebox->Relative = true;
	//Opacity.Valuebox->MinValue = 0.0;
	//Opacity.Valuebox->MinVisible = 0.0;
	//Opacity.Valuebox->MaxValue = 1.0;
	//Opacity.Valuebox->MaxVisible = 1.0;
	//Opacity.Valuebox->Granularity = 0.01;
	//Opacity.Valuebox->DisplayMultiplier = 100.0;
	//Opacity.Valuebox->AdjustSpeed = 0.0025;
	//Opacity.Valuebox->DecimalPlaces = 0;
	//Opacity.Valuebox->Suffix = L"%";
	//Properties.Add(&Opacity);
}

Object* OpFilter::Copy()
{
	return OpFilter(*this);
}

void OpFilter::CopyWorkingValues()
{
	//m_opacity = Opacity;
}

void OpFilter::Process()
{
	Image* in = (Image*)m_in.GetInputObject();

	if (!in)
	{
		m_out.SetResult(null);
		return; 
	}

	/*	Determine the output dimensions.  */

	Size outSize(in->Width, in->Height);

	/*	Create the output image. If it has already been
		created, check if it needs to be resized. */

	Image* out = new Image8(outSize);

	/*	Change the opacity. */


	//Graphics g(out);

	//g.DrawImage(
	//	0, 0,
	//	in,
	//	m_opacity);

	m_out.SetResult(out);
}