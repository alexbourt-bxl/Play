#include "OpSaturation.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpSaturation, Operator);
SET_TYPE_DISPLAY_NAME(OpSaturation, L"Saturation");

OpSaturation::OpSaturation()
{
	Saturation = 1;

	Init();
}

OpSaturation::OpSaturation(const OpSaturation& op) 
{
	Saturation = op.Saturation;

	Init();
}

OpSaturation::~OpSaturation()
{

}

void OpSaturation::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);

	Saturation.Name = L"Saturation";
	Saturation.Valuebox->Relative = true;
	Saturation.Valuebox->MinValue = 0;
	Saturation.Valuebox->MinVisible = 0;
	Saturation.Valuebox->MaxVisible = 2;
	Saturation.Valuebox->Granularity = 0.01;
	Saturation.Valuebox->DisplayMultiplier = 100;
	Saturation.Valuebox->AdjustSpeed = 0.0025;
	Saturation.Valuebox->DecimalPlaces = 0;
	Saturation.Valuebox->Suffix = L"%";
	Properties.Add(&Saturation);
}

Object* OpSaturation::Copy()
{
	return OpSaturation(*this);
}

void OpSaturation::CopyWorkingValues()
{
	m_saturation = Saturation;
}

void OpSaturation::Process()
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

	/*	Change the saturation. */

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)out->Buffer;

	int yWidth;

	int intSaturation = Math::ToInt8(m_saturation);

	int c, v, r, g, b, a, t;

	for (int y = 0; y < outSize.Height; y++)
	{
		yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
		{
			c = inBuffer[yWidth + x];

			v = Color8::GetLuminance(c);

			r = (c & 0x00FF0000) >> 16;
			g = (c & 0x0000FF00) >>  8;
			b = (c & 0x000000FF);
			a = (c & 0xFF000000) >> 24;

			r = INT_LERP_MM(v, r, intSaturation, t);
			g = INT_LERP_MM(v, g, intSaturation, t);
			b = INT_LERP_MM(v, b, intSaturation, t);

			outBuffer[yWidth + x] = 
				(r << 16) |
				(g <<  8) |
				(b      ) |
				(a << 24);
		}
	}

	m_out.SetResult(out);
}