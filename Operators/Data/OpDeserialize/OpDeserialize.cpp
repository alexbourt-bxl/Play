#include "OpDeserialize.h"

#include <Types\Basic\Data.h>
#include <Serialization\Serializable.h>

#include <assert.h>

DEFINE_TYPE(OpDeserialize, Operator);

OpDeserialize::OpDeserialize() 
{
	Init();
}

OpDeserialize::OpDeserialize(const OpDeserialize& /*op*/) 
{
	Init();
}

OpDeserialize::~OpDeserialize()
{

}

String OpDeserialize::GetOpName()
{
	return L"Deserialize";
}


void OpDeserialize::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Data));
	In.Add(&m_in);

	Out.Add(&m_out);
}

Object* OpDeserialize::Copy()
{
	return OpDeserialize(*this);
}

bool OpDeserialize::UsesSpec(Type /*specType*/, Type /*objType*/)
{
	return false;
}

bool OpDeserialize::ForwardsSpec(Type /*specType*/)
{
	return false;
}

void OpDeserialize::UpdateResult(SpecList& _specs)
{
	if (!m_in.Connection_->Out)
		return;

	SpecList specs = _specs;
	Data* serialized = (Data*)m_in.GetInputObject(specs);

	if (serialized->Size < 10)
		return;

	uint pos = 0;

	// Check that this is serialized Play data

	uint marker_PLAY = *((uint*)(serialized->Buffer + pos));	pos += sizeof(Marker_PLAY);

	if (marker_PLAY != Marker_PLAY)
		return;

	uint marker_SERI = *((uint*)(serialized->Buffer + pos));	pos += sizeof(Marker_SERI);

	if (marker_SERI != Marker_SERI)
		return;

	// Determine the type of serialized object

	ushort typeId = *((ushort*)(serialized->Buffer + pos));		pos += sizeof(ushort);

	m_out.SetResult(Object::Create(__Type::GetFromId(typeId)), SpecList::Empty);
	
	if (m_out.Result)
	{
		Serializable* s = dynamic_cast<Serializable*>(m_out.Result);
		s->Deserialize(serialized);
	}
}