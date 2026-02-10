#include "Hashtable.h"

#include <Types/Object.h>
#include <Types/Basic\String.h>
#include <Containers/List.h>
#include <Containers/Array.h>
#include <Events/Event.h>

#include <stdlib.h>
#include <wchar.h>
#include <assert.h>
#include <new>

Hashtable::Hashtable(int nSlots)
{
	m_slots = new Array<List>(nSlots);

	for (int i = 0; i < m_slots->Count; i++)
		new (&(*m_slots)[i]) List();

	m_nObjects = 0;
	m_nUsedSlots = 0;

	m_canResize = false;
	m_loadFactor = 0.75;
	m_factorUsedSlots = false;
}

Hashtable::~Hashtable()
{
	for (int i = 0; i < m_slots->Count; i++)
		((List*)&(*m_slots)[i])->~List();

	if (m_slots)
		delete m_slots;
}

void Hashtable::Add(Object* obj)
{
	assert(m_slots);

	int hash = Hash(obj->GetId()) % m_slots->Count;

	if (m_canResize && CheckLoadFactor(hash))
		hash = Hash(obj->GetId()) % m_slots->Count; // update the hash if the table has been resized

	List& list = (*m_slots)[hash];

	if (!list.First) 
		m_nUsedSlots++;
	
	list.InsertLast(new List::Node(obj));
	m_nObjects++;

	Container::Message msg(Owner, obj);
	AddEvent->Send(msg);
}

void Hashtable::Remove(Object* obj)
{
	assert(m_slots);

	int hash = Hash(obj->GetId()) % m_slots->Count;

	List& list = (*m_slots)[hash];
	List::Node* slot = list.First;
	while (slot)
	{
		if (((Object*)slot->Data)->GetId() == obj->GetId())
		{
			list.Remove(slot);
			m_nObjects--;
			break;
		}
		
		slot = slot->Next;
	}

	if (!list.First)
		m_nUsedSlots--;

	Container::Message msg(Owner, obj);
	RemoveEvent->Send(msg);
}

Object* Hashtable::Get(uint objId)
{
	assert(m_slots);

	int hash = Hash(objId) % m_slots->Count;

	List::Node* slot = (*m_slots)[hash].First;
	while (slot)
	{
		if (((Object*)slot->Data)->GetId() == objId) 
			return (Object*)slot->Data;
		
		slot = slot->Next;
	}

	return null;
}

int Hashtable::GetSlotCount()
{
	assert(m_slots);
	return m_slots->Count;
}

int Hashtable::GetCount()
{
	return m_nObjects;
}

List& Hashtable::operator [] (int index) const
{
	assert(m_slots);
	assert(index < m_slots->Count);
	return (*m_slots)[index];
}

void Hashtable::Clear()
{
	assert(m_slots);

	for (int i = 0; i < m_slots->Count; i++)
	{
		List& list = (*m_slots)[i];

		if (list.First)
		{
			m_nObjects -= list.Count;
			m_nUsedSlots--;
			list.Clear();
		}
	}

	assert(m_nUsedSlots == 0 && m_nObjects == 0);
}

bool Hashtable::GetCanResize()
{
	return m_canResize;
}

void Hashtable::SetCanResize(bool canResize)
{
	m_canResize = canResize;
}

double Hashtable::GetLoadFactor()
{
	return m_loadFactor;
}

void Hashtable::SetLoadFactor(double loadFactor)
{
	m_loadFactor = loadFactor;
}

bool Hashtable::GetFactorUsedSlots()
{
	return m_factorUsedSlots;
}

void Hashtable::SetFactorUsedSlots(bool factorUsedSlots)
{
	m_factorUsedSlots = factorUsedSlots;
}

bool Hashtable::CheckLoadFactor(uint hash)
{
	// If the load factor threshold is reached, double the table size

	if (!(*m_slots)[hash].First // a new empty slot is needed
		&& (m_factorUsedSlots ? m_nUsedSlots : m_nObjects) >= (int)(m_loadFactor * (double)m_slots->Count))
	{
		// Resize array
		
		Array<List>* _slots = new Array<List>(m_slots->Count * 2); // double to minimize malloc fragmentation
		
		for (int i = 0; i < _slots->Count; i++)
			new (&(*_slots)[i]) List();

		m_nUsedSlots = 0;
		
		// Rehash all current values

		for (int i = 0; i < m_slots->Count; i++)
		{
			List& list = (*m_slots)[i];

			list.DisableEvents();

			List::Node* slot = list.First;
			while (slot)
			{
				int _hash = Hash(((Object*)slot->Data)->GetId()) % _slots->Count;
				
				List& _list = (*_slots)[_hash];
				
				if (!_list.First)
					m_nUsedSlots++;
		
				_list.DisableEvents();
				_list.InsertLast(new List::Node(slot->Data));
				_list.EnableEvents();

				List::Node* _slot = slot;
				slot = slot->Next;

				list.Remove(_slot);
			}

			list.EnableEvents();
		}

		// Delete old array

		for (int i = 0; i < m_slots->Count; i++)
			((List*)&(*m_slots)[i])->~List();
		
		delete m_slots;
		m_slots = _slots;

		return true;
	}

	return false;
}

uint Hashtable::Hash(const String& s)
{
	assert(((String&)s) != L"");

	int hash = 0;
	int keySize = ((String&)s).Length * sizeof(wchar_t);
	
	for (int i = 0; i < keySize; i++)
	{
		hash += ((String&)s)[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

uint Hashtable::Hash(int i)
{
	// Using Robert Jenkins' 32-bit int hashing algorithm.

	i = (i + 0x7ed55d16) + (i << 12);
	i = (i ^ 0xc761c23c) ^ (i >> 19);
	i = (i + 0x165667b1) + (i << 5);
	i = (i + 0xd3a2646c) ^ (i << 9);
	i = (i + 0xfd7046c5) + (i << 3);
	i = (i ^ 0xb55a4f09) ^ (i >> 16);

	return i;
}