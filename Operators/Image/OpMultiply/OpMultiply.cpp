#include "OpMultiply.h"

DEFINE_TYPE(OpMultiply, Operator);
SET_TYPE_DISPLAY_NAME(OpMultiply, L"Multiply");

OpMultiply::OpMultiply() 
{
	Init();
}

OpMultiply::OpMultiply(const OpMultiply& op) 
{
	Init();
}

OpMultiply::~OpMultiply()
{
	if (m_outImage)
		delete m_outImage;
}

void OpMultiply::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_inUnder);
	In.Add(&m_inOver);

	Out.Add(&m_out);
	m_outImage = null;
}

Object* OpMultiply::Copy()
{
	return new OpMultiply(*this);
}

void OpMultiply::CopyWorkingValues()
{
	//
}

void OpMultiply::Process()
{
	Image* inOver = m_inOver.Data.Get<Image>(Tag::Image);
	Point* inOverOffset = m_inOver.Data.Get<Point>(Tag::ImageOffset);

	Image* inUnder = m_inUnder.Data.Get<Image>(Tag::Image);

	if (!inOver && !inUnder)
	{
		if (m_outImage)
		{
			delete m_outImage;
			m_outImage = null;
		}

		m_out.Data.Clear();
		return; 
	}
	else if (!inOver)
	{
		if (m_outImage)
		{
			delete m_outImage;
			m_outImage = null;
		}

		m_out.Data.Set(Tag::Image, inUnder);
		return;
	}
	else if (!inUnder)
	{
		if (m_outImage)
		{
			delete m_outImage;
			m_outImage = null;
		}

		m_out.Data.Set(Tag::Image, inOver);
		return;
	}

	assert(typeof(inOver) == TYPEOF(Image8));
	assert(typeof(inUnder) == TYPEOF(Image8));

	/*	Determine the output dimensions.  */

	//Size outSize(
	//	min(inOver->Width, inUnder->Width),
	//	min(inOver->Height, inUnder->Height));

	Size outSize = inUnder->Size;

	/*	Create the output image. */

	Image* outImage = m_out.Data.Get<Image>(Tag::Image);

	m_outImage = 
		outImage && typeof(outImage) == TYPEOF(Image8) && outImage->Size == outSize
		? (Image8*)outImage
		: new Image8(outSize);

	Image8* out8 = (Image8*)m_outImage;

	Graphics g(out8);
	
	g.ClearRect(
		0, 0, 
		out8->Width, out8->Height);

	int* inOverBuffer = (int*)((Image8*)inOver)->Buffer;
	int* inUnderBuffer = (int*)((Image8*)inUnder)->Buffer;
	int* outBuffer = (int*)out8->Buffer;

	/*	Set up the offset. */

	Point overOffset(0, 0);
	
	if (inOverOffset)
		overOffset += *inOverOffset;// - inUnder->Offset; // TODO: what to do with the under offset?

	int xStart = max(0, overOffset.X);
	int xEnd = min(inOver->Width + overOffset.X, out8->Width);

	int yStart = max(0, overOffset.Y);
	int yEnd = min(inOver->Height + overOffset.Y, out8->Height);

	/*	Multiply the inputs. */

	int ywidthOut, ywidthInOver, ywidthInUnder;

	for (int y = yStart; y < yEnd; y++)
	{
		ywidthOut = y * out8->Width;
		ywidthInUnder = y * inUnder->Width;
		ywidthInOver = (y - overOffset.Y) * inOver->Width;

		for (int x = xStart; x < xEnd; x++)
		{
			outBuffer[ywidthOut + x] = Color8::Multiply(
											inUnderBuffer[ywidthInUnder + x],
											inOverBuffer[ywidthInOver + x - overOffset.X]);
		}
	}

	m_out.Data.Set(Tag::Image, m_outImage);
}