#include "OpChromaRange.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpChromaRange, Operator);
SET_TYPE_DISPLAY_NAME(OpChromaRange, L"Chroma Range");

OpChromaRange::OpChromaRange()
{
	Color           = Color::Green;
	ChromaSpread    = 0.3;
	ChromaTolerance = 0.1;
	LumaSpread		= 0.3;

	Init();
}

OpChromaRange::OpChromaRange(const OpChromaRange& op) 
{
	Color           = op.Color;
	ChromaSpread    = op.ChromaSpread;
	ChromaTolerance = op.ChromaTolerance;
	LumaSpread      = op.LumaSpread;

	Init();
}

OpChromaRange::~OpChromaRange()
{

}

void OpChromaRange::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);

	Color.Name = L"Color";
	Properties.Add(&Color);

	ChromaSpread.Name = L"Chroma Spread";
	ChromaSpread.Valuebox->Relative = true;
	ChromaSpread.Valuebox->MinValue = 0;
	ChromaSpread.Valuebox->MinVisible = 0;
	ChromaSpread.Valuebox->MaxValue = 1;
	ChromaSpread.Valuebox->MaxVisible = 1;
	ChromaSpread.Valuebox->Granularity = 0.01;
	ChromaSpread.Valuebox->DisplayMultiplier = 100;
	ChromaSpread.Valuebox->AdjustSpeed = 0.0025;
	ChromaSpread.Valuebox->DecimalPlaces = 0;
	ChromaSpread.Valuebox->Suffix = L"%";
	Properties.Add(&ChromaSpread);

	ChromaTolerance.Name = L"Chroma Tolerance";
	ChromaTolerance.Valuebox->Relative = true;
	ChromaTolerance.Valuebox->MinValue = 0;
	ChromaTolerance.Valuebox->MinVisible = 0;
	ChromaTolerance.Valuebox->MaxValue = 1;
	ChromaTolerance.Valuebox->MaxVisible = 1;
	ChromaTolerance.Valuebox->Granularity = 0.01;
	ChromaTolerance.Valuebox->DisplayMultiplier = 100;
	ChromaTolerance.Valuebox->AdjustSpeed = 0.0025;
	ChromaTolerance.Valuebox->DecimalPlaces = 0;
	ChromaTolerance.Valuebox->Suffix = L"%";
	Properties.Add(&ChromaTolerance);

	LumaSpread.Name = L"Luma Spread";
	LumaSpread.Valuebox->Relative = true;
	LumaSpread.Valuebox->MinValue = 0;
	LumaSpread.Valuebox->MinVisible = 0;
	LumaSpread.Valuebox->MaxValue = 1;
	LumaSpread.Valuebox->MaxVisible = 1;
	LumaSpread.Valuebox->Granularity = 0.01;
	LumaSpread.Valuebox->DisplayMultiplier = 100;
	LumaSpread.Valuebox->AdjustSpeed = 0.0025;
	LumaSpread.Valuebox->DecimalPlaces = 0;
	LumaSpread.Valuebox->Suffix = L"%";
	Properties.Add(&LumaSpread);
}

Object* OpChromaRange::Copy()
{
	return OpChromaRange(*this);
}

void OpChromaRange::CopyWorkingValues()
{
	m_color = Color;
	m_chromaSpread = ChromaSpread;
	m_chromaTolerance = ChromaTolerance;
	m_lumaSpread = LumaSpread;
}

void OpChromaRange::Process()
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

	struct YCBCR
	{
		double Y;
		double Cb;
		double Cr;
	} ref;

	ref.Y  = 0.2126 * m_color.R + 0.7152 * m_color.G + 0.0722 * m_color.B;
	ref.Cb = 0.5389 * (m_color.B - ref.Y);
	ref.Cr = 0.6350 * (m_color.R - ref.Y);
			   
	int c, r, g, b, a;
	double d, fy, fa;
	double Y, Cb, Cr;

	Color8 color8 = m_color;

	// Isolate the specified color

	double chromaSpread = m_chromaSpread / 2;
	double lumaSpread = m_lumaSpread / 2;

	double lowerY, upperY;

	for (int y = 0; y < outSize.Height; y++)
	{
		yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
		{
			c = inBuffer[yWidth + x];

			r = (c & 0x00FF0000) >> 16;
			g = (c & 0x0000FF00) >>  8;
			b = (c & 0x000000FF);

			Y  = 0.2126 * (double)r / 255 + 0.7152 * (double)g / 255 + 0.0722 * (double)b / 255;
			Cb = 0.5389 * ((double)b / 255 - Y);
			Cr = 0.6350 * ((double)r / 255 - Y);

			d = Math::GetDistance(FPoint(Cb, Cr), FPoint(ref.Cb, ref.Cr));

			if (d < chromaSpread)
				fa = 1;
			else if (d < chromaSpread + m_chromaTolerance && m_chromaTolerance > 0)
				fa = (chromaSpread + m_chromaTolerance - d) / m_chromaTolerance;
			else
				fa = 0;

			lowerY = ref.Y - lumaSpread;
			upperY = ref.Y + lumaSpread;

			if (Y < lowerY)
				fy = Y / lowerY;
			else if (Y > upperY)
				fy = (1 - Y) / (1 - upperY);
			else
				fy = 1;

			fa *= fy;

			a = Math::ToInt8(fa);

			r = a;
			g = a;
			b = a;

			outBuffer[yWidth + x] = 
				(r << 16) |
				(g <<  8) |
				(b      ) |
				(a << 24);
		}
	}

	m_out.SetResult(out);
}