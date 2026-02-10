#include "OpNull.h"

#include <assert.h>

DEFINE_TYPE(OpNull, Operator);
SET_TYPE_DISPLAY_NAME(OpNull, L"Null");

OpNull::OpNull() 
{
	Init();
}

OpNull::OpNull(const OpNull& /*op*/) 
{
	Init();
}

OpNull::~OpNull()
{

}

void OpNull::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Object));
	In.Add(&m_in);

	Out.Add(&m_out);
}

Object* OpNull::Copy()
{
	return new OpNull(*this);
}

void OpNull::CopyWorkingValues()
{
	//
}

void OpNull::Process()
{
	/*	In operators that simply pass data along,
		certain things need to be filtered out. */

	m_out.Data = m_in.Data;
	m_out.Data.Remove(Tag::ImagePreview);
}