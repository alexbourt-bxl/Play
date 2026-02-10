#include "OpConvert.h"

#include <Types\Graphics\Image8.h>
#include <Types\Graphics\GdiImage.h>
#include <Types\Graphics\GdiImage8.h>

#include <Operators\Components\Specs\TypeSpec.h>

DEFINE_TYPE(OpConvert, Operator);

OpConvert::OpConvert() 
{
	Init();
}

OpConvert::OpConvert(const OpConvert& op) 
{
	Init();

	OutType = op.OutType;
}

OpConvert::~OpConvert()
{

}

String OpConvert::GetOpName()
{
	return L"Convert";
}

void OpConvert::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.AcceptedTypes.Add(TYPEOF(GdiImage));
	In.AcceptedTypes.Add(TYPEOF(GdiImage8));
	In.Add(&m_in);

	Out.Add(&m_out);
}

Object* OpConvert::Copy()
{
	return OpConvert(*this);
}

bool OpConvert::UsesSpec(Type /*specType*/, Type /*objType*/)
{
	return false;
}

bool OpConvert::ForwardsSpec(Type /*specType*/)
{
	return false;
}

bool OpConvert::_CheckDirty(SpecList& updateSpecs)
{
	Image* out = (Image*)m_out.Result;

	/*	If no output has been generated. */

	if (!out)
		return true;

	/*	If a requested type doesn't match the output size. */

	Type type = TypeSpec::Scan(updateSpecs);

	if (type != typeof(out))
		return true;

	/*	Otherwise the op is clean. */

	return false;
}

void OpConvert::UpdateResult(SpecList& _updateSpecs)
{
	/*	Check spec list for output type. 
		If not available, use own type. */

	SpecList updateSpecs = _updateSpecs;
	Type outType = TypeSpec::ScanAndRemove(updateSpecs);

	if (outType == __Type::Null)
		outType = *OutType;

	/*	OpConvert does not request a type not to affect
		the preceding op chain in any way. */

	Object* in = m_in.GetInputObject(updateSpecs);

	if (!in) 
		return;

	/*	Clear result. */
	m_out.SetResult(null, SpecList::Empty);

	if (!CanConvert(typeof(in), outType))
		return;

	/*	If the input and output types are the same
		simply pass the input along. */

	if (typeof(in) == outType)
		m_out.SetResultTransitive(in, SpecList::Empty);

	/*	Otherwise perform a proper cached conversion. */

	/*	Images */

	else if (typeof(in) == TYPEOF(Image))
		m_out.SetResult(((Image*)in)->ConvertTo(outType), SpecList::Empty);
}

bool OpConvert::CanConvert(Type fromType, Type toType)
{
	/*	Images */

	if (fromType == TYPEOF(Image)
		&& toType == TYPEOF(Image))
		return true;

	return false;
}