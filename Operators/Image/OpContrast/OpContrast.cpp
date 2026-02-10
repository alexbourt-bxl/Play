#include "OpContrast.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpContrast, Operator);
SET_TYPE_DISPLAY_NAME(OpContrast, L"Contrast");

OpContrast::OpContrast()
{
	Contrast = 1;
	Midpoint = 0.5;

	Init();
}

OpContrast::OpContrast(const OpContrast& op) 
{
	Contrast = op.Contrast;
	Midpoint = op.Midpoint;

	Init();
}

OpContrast::~OpContrast()
{

}

void OpContrast::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);

	Contrast.Name = L"Contrast";
	Contrast.Valuebox->Relative = true;
	Contrast.Valuebox->MinValue = 0;
	Contrast.Valuebox->MinVisible = 0;
	Contrast.Valuebox->MaxVisible = 2;
	Contrast.Valuebox->Granularity = 0.01;
	Contrast.Valuebox->DisplayMultiplier = 100;
	Contrast.Valuebox->AdjustSpeed = 0.0025;
	Contrast.Valuebox->DecimalPlaces = 0;
	Contrast.Valuebox->Suffix = L"%";
	Properties.Add(&Contrast);

	Midpoint.Name = L"Midpoint";
	Midpoint.Valuebox->Relative = true;
	Midpoint.Valuebox->MinValue = 0;
	Midpoint.Valuebox->MinVisible = 0;
	Midpoint.Valuebox->MaxValue = 1;
	Midpoint.Valuebox->MaxVisible = 1;
	Midpoint.Valuebox->Granularity = 0.01;
	Midpoint.Valuebox->DisplayMultiplier = 100;
	Midpoint.Valuebox->AdjustSpeed = 0.0025;
	Midpoint.Valuebox->DecimalPlaces = 0;
	Midpoint.Valuebox->Suffix = L"%";
	Properties.Add(&Midpoint);
}

Object* OpContrast::Copy()
{
	return OpContrast(*this);
}

void OpContrast::CopyWorkingValues()
{
	m_contrast = Contrast;
	m_midpoint = Midpoint;
}

void OpContrast::Process()
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

	/*	Change the brightness. */

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)out->Buffer;

	int yWidth;

	int intContrast = Math::ToInt8(m_contrast);
	int intMidpoint = Math::ToInt8(m_midpoint);
	
	// Determine the average color of the image

	int c, r, g, b, a;
	int avgR = 0, avgG = 0, avgB = 0;
	int t, count = 0;

	for (int y = 0; y < outSize.Height; y++)
	{
		yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
		{
			c = inBuffer[yWidth + x];

			avgR += (c >> 16) & 0xFF;
			avgG += (c >>  8) & 0xFF;
			avgB += (c      ) & 0xFF;

			count++;
		}
	}

	avgR = avgR / count - 0x80;
	avgG = avgG / count - 0x80;
	avgB = avgB / count - 0x80;

	int midpointR = intMidpoint + avgR;
	int midpointG = intMidpoint + avgG;
	int midpointB = intMidpoint + avgB;

	// Adjust the contrast

	int lutR[256], lutG[256], lutB[256];

	for (int i = 0; i < 256; i++)
	{
		lutR[i] = min(max(0, INT_MULT(i - midpointR, intContrast, t) + midpointR), 0xFF);
		lutG[i] = min(max(0, INT_MULT(i - midpointG, intContrast, t) + midpointG), 0xFF);
		lutB[i] = min(max(0, INT_MULT(i - midpointB, intContrast, t) + midpointB), 0xFF);
	}

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

			r = lutR[r];
			g = lutG[g];
			b = lutB[b];
			//a = lut[a];

			outBuffer[yWidth + x] = 
				(r << 16) |
				(g <<  8) |
				(b      ) |
				(a << 24);
		}
	}

	m_out.SetResult(out);
}