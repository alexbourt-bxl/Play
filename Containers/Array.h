#ifndef ARRAY_H
#define ARRAY_H

#include <Containers/Container.h>
#include <Math\Math.h>
#include <Events/Event.h>

#include <assert.h>
#include <new>

/*	Container classes like Array and List do not derive from Object,
	because they are utility classes used to store actual objects. */

/*	If the array will be manually resized, the caller must provide
	a handler for ResizeAddEvent, which will take care of creating
	the new items. */

template <class Message>
class Event;

template <class Class>
class Array
	: public Container
{
public:
	Array()
	{
		ResizeAddEvent = new Event<Container::Message&>();
		ResizeAddEvent->Sender = this;

		m_arraySize = 8;
		m_items = (Class*)malloc(m_arraySize * sizeof(Class));
		m_nItems = 0;
	}

	Array(bool createMessages)
		: Container(createMessages)
	{
		if (createMessages)
		{
			ResizeAddEvent = new Event<Container::Message&>();
			ResizeAddEvent->Sender = this;
		}
		else
		{
			ResizeAddEvent = null;
		}

		m_arraySize = 8;
		m_items = (Class*)malloc(m_arraySize * sizeof(Class));
		m_nItems = 0;
	}

	Array(const Array& array)
	{
		if (array.ResizeAddEvent)
		{
			ResizeAddEvent = new Event<Container::Message&>();
			ResizeAddEvent->Sender = this;
		}
		else
		{
			ResizeAddEvent = null;
		}

		m_arraySize = array.m_arraySize;
		m_items = (Class*)malloc(m_arraySize * sizeof(Class));
		m_nItems = array.m_nItems;

		for (int i = 0; i < m_nItems; i++)
		{
			new (&m_items[i]) Class; //(); // construct item
			m_items[i] = array.m_items[i];

			/*	No events called here. */
		}
	}

	Array(int count)
	{
		ResizeAddEvent = new Event<Container::Message&>();
		ResizeAddEvent->Sender = this;

		m_arraySize = Math::GetNextPowerOf2(count);
		m_items = (Class*)malloc(m_arraySize * sizeof(Class));
		m_nItems = count;
	}

	~Array()
	{
		Clear();

		if (m_items)
			free(m_items);

		if (ResizeAddEvent)
			delete ResizeAddEvent;
	}

	operator Class*()
	{
		return m_items;
	}

	Array& operator = (const Array& array)
	{
		Class* _items = (Class*)realloc(m_items, array.m_arraySize * sizeof(Class));
		assert(_items);

		m_arraySize = array.m_arraySize;
		m_items = _items;
		m_nItems = array.m_nItems;

		for (int i = 0; i < m_nItems; i++)
		{
			new (&m_items[i]) Class; //(); // construct item
			m_items[i] = array.m_items[i];

			/*	No events called here. */
		}

		return *this;
	}

	__declspec(property(get = GetCount)) 
	int Count;
	inline int GetCount() 
	{ 
		return m_nItems; 
	}

	inline Class& GetItem(int index) const
	{
		assert(index >= 0 && index < m_nItems);
		return m_items[index];
	}

	inline Class& operator [] (int index) const
	{
		assert(index >= 0 && index < m_nItems);
		return m_items[index];
	}

	void Resize(int nItems)
	{
		uint _arraySize = Math::GetNextPowerOf2(nItems);

		/*	Properly remove trimmed items. */

		if (nItems < m_nItems)
		{
			for (int i = nItems; i < m_nItems; i++)
			{
				Container::Message msg(Owner, &m_items[i]);
				msg.Result = true;

				if (RemoveEvent)
					RemoveEvent->Send(msg);

				/*	Cancelling the removal here
					will produce no result. */

				//m_items[i].~Class(); // destruct item
			}
		}

		Class* _items = (Class*)realloc(m_items, _arraySize * sizeof(Class));
		assert(_items);

		m_arraySize = _arraySize;
		m_items = _items;

		/*	Construct new items. */

		if (nItems > m_nItems)
		{
			for (int i = m_nItems; i < nItems; i++)
			{
				new (&m_items[i]) Class; //(); // construct item

				Container::Message msg(Owner, &m_items[i]);
				msg.Result = true;

				if (ResizeAddEvent)
					ResizeAddEvent->Send(msg);

				if (AddEvent)
					AddEvent->Send(msg);

				/*	Cancelling the addition here
					will produce no result. */
			}
		}

		m_nItems = nItems;
	}

	void Add(Class item)
	{
		/*	The AddEvent is sent before the buffer is resized which 
			makes it possible to do things like clear the array (from 
			the event handler) before the new item is added. */

		Container::Message msg(Owner, &item);
		msg.Result = true;

		if (AddEvent)
			AddEvent->Send(msg);

		/*	The addition of the item can be 
			cancelled by the event handler. */
		if (!msg.Result)
			return;

		/*	Now the buffer is resized and the new item added. */

		if (m_nItems >= (signed)m_arraySize)
		{
			Class* _items = (Class*)realloc(m_items, m_arraySize * 2 * sizeof(Class));
			assert(_items);

			m_arraySize *= 2;
			m_items = _items;
		}

		new (&m_items[m_nItems]) Class; //(); // construct item
		m_items[m_nItems++] = item;
	}

	void Remove(Class item)
	{
		for (int i = 0; i < m_nItems; i++)
		{
			if (m_items[i] == item)
			{
				RemoveAt(i);
				return;
			}
		}
	}

	void RemoveAt(int index)
	{
		assert(index < m_nItems);

		Container::Message msg(Owner, &m_items[index]);
		msg.Result = true;

		if (RemoveEvent)
			RemoveEvent->Send(msg);

		/*	The removal of the item can be 
			cancelled by the event handler. */
		if (!msg.Result)
			return;

		/*	TODO: 
		
			removal and addition should be cancellable from all containers. */

		//m_items[index].~Class(); // destruct item

		for (int i = index; i < m_nItems - 1; i++)
			m_items[i] = m_items[i + 1];

		m_nItems--;
	}

	void Clear()
	{
		for (int i = 0; i < m_nItems; i++)
		{
			Container::Message msg(Owner, &m_items[i]);
			msg.Result = true;

			if (RemoveEvent)
				RemoveEvent->Send(msg);

			/*	Cancelling the removal here
				will produce no result. */

			//m_items[i].~Class(); // destruct item
		}

		m_nItems = 0;
	}

	int GetIndexOf(Class item)
	{
		for (int i = m_nItems - 1; i >= 0; i--) // TODO: Why do I do this backwards here?
		{
			if (m_items[i] == item)
				return i;
		}

		return -1;
	}

	void SetIndexOf(Class item, int newIndex)
	{
		int index = GetIndexOf(item);

		assert(index > -1);
		SetIndexAt(index, newIndex);
	}

	void SetIndexAt(int index, int newIndex)
	{
		if (index == newIndex)
			return;

		assert(index < m_nItems && newIndex < m_nItems);

		Class item = m_items[index];

		if (index <= newIndex)
		{
			for (int i = index; i < newIndex; i++)
				m_items[i] = m_items[i + 1];
		}
		else
		{
			for (int i = index; i > newIndex; i--)
				m_items[i] = m_items[i - 1];
		}

		m_items[newIndex] = item;
	}

	bool Contains(Class item)
	{
		return GetIndexOf(item) > -1;
	}

	/*	I have to use a pointer here because of the circular 
		dependency between Event, Array and Container. */
	Event<Container::Message&>* ResizeAddEvent;

private:
	uint	m_arraySize;
	Class*	m_items;
	int		m_nItems;
};

#endif