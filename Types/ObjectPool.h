#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <Containers/Hashtable.h>
#include <Containers/Array.h>
#include <Threads\Thread.h>
#include <Threads\ThreadLock.h>
#include <Types/Type.h>

/*
	The object pool takes care of managing objects,
	properly deleting them, etc.
*/

class ObjectPool
	: public Hashtable,
	  public Thread, public ThreadLock
{
	friend class Object;

public:
	ObjectPool();
	~ObjectPool();

	Array<Object*> GetByType(Type type);

	//void MarkForDelete(Object* obj);

	void Empty();

	uint ThreadProc();

private:
	uint m_lastIssuedId;
	uint GetNextId();

	/*	As a rudimentary form of garbage collection, an object 
		can be marked for deletion by the pool at a later time. */
	Array<Object*> m_deleteQueue;
	void deleteQueue_OnRemove(Container::Message& msg);

#ifdef TEST
public:
	static void UnitTest();
#endif
};

#endif