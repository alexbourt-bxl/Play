#include "OpPremultiply.h"

DEFINE_TYPE(OpPremultiply, Operator);
SET_TYPE_DISPLAY_NAME(OpPremultiply, L"Premultiply");

OpPremultiply::OpPremultiply() 
{
	Init();
}

OpPremultiply::OpPremultiply(const OpPremultiply& /*op*/) 
{
	Init();
}

OpPremultiply::~OpPremultiply()
{

}

Object* OpPremultiply::Copy()
{
	return OpPremultiply(*this);
}

void OpPremultiply::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
}

void OpPremultiply::CopyWorkingValues()
{
	//
}

void OpPremultiply::Process()
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

	/*	Premultiply the image. */

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)out->Buffer;

	int yWidth;

	for (int y = 0; y < outSize.Height; y++)
	{
		yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
			outBuffer[yWidth + x] = Color8::Premult(inBuffer[yWidth + x]);
	}

	m_out.SetResult(out);
}