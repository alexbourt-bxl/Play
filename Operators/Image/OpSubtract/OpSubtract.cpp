#include "OpSubtract.h"

DEFINE_TYPE(OpSubtract, Operator);
SET_TYPE_DISPLAY_NAME(OpSubtract, L"Subtract");

OpSubtract::OpSubtract() 
{
	Init();
}

OpSubtract::OpSubtract(const OpSubtract& /*op*/) 
{
	Init();
}

OpSubtract::~OpSubtract()
{

}

void OpSubtract::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_inUnder);
	In.Add(&m_inOver);

	Out.Add(&m_out);
}

Object* OpSubtract::Copy()
{
	return OpSubtract(*this);
}

void OpSubtract::CopyWorkingValues()
{
	//
}

void OpSubtract::Process()
{
	Image* inOver  = (Image*)m_inOver.GetInputObject();
	Image* inUnder = (Image*)m_inUnder.GetInputObject();

	if (!inOver && !inUnder)
	{
		m_out.SetResult(null);
		return; 
	}
	else if (!inOver)
	{
		m_out.SetResultTransitive(inUnder);
		return;
	}
	else if (!inUnder)
	{
		m_out.SetResultTransitive(inOver);
		return;
	}

	assert(typeof(inOver) == TYPEOF(Image8));
	assert(typeof(inUnder) == TYPEOF(Image8));

	/*	Determine the output dimensions.  */

	Size outSize(
		min(inOver->Width, inUnder->Width),
		min(inOver->Height, inUnder->Height));

	/*	Create the output image. */

	Image8* out = 
		m_out.Result && typeof(m_out.Result) == TYPEOF(Image8) && ((Image8*)m_out.Result)->Size == outSize
		? (Image8*)m_out.Result
		: new Image8(outSize);

	Graphics g(out);
	
	g.CopyImage(
		0, 0, 
		inUnder, 
		0, 0, 
		inUnder->Width, inUnder->Height);

	int* inOverBuffer = (int*)((Image8*)inOver)->Buffer;
	int* inUnderBuffer = (int*)((Image8*)inUnder)->Buffer;
	int* outBuffer = (int*)out->Buffer;

	/*	Set up the offset. */

	Point overOffset = inOver->Offset;// - inUnder->Offset;

	int xStart = max(0, overOffset.X);
	int xEnd = min(outSize.Width + overOffset.X, out->Width);

	int yStart = max(0, overOffset.Y);
	int yEnd = min(outSize.Height + overOffset.Y, out->Height);

	/*	Multiply the inputs. */

	int ywidthOut, ywidthInOver, ywidthInUnder;

	for (int y = yStart; y < yEnd; y++)
	{
		ywidthOut = y * out->Width;
		ywidthInUnder = y * inUnder->Width;
		ywidthInOver = (y - overOffset.Y) * inOver->Width;

		for (int x = xStart; x < xEnd; x++)
		{
			outBuffer[ywidthOut + x] = Color8::Subtract(
											inUnderBuffer[ywidthInUnder + x],
											inOverBuffer[ywidthInOver + x - overOffset.X]);
		}
	}

	m_out.SetResult(out);
}