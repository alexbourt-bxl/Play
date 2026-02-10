#include "OpChannels.h"

#include <Controls\Valuebox\Valuebox.h>
#include <Controls\Layout\LineLayout\LineLayout.h>

DEFINE_TYPE(OpChannels, Operator);
SET_TYPE_DISPLAY_NAME(OpChannels, L"Channels");

OpChannels::OpChannels()
{
	Init();
}

OpChannels::OpChannels(const OpChannels& op) 
{
	Init();
}

OpChannels::~OpChannels()
{
	if (m_outImage)
		delete m_outImage;
}

Object* OpChannels::Copy()
{
	return new OpChannels(*this);
}

void OpChannels::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outImage = null;

	m_control.Channels.Add(RewireControl::Channel(1, L"R", Color(1, 0.337, 0.286)));
	m_control.Channels.Add(RewireControl::Channel(2, L"G", Color(0.294, 1, 0)));
	m_control.Channels.Add(RewireControl::Channel(3, L"B", Color(0.157, 0.573, 1)));
	m_control.Channels.Add(RewireControl::Channel(4, L"A", Color::White));

	m_control.Channels[0] = 1;
	m_control.Channels[1] = 2;
	m_control.Channels[2] = 3;
	m_control.Channels[3] = 4;

	m_control.ChangeEvent += EventHandler(this, &OpChannels::control_OnChange);
}

void OpChannels::LayoutControls(LineLayout* layout)
{
	layout->Controls.Add(&m_control);
}

void OpChannels::CopyWorkingValues()
{
	m_r = m_control.Channels[0];
	m_g = m_control.Channels[1];
	m_b = m_control.Channels[2];
	m_a = m_control.Channels[3];
}

void OpChannels::Process()
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

	Image8* out8 = (Image8*)m_outImage;

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)out8->Buffer;

	int yWidth;

	int colorMaskR = 0x00FF0000, shiftBitsR = 16;
	int colorMaskG = 0x0000FF00, shiftBitsG =  8;
	int colorMaskB = 0x000000FF, shiftBitsB =  0;
	int colorMaskA = 0xFF000000, shiftBitsA = 24;

	int maskR, shiftR;
	int maskG, shiftG;
	int maskB, shiftB;
	int maskA, shiftA;

	switch (m_r)
	{
	default:
	case 0: maskR = 0x00000000; shiftR = 0x00000000; break;
	case 1:	maskR = colorMaskR; shiftR = shiftBitsR; break;
	case 2:	maskR = colorMaskG; shiftR = shiftBitsG; break;
	case 3:	maskR = colorMaskB; shiftR = shiftBitsB; break;
	case 4:	maskR = colorMaskA; shiftR = shiftBitsA; break;
	}

	switch (m_g)
	{
	case 0: maskG = 0x00000000; shiftG = 0x00000000; break;
	case 1:	maskG = colorMaskR; shiftG = shiftBitsR; break;
	default:
	case 2:	maskG = colorMaskG; shiftG = shiftBitsG; break;
	case 3:	maskG = colorMaskB; shiftG = shiftBitsB; break;
	case 4:	maskG = colorMaskA; shiftG = shiftBitsA; break;
	}

	switch (m_b)
	{
	case 0: maskB = 0x00000000; shiftB = 0x00000000; break;
	case 1:	maskB = colorMaskR; shiftB = shiftBitsR; break;
	case 2:	maskB = colorMaskG; shiftB = shiftBitsG; break;
	default:
	case 3:	maskB = colorMaskB; shiftB = shiftBitsB; break;
	case 4:	maskB = colorMaskA; shiftB = shiftBitsA; break;
	}

	switch (m_a)
	{
	case 0: maskA = 0x00000000; shiftA = 0x00000000; break;
	case 1:	maskA = colorMaskR; shiftA = shiftBitsR; break;
	case 2:	maskA = colorMaskG; shiftA = shiftBitsG; break;
	case 3:	maskA = colorMaskB; shiftA = shiftBitsB; break;
	default:
	case 4:	maskA = colorMaskA; shiftA = shiftBitsA; break;
	}

	// Rewire channels

	uint c, r, g, b, a;

	for (int y = 0; y < outSize.Height; y++)
	{
		yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
		{
			c = inBuffer[yWidth + x];

			r = (c & maskR) >> shiftR;
			g = (c & maskG) >> shiftG;
			b = (c & maskB) >> shiftB;
			a = (c & maskA) >> shiftA;

			outBuffer[yWidth + x] = 
				(r << 16) |
				(g <<  8) |
				(b      ) |
				(a << 24);
		}
	}

	m_out.Data.Set(Tag::Image, m_outImage);
}