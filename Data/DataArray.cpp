#include "DataArray.h"

Object* DataArray::operator [] (const Tag& tag)
{
	for (int i = 0; i < Count; i++)
	{
		if ((*this)[i].Tag == tag)
			return (*this)[i].Object;
	}

	return null;
}

void DataArray::Set(const Tag& tag, Object* obj)
{
	Data data(obj, tag);

	int index = GetIndexOf(data);

	if (index > -1)
		RemoveAt(index);

	Add(data);
}

void DataArray::Remove(const Tag& tag)
{
	for (int i = Count - 1; i >= 0; i--)
	{
		if ((*this)[i].Tag == tag)
			RemoveAt(i);
	}
}