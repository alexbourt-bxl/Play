#include "OpNoise.h"

#include <Native\Gdi.h>
#include <System\System.h>
#include <Math\Math.h>
#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpNoise, Operator);
SET_TYPE_DISPLAY_NAME(OpNoise, L"Noise");

OpNoise::OpNoise() 
{
	Seed = 0;

	Init();
}

OpNoise::OpNoise(const OpNoise& op) 
{
	Seed = op.Seed;

	Init();
}

OpNoise::~OpNoise()
{
	if (m_outImage)
		delete m_outImage;
}

Object* OpNoise::Copy()
{
	return new OpNoise(*this);
}

void OpNoise::Init()
{
	/*	TODO: properties should have min and max values, just like 
		valueboxes. Otherwise later on I can't use them with scripts. */

	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outImage = null;

	Seed.Name = L"Seed";
	Seed.Valuebox->MinValue = 0;
	Seed.Valuebox->MinVisible = 0;
	Properties.Add(&Seed);

	m_initSeed = 0x8CA2;
}

void OpNoise::LayoutControls(LineLayout* layout)
{
	layout->Controls.Add(Seed.Control);
}

void OpNoise::CopyWorkingValues()
{
	m_seed = Seed;
}

void OpNoise::Process()
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

	/*	Paint the noise. */

	/*	Using Robert Jenkins' 32-bit int hashing algorithm to generate 
		predictable pseudo-random values that work with clip regions. */

	#define HASH(x)	\
		(x) = ((x) + 0x7ed55d16) + ((x) << 12); \
		(x) = ((x) ^ 0xc761c23c) ^ ((x) >> 19); \
		(x) = ((x) + 0x165667b1) + ((x) << 5);  \
		(x) = ((x) + 0xd3a2646c) ^ ((x) << 9);  \
		(x) = ((x) + 0xfd7046c5) + ((x) << 3);  \
		(x) = ((x) ^ 0xb55a4f09) ^ ((x) >> 16);

	int seed = m_seed;
	HASH(seed);

	Image8* out8 = (Image8*)m_outImage;

	int* buffer = (int*)out8->Buffer;
	int yVal, val;

	for (int y = 0; y < outSize.Height; y++)
	{
		yVal = y + seed;
		HASH(yVal);

		int yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
		{
			val = yVal + x;
			HASH(val);

			buffer[yWidth + x] = val | 0xFF000000;
		}

		UpdateProgress((double)y / (double)outSize.Width);
	}

	#undef HASH

	m_out.Data.Set(Tag::Image, m_outImage);
}