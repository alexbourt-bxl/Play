#ifndef DATA_ARRAY_H
#define DATA_ARRAY_H

#include <Containers/Array.h>
#include "Data.h"

class DataArray
	: public Array<Data>
{
public:
	Object* operator [] (const Tag& tag);

	template <class Class>
	Class* Get(const Tag& tag)
	{
		Object* obj = (*this)[tag];
		
		if (obj)
		{
			assert(typeof(obj) == TYPEOF(Class));
			return (Class*)obj;
		}

		return null;
	}

	void Set(const Tag& tag, Object* obj);
	void Remove(const Tag& tag);
};

#endif