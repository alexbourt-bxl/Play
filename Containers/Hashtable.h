#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <Types/BaseTypes.h>
#include <Containers/Container.h>

template <class Class> class Array;
class List;
class String;
class Object;

class Hashtable
	: public Container
{
public:
	Hashtable(int nSlots = 1024);
	virtual ~Hashtable();
	
	void InitSlots();

	void Add(Object* o);
	void Remove(Object* o);

	Object*	Get(uint objId);

	__declspec(property(get = GetSlotCount)) 
	int	SlotCount;
	int	GetSlotCount();

	__declspec(property(get = GetCount)) 
	int	Count;
	int	GetCount();

	List& operator [] (int index) const; // Returns the list at the specified slot.

	void Clear();

	__declspec(property(get = GetCanResize, put = SetCanResize))
	bool CanResize;
	bool GetCanResize();
	void SetCanResize(bool canResize);

	__declspec(property(get = GetLoadFactor, put = SetLoadFactor))
	double LoadFactor;
	double GetLoadFactor();
	void SetLoadFactor(double loadFactor);

	__declspec(property(get = GetFactorUsedSlots, put = SetFactorUsedSlots))
	bool FactorUsedSlots;
	bool GetFactorUsedSlots();
	void SetFactorUsedSlots(bool factorUsedSlots);

	bool CheckLoadFactor(uint hash);

	static uint	Hash(const String& s);
	static uint	Hash(int i);

protected:
	Array<List>* m_slots;

	int	  m_nObjects;
	int	  m_nUsedSlots;

private:
	bool  m_canResize;
	double m_loadFactor;

	/*	The load factor can be determined either as 
		object/slots or as used/total slots. */
	bool  m_factorUsedSlots;
};

#endif