#include "OpDecompress.h"

#include <Types\Basic\Data.h>

#include <assert.h>

DEFINE_TYPE(OpDecompress, Operator);

OpDecompress::OpDecompress() 
{
	Init();
}

OpDecompress::OpDecompress(const OpDecompress& /*op*/) 
{
	Init();
}

OpDecompress::~OpDecompress()
{

}

String OpDecompress::GetOpName()
{
	return L"Decompress";
}

void OpDecompress::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Data));
	In.Add(&m_in);

	Out.Add(&m_out);
}

Object* OpDecompress::Copy()
{
	return OpDecompress(*this);
}

bool OpDecompress::UsesSpec(Type /*specType*/, Type /*objType*/)
{
	return false;
}

bool OpDecompress::ForwardsSpec(Type /*specType*/)
{
	return false;
}

void OpDecompress::UpdateResult(SpecList& _specs)
{
	if (!m_in.Connection_->Out)
		return;

	SpecList specs = _specs;
	Data* data = (Data*)m_in.GetInputObject(specs);

	if (!data)
		return;

	/*	Decompress the data. */

	//compress and decompress with zlib
	//	and
	//derive GdiImage8 from GdiImage 
	//and update all ops appropriately
}