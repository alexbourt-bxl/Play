#ifndef DATA_H
#define DATA_H

#include <Types/Reference.h>
#include <Data/Tag.h>

class Data
{
	typedef Tag _Tag;
	typedef Object _Object;

public:
	Data();
	Data(Object* obj, const _Tag& tag);
	Data(const Data& data);
	~Data();

	__declspec(property(get = GetObject_, put = SetObject_))
	_Object* Object;
	_Object* GetObject_();
	void SetObject_(_Object* obj);

	__declspec(property(get = GetTag, put = SetTag))
	_Tag& Tag;
	_Tag& GetTag();
	void  SetTag(const _Tag& tag);

	operator bool();

	bool operator == (const Data& data);
	bool operator != (const Data& data);

private:
	_Object* m_object;
	_Tag m_tag;

public:
	static Data Null;
};

#endif