#ifndef REFERENCE_H
#define REFERENCE_H

/*	A Reference in this sense is basically a shared pointer to an Object. 
	
	Each object keeps its own reference count.
	
	(I don't mind the semantic confusion between these References and
	standard C++ & references). */

#include <Types/Object.h>
#include <assert.h>

class Reference 
{
	typedef Object _Object;

public:
	Reference() 
		: m_object(null)
	{
		//
	}

	Reference(_Object* obj)
		: m_object(obj)
	{
		m_object->m_refCount++;
	}

	Reference(const Reference& r)
		: m_object(r.m_object)
	{
		m_object->m_refCount++;
	}

	~Reference()
	{
		Release();
	}

	void Release()
	{
		if (m_object && --m_object->m_refCount == 0)
		{
			delete m_object; 
			m_object = null;
		}
	}

	__declspec(property(get = GetObject_))
	_Object* Object;
	_Object* GetObject_() { return m_object; }

	operator _Object* () { return m_object; }

	_Object& operator *  () { return *m_object; }
	_Object* operator -> () { return m_object; }

	bool operator == (const Reference& r)
	{
		return m_object == r.m_object;
	}

	bool operator != (const Reference& r)
	{
		return m_object != r.m_object;
	}

	Reference& operator = (_Object* obj)
	{
		if (m_object == obj) return *this;

		Release();

		m_object = obj;
		m_object->m_refCount++;

		return *this;
	}

	Reference& operator = (const Reference& r)
	{
		if (this == &r)	return *this;
		if (m_object == r.m_object) return *this;

		Release();

		m_object = r.m_object;
		m_object->m_refCount++;

		return *this;
	}

	/*	Setting a reference to null will release it. */
	Reference& operator = (int _null) 
	{
		assert(_null == 0); // The only numeric value that can be assigned to the reference
		Release();			// is null for the purpose of releasingt it.

		return *this;
	}

protected:
	_Object* m_object;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

/*	Typed reference. */

//template <typename Class>
//class TypedReference
//	: public Reference
//{
//public:
//	operator Class* () { return (Class*)m_object; }
//
//	Class& operator *  () { return (Class&)*m_object; }
//	Class* operator -> () { return (Class*)m_object; }
//
//	Reference& operator = (Class* obj)
//	{
//		if (m_object == obj) return *this;
//		assert(typeof(obj) == TYPEOF(Class));
//
//		Release();
//
//		m_object = obj;
//		m_object->m_refCount++;
//
//		return *this;
//	}
//
//	Reference& operator = (const Reference& r)
//	{
//		if (this == &r)	return *this;
//
//		assert(typeof(r.m_object) == TYPEOF(Class));
//		if (m_object == r.m_object) return *this;
//
//		Release();
//
//		m_object = r.m_object;
//		m_object->m_refCount++;
//
//		return *this;
//	}
//};

////////////////////////////////////////////////////////////////////////////////////////////////////

typedef Reference ref;
//#define tref TypedReference;

#endif