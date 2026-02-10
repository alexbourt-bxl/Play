#include "OpBrightness.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpBrightness, Operator);
SET_TYPE_DISPLAY_NAME(OpBrightness, L"Brightness");

OpBrightness::OpBrightness()
{
	Gain   = 1;
	Offset = 0;

	Init();
}

OpBrightness::OpBrightness(const OpBrightness& op) 
{
	Gain   = op.Gain;
	Offset = op.Offset;

	Init();
}

OpBrightness::~OpBrightness()
{
	if (m_outImage)
		delete m_outImage;
}

Object* OpBrightness::Copy()
{
	return new OpBrightness(*this);
}

void OpBrightness::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outImage = null;

	Gain.Name = L"Gain";
	Gain.Valuebox->Relative = true;
	Gain.Valuebox->MinValue = 0;
	Gain.Valuebox->MinVisible = 0;
	Gain.Valuebox->MaxVisible = 2;
	Gain.Valuebox->Granularity = 0.01;
	Gain.Valuebox->DisplayMultiplier = 100;
	Gain.Valuebox->AdjustSpeed = 0.0025;
	Gain.Valuebox->DecimalPlaces = 0;
	Gain.Valuebox->Suffix = L"%";
	Properties.Add(&Gain);

	Offset.Name = L"Pedestal";
	Offset.Valuebox->Relative = true;
	Offset.Valuebox->MinValue = -1;
	Offset.Valuebox->MinVisible = -1;
	Offset.Valuebox->MaxValue = 1;
	Offset.Valuebox->MaxVisible = 1;
	Offset.Valuebox->Granularity = 0.01;
	Offset.Valuebox->DisplayMultiplier = 100;
	Offset.Valuebox->AdjustSpeed = 0.0025;
	Offset.Valuebox->DecimalPlaces = 0;
	Offset.Valuebox->Suffix = L"%";
	Properties.Add(&Offset);
}

void OpBrightness::CopyWorkingValues()
{
	m_gain   = Gain;
	m_offset = Offset;
}

void OpBrightness::Process()
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

	assert(typeof(in) == TYPEOF(Image8));

	/*	Create the output image. */

	Size outSize = in->Size;

	Image* outImage = m_out.Data.Get<Image>(Tag::Image);

	m_outImage = 
		outImage && typeof(outImage) == TYPEOF(Image8) && outImage->Size == outSize
		? (Image8*)outImage
		: new Image8(outSize);

	/*	Change the brightness. */

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)((Image8*)m_outImage)->Buffer;

	int yWidth;

	// Gain

	int intGain   = Math::ToInt8(m_gain);
	int intOffset = Math::ToInt8(m_offset);
	
	int lut[256];
	int _i, t;

	for (int i = 0; i < 256; i++)
	{
		_i = INT_MULT(i, intGain, t);
		lut[i] = min(max(0, _i + intOffset), 0xFF);
	}

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

			r = lut[r];
			g = lut[g];
			b = lut[b];

			outBuffer[yWidth + x] = 
				(r << 16) |
				(g <<  8) |
				(b      ) |
				(a << 24);
		}
	}

	m_out.Data = m_in.Data;
	m_out.Data.Set(Tag::Image, m_outImage);
	m_out.Data.Remove(Tag::ImagePreview);
}