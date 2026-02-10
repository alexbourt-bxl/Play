#ifndef OBJECT_H
#define OBJECT_H

#include <Types/Type.h>

/*	Only a complete type that derives from Object can be created through Object::Create(). 
	Also, for this method to work, all Types MUST implement a default constructor. */

//#include "ObjectPool.h"

//class String;
template <class Class> class Array;

class Object
{
	//friend class ObjectPool;
	friend class Reference;

	////////////////////////////////////////////////////

	/*	NOTE: This is done manually here so that the Object type 
		remains abstract (GetType() is not implemented). */

private:
	static __Type* s_type;

public:
	static __Type* GetClassType();
	static Object* Create(Type type);

	////////////////////////////////////////////////////

public:
	Object();
	virtual ~Object();

	virtual Object* Copy() = 0;

	/*
	static Type GetClassType(); // Each class that derives from type must implement this.
	*/

	virtual Type GetType() const = 0;

	/*	This casting operator is necessary to be able 
		to use typeof() with objects and references. */
	//inline operator Object*() { return this; }

	inline uint GetId() { return m_id; }

	//__declspec(property(get = GetOwner, put = SetOwner))
	//void*	Owner;
	//void*	GetOwner();
	//void	SetOwner(void* owner);

	//__declspec(property(get = GetName, put = SetName))
	//String& Name;
	//String& GetName();
	//void	SetName(const String& name);

	//void	MarkForDelete();

	//static  Object* GetById(uint objId);

	//static  ObjectPool& GetPool();

private:
	uint m_id; // a unique global object ID
	static uint s_lastIssuedId;

	uint m_refCount;

	//void*	m_owner;

	//String*	m_name;

//////////////////////////////////////////////////

//protected:
	//static int init_dummy__;
};

//////////////////////////////////////////////////////////////////////////////////

// Returns the ID of an object
//inline uint idof(Object* object)
//{
//	return object->GetId();
//}

// Returns the type of an object
inline Type typeof(Object* object)
{ 
	return object->GetType();
}

#endif