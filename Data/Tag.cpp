#include "Tag.h"

#include <windows.h>

int Tag::s_tags[Tag::s_tableSize] = { 0 };
Tag Tag::Null;

Tag::Tag()
{
	m_id = 0;
}

Tag::Tag(const String& name)
{
	m_name = name;
	m_id = GetId(m_name);
}

Tag::Tag(const Tag& tag)
{
	m_name = tag.m_name;
	m_id = tag.m_id;
}

String& Tag::GetName()
{
	return m_name;
}

void Tag::SetName(const String& name)
{
	m_name = name;
	m_id = GetId(m_name);
}

int Tag::GetId()
{
	return m_id;
}

Tag& Tag::operator = (const Tag& tag)
{
	m_name = tag.m_name;
	m_id = tag.m_id;
	return *this;
}

Tag& Tag::operator = (const String& name)
{
	m_name = name;
	m_id = GetId(m_name);
	return *this;
}

bool Tag::operator == (const Tag& tag)
{
	return m_id == tag.m_id;
}

bool Tag::operator != (const Tag& tag)
{
	return m_id != tag.m_id;
}

int Tag::GetId(const String& _string)
{
	String& string = (String&)_string;

	int length = string.Length;
	uint hash = 0;
	
	// Determine type ID

	for (int i = 0; i < length; i++)
	{
		hash += string[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	int id = hash % s_tableSize;

	/*	Make sure the ID is unique. */

	for (int ipo = id, imo = id; ipo < s_tableSize || imo > 0; )
	{
		if (ipo < s_tableSize 
			&& (!s_tags[ipo] // no id
			|| (signed)s_tags[ipo] == ipo))	// already used
		{
			s_tags[ipo] = ipo;
			return ipo;
		}

		if (imo > 0 
			&& (!s_tags[imo] // no id
			|| (signed)s_tags[imo] == imo)) // already used
		{
			s_tags[imo] = imo;
			return imo;
		}

		ipo++;
		imo--;
	}

	////////////////////////////////////////////////////////////////////////////////

	/*	This should never happen. If it dit, it means
		that the _tag table is too small.*/

	OutputDebugString(
		L"\n"
		L"ERROR: The _tag table is full.\n"
		L"\n");

	DebugBreak();
	return null;
}

///////////////////////////////////////////////////////////////////////////////////////

Tag Tag::Image(L"Image");
Tag Tag::ImagePreview(L"ImagePreview");
Tag Tag::ImageOffset(L"ImageOffset");