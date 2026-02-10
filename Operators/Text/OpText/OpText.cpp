#include "OpText.h"

DEFINE_TYPE(OpText, Operator);
SET_TYPE_DISPLAY_NAME(OpText, L"Text");

OpText::OpText() 
{
	Init();
}

OpText::OpText(const OpText& op) 
{
	Text = op.Text;

	Init();
}

OpText::~OpText()
{

}

void OpText::Init()
{
	m_out.Result = &Text;
	Outputs.Add(&m_out);
}

Object* OpText::Copy()
{
	return OpText(*this);
}

bool OpText::UsesSpec(Type /*specType*/, Type /*objType*/)
{
	return false;
}

bool OpText::ForwardsSpec(Type /*specType*/)
{
	return false;
}

void OpText::Process()
{

}