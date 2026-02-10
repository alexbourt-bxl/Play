#include "OpOver.h"

DEFINE_TYPE(OpOver, Operator);
SET_TYPE_DISPLAY_NAME(OpOver, L"Over");

OpOver::OpOver() 
{
	Init();
}

OpOver::OpOver(const OpOver& ) 
{
	Init();
}

OpOver::~OpOver()
{
	if (m_outImage)
		delete m_outImage;
}

void OpOver::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_inUnder);
	In.Add(&m_inOver);

	Out.Add(&m_out);
	m_outImage = null;
}

Object* OpOver::Copy()
{
	return new OpOver(*this);
}

void OpOver::CopyWorkingValues()
{
	//
}

void OpOver::Process()
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

	/*	TODO: a consistent policy of two-input nodes has to be
		decided as to which size to use if the sizes are different. 
		For now the under size is used. */

	/*	Create the output image. */

	Image* outImage = m_out.Data.Get<Image>(Tag::Image);

	m_outImage = 
		outImage && typeof(outImage) == TYPEOF(Image8) && outImage->Size == outSize
		? (Image8*)outImage
		: new Image8(outSize);

	Image8* out8 = (Image8*)m_outImage;

	Graphics g(out8);
	
	g.CopyImage(
		0, 0, 
		inUnder, 
		0, 0, 
		inUnder->Width, inUnder->Height);

	Point overOffset(0, 0);
	
	if (inOverOffset)
		overOffset += *inOverOffset;// - inUnder->Offset; // TODO: what to do with the under offset?

	g.DrawImage(
		overOffset.X, overOffset.Y,
		inOver);

	//int* inOverBuffer = (int*)((Image8*)inOver)->Buffer;
	//int* inUnderBuffer = (int*)((Image8*)inUnder)->Buffer;
	//int* outBuffer = (int*)out->Buffer;

	///*	Set up the offset. */

	//Point overOffset = inOver->Offset - inUnder->Offset;

	//int xStart = max(0, overOffset.X);
	//int xEnd = min(outSize.Width + overOffset.X, out->Width);

	//int yStart = max(0, overOffset.Y);
	//int yEnd = min(outSize.Height + overOffset.Y, out->Height);

	///*	Composite the inputs. */

	//int ywidthOut, ywidthInOver, ywidthInUnder;

	//for (int y = yStart; y < yEnd; y++)
	//{
	//	ywidthOut = y * out->Width;
	//	ywidthInUnder = y * inUnder->Width;
	//	ywidthInOver = (y - overOffset.Y) * inOver->Width;

	//	for (int x = xStart; x < xEnd; x++)
	//	{
	//		outBuffer[ywidthOut + x] = Color8::Over(
	//										inUnderBuffer[ywidthInUnder + x],
	//										inOverBuffer[ywidthInOver + x - overOffset.X]);
	//	}
	//}

	m_out.Data.Set(Tag::Image, m_outImage);
}