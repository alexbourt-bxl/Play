#include "OpSerialize.h"

#include <Types\Basic\Data.h>
#include <Serialization\Serializable.h>

#include <assert.h>

DEFINE_TYPE(OpSerialize, Operator);

OpSerialize::OpSerialize() 
{
	Init();
}

OpSerialize::OpSerialize(const OpSerialize& /*op*/) 
{
	Init();
}

OpSerialize::~OpSerialize()
{

}

String OpSerialize::GetOpName()
{
	return L"Serialize";
}

void OpSerialize::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Serializable));
	In.Add(&m_in);

	Out.Add(&m_out);
}

Object* OpSerialize::Copy()
{
	return OpSerialize(*this);
}

bool OpSerialize::UsesSpec(Type /*specType*/, Type /*objType*/)
{
	return false;
}

bool OpSerialize::ForwardsSpec(Type /*specType*/)
{
	return false;
}

void OpSerialize::UpdateResult(SpecList& _specs)
{
	if (!m_in.Connection_->Out)
		return;

	SpecList specs = _specs;
	Object* obj = m_in.GetInputObject(specs);

	Serializable* s = dynamic_cast<Serializable*>(obj);

	m_out.SetResult(s->Serialize(), SpecList::Empty);
}