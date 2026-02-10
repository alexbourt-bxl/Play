#include "OpUnpremultiply.h"

DEFINE_TYPE(OpUnpremultiply, Operator);
SET_TYPE_DISPLAY_NAME(OpUnpremultiply, L"Unpremultiply");

OpUnpremultiply::OpUnpremultiply() 
{
	Init();
}

OpUnpremultiply::OpUnpremultiply(const OpUnpremultiply& /*op*/) 
{
	Init();
}

OpUnpremultiply::~OpUnpremultiply()
{

}

Object* OpUnpremultiply::Copy()
{
	return OpUnpremultiply(*this);
}

void OpUnpremultiply::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
}

void OpUnpremultiply::CopyWorkingValues()
{
	//
}

void OpUnpremultiply::Process()
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

	/*	Unpremultiply the image. */

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)out->Buffer;

	int yWidth;

	for (int y = 0; y < outSize.Height; y++)
	{
		yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
			outBuffer[yWidth + x] = Color8::Unpremult(inBuffer[yWidth + x]);
	}

	m_out.SetResult(out);
}