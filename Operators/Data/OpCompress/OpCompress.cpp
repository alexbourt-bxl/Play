#include "OpCompress.h"

#include <Types\Basic\Data.h>
#include <zlib.h>

#include <assert.h>

DEFINE_TYPE(OpCompress, Operator);

OpCompress::OpCompress() 
{
	Init();

	Strength = -1;
}

OpCompress::OpCompress(const OpCompress& op) 
{
	Init();

	Strength = op.Strength;
}

OpCompress::~OpCompress()
{

}

String OpCompress::GetOpName()
{
	return L"Compress";
}

void OpCompress::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Data));
	In.Add(&m_in);

	Out.Add(&m_out);

	Strength.Name = L"Strength";
	Strength.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&Strength);
}

Object* OpCompress::Copy()
{
	return OpCompress(*this);
}

bool OpCompress::UsesSpec(Type /*specType*/, Type /*objType*/)
{
	return false;
}

bool OpCompress::ForwardsSpec(Type /*specType*/)
{
	return false;
}

void OpCompress::UpdateResult(SpecList& _specs)
{
	if (!m_in.Connection_->Out)
		return;

	SpecList specs = _specs;
	Data* data = (Data*)m_in.GetInputObject(specs);

	if (!data)
		return;

	byte* inBuffer = data->Buffer;

	const uint step = 128 * 1024; // 128k

	byte inBuf[step];
	byte outBuf[step];

	/*	Write compression markers. */

	Data* out = new Data(
		  sizeof(Marker_PLAY)
		+ sizeof(Marker_CMPR));

	byte* outBuffer = out->Buffer;

	*((uint*)outBuffer) = Marker_PLAY;	outBuffer += sizeof(Marker_PLAY);
	*((uint*)outBuffer) = Marker_CMPR;	outBuffer += sizeof(Marker_CMPR);

	/*	Initialize compression. */

	z_stream z;
	int result, flush;

	z.zalloc = Z_NULL;
	z.zfree  = Z_NULL;
	z.opaque = Z_NULL;
	
	result = deflateInit(&z, Strength);
	
	assert(result == Z_OK);
	if (result != Z_OK) 
	{
		delete out;
		m_out.SetResult(null, SpecList::Empty);
		return;
	}

	/*	Compress the data. */

	do
	{
		uint inSize = min(step, data->Size - (inBuffer - data->Buffer));
		assert(inSize >= 0);

		memcpy_s(inBuf, step, inBuffer, inSize);
		inBuffer += inSize;

		z.avail_in = inSize;
		z.next_in = inBuf;

		flush = inSize > 0
			? Z_NO_FLUSH
			: Z_FINISH;

		do
		{
			z.avail_out = step;
			z.next_out = outBuf;

			result = deflate(&z, flush);
			assert(result != Z_STREAM_ERROR);  

			int count = step - z.avail_out;
			
			if (count > 0)
			{
				int outLeft = out->Size - (outBuffer - out->Buffer);

				if (outLeft < count)
				{
					out->SetSize(out->Size + count);
					outLeft = out->Size - (outBuffer - out->Buffer);
				}

				memcpy_s(outBuffer, outLeft, outBuf, count);
				outBuffer += count;
			}
		} 
		while (z.avail_out == 0);
		assert(z.avail_in == 0);
	}
	while (flush != Z_FINISH);
	assert(result == Z_STREAM_END);

	deflateEnd(&z);

	m_out.SetResult(out, SpecList::Empty);
}