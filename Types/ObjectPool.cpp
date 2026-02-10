#include "ObjectPool.h"

#include <Types/Object.h>
#include <Containers/List.h>

ObjectPool::ObjectPool() : Hashtable(8)
{
	m_lastIssuedId = 0;

	CanResize = true;
	FactorUsedSlots = true;

	*m_deleteQueue.RemoveEvent += EventHandler(this, &ObjectPool::deleteQueue_OnRemove);
	
	Start();
}

ObjectPool::~ObjectPool()
{
	//Empty();
}

Array<Object*> ObjectPool::GetByType(Type type)
{
	Lock();

	Array<Object*> objects;

	for (int i = 0; i < SlotCount; i++)
	{
		List::Node* slot = (*this)[i].First;
		while (slot)
		{
			Object* obj = (Object*)slot->Data;
			
			if (typeof(obj) == type)
				objects.Add(obj);

			slot = slot->Next;
		}
	}

	Unlock();

	return objects;
}

uint ObjectPool::GetNextId()
{
	Lock();
	uint nextId = ++m_lastIssuedId;
	Unlock();

	return nextId;
}

//void ObjectPool::MarkForDelete(Object* obj)
//{
//	Lock();
//
//	Remove(obj); // doesn't delete the object
//	m_deleteQueue.Add(obj);
//
//	Unlock();
//	WakeUp();
//}

void ObjectPool::Empty()
{
	Lock();

	/*	Delete all remaining 
		objects from the pool. */
	//for (int i = 0; i < SlotCount; i++)
	//{
	//	List& slot = (*this)[i];

	//	while (slot.First)
	//	{
	//		delete (Object*)slot.First->Data;
	//		slot.Remove(slot.First);
	//	}
	//}

	/*	Delete all the obects 
		in the delete queue. */
	m_deleteQueue.Clear();

	/*	TODO: Garbage collection. */

	Clear();
	Unlock();
}

uint ObjectPool::ThreadProc()
{
	while (!WasKilled)
	{
		Lock();

		/*	Delete all the obects 
			in the delete queue. */
		m_deleteQueue.Clear();

		Unlock();
		Sleep();
	}

	//       POOL
	return 0x2002;
}

#ifdef TEST
void ObjectPool::UnitTest()
{
	for (uint i = 1; i <= 200; i++)
	{
		//system("cls");

		for (uint j = 0; j < i; j++)
			new string();

		for (uint j = 0; j < Object::Pool.slotCount; j++)
		{
			wprintf_s(L"%i\t: ", j);

			List::Node* slot = Object::Pool[j].first;
			while (slot)
			{
				Object* obj = (Object*)slot->data;
				wprintf_s(L"%s %i ", typeof(obj)->name, idof(obj));
				slot = slot->next;
			}

			wprintf_s(L"\n");
		}

		wprintf_s(L"Slots: %i\tUsed: %i\tObjects: %i\tLoad: %.1f%%\n",
			Object::Pool.slotCount,
			Object::Pool.m_nUsedSlots,
			Object::Pool.Count,
			(double)Object::Pool.m_nUsedSlots / (double)Object::Pool.slotCount * 100.0);

		Object::Pool.Clear();
		Object::Pool.m_lastIssuedID = 0;

		//getchar();
	}
}
#endif