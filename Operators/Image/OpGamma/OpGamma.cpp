#include "OpGamma.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpGamma, Operator);
SET_TYPE_DISPLAY_NAME(OpGamma, L"Gamma");

OpGamma::OpGamma()
{
	Gamma = 1;

	Init();
}

OpGamma::OpGamma(const OpGamma& op) 
{
	Gamma = op.Gamma;

	Init();
}

OpGamma::~OpGamma()
{

}

Object* OpGamma::Copy()
{
	return OpGamma(*this);
}

void OpGamma::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);

	Gamma.Name = L"Gamma";
	Gamma.Valuebox->Relative = true;
	Gamma.Valuebox->MinValue = 0.01;
	Gamma.Valuebox->MinVisible = 0;
	Gamma.Valuebox->MaxVisible = 4;
	Gamma.Valuebox->Granularity = 0.01;
	Gamma.Valuebox->AdjustSpeed = 0.0025;
	//Gain.Valuebox->DecimalPlaces = -1;
	Properties.Add(&Gamma);

	Invert.Button->Text = L"Invert";
	Invert.Button->Width = 70;
	Properties.Add(&Invert);
}

void OpGamma::CopyWorkingValues()
{
	m_gamma = Gamma;
	m_invert = Invert;
}

void OpGamma::Process()
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

	/*	Change the gamma. */

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)out->Buffer;

	int yWidth;

	int lut[256];

	double _1_gamma = 
		m_invert
		? m_gamma
		: 1 / m_gamma;

	for (int i = 0; i < 256; i++)
		lut[i] = (int)(pow((double)i / 255, _1_gamma) * 255);

	int c, r, g, b, a;

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

			outBuffer[yWidth + x] =
				(lut[r] << 16) |
				(lut[g] <<  8) |
				(lut[b]      ) |
				(    a  << 24);
		}
	}

	m_out.SetResult(out);
}