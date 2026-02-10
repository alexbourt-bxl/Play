#include "OpThreshold.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpThreshold, Operator);
SET_TYPE_DISPLAY_NAME(OpThreshold, L"Threshold");

OpThreshold::OpThreshold()
{
	Threshold = 0.5;

	Init();
}

OpThreshold::OpThreshold(const OpThreshold& op) 
{
	Threshold = op.Threshold;

	Init();
}

OpThreshold::~OpThreshold()
{

}

void OpThreshold::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);

	Threshold.Name = L"Threshold";
	Threshold.Valuebox->Relative = true;
	Threshold.Valuebox->MinValue = 0;
	Threshold.Valuebox->MinVisible = 0;
	Threshold.Valuebox->MaxValue = 1;
	Threshold.Valuebox->MaxVisible = 1;
	Threshold.Valuebox->Granularity = 0.01;
	Threshold.Valuebox->DisplayMultiplier = 100;
	Threshold.Valuebox->AdjustSpeed = 0.0025;
	Threshold.Valuebox->DecimalPlaces = 0;
	Threshold.Valuebox->Suffix = L"%";
	Properties.Add(&Threshold);
}

Object* OpThreshold::Copy()
{
	return OpThreshold(*this);
}

void OpThreshold::CopyWorkingValues()
{
	m_threshold = Threshold;
}

void OpThreshold::Process()
{
	Image* in = (Image*)m_in.GetInputObject();

	if (!in)
	{
		m_out.SetResult(null);
		return; 
	}

	assert(typeof(in) == TYPEOF(Image8));

	/*	Create the output image. */

	Size outSize = in->Size;

	Image8* out = 
		m_out.Result && typeof(m_out.Result) == TYPEOF(Image8) && ((Image8*)m_out.Result)->Size == outSize
		? (Image8*)m_out.Result
		: new Image8(outSize);

	///

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)out->Buffer;

	int yWidth;

	int intThreshold = Math::ToInt8(m_threshold);
	
	int c, r, g, b, a;
	int t;

	// Threshold

	// TODO: allow multiple thresholds

	int lut[256];

	for (int i = 0; i < 256; i++)
		lut[i] = i >= intThreshold ? 0xFF : 0;

	for (int y = 0; y < outSize.Height; y++)
	{
		yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
		{
			c = inBuffer[yWidth + x];

			r = (c & 0x00FF0000) >> 16;
			g = (c & 0x0000FF00) >>  8;
			b = (c & 0x000000FF);
			a = (c & 0xFF000000) >> 24;

			r = lut[r];
			g = lut[g];
			b = lut[b];
			a = lut[a];

			outBuffer[yWidth + x] = 
				(r << 16) |
				(g <<  8) |
				(b      ) |
				(a << 24);
		}
	}

	m_out.SetResult(out);
}