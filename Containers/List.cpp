#include "List.h"

#include <Events/Event.h>

List::List()
{
	First = null;
	Last  = null;
}

List::List(bool createMessages)
	: Container(createMessages)
{
	First = null;
	Last = null;
}

List::~List()
{
	Clear();
}

int List::GetCount()
{
	int count = 0;

	Node* node = First;
	while (node)
	{
		count++;
		node = node->Next;
	}

	return count;
}

void List::InsertAfter(List::Node* node, List::Node* newNode)
{
	newNode->Prev = node;
	newNode->Next = node->Next;

	if (node->Next)
		node->Next->Prev = newNode;
	else
		Last = newNode;

	node->Next = newNode;

	if (AddEvent)
	{
		Container::Message msg(Owner, newNode->Data);
		AddEvent->Send(msg);
	}
}

void List::InsertBefore(List::Node* node, List::Node* newNode)
{
	newNode->Prev = node->Prev;
	newNode->Next = node;

	if (node->Prev)
		node->Prev->Next = newNode;
	else
		First = newNode;

	node->Prev = newNode;

	if (AddEvent)
	{
		Container::Message msg(Owner, newNode->Data);
		AddEvent->Send(msg);
	}
}

void List::InsertFirst(List::Node* newNode)
{
	if (First)
	{
		InsertBefore(First, newNode);
	}
	else
	{
		First = newNode;
		Last  = newNode;
		newNode->Prev = null;
		newNode->Next = null;

		if (AddEvent)
		{
			Container::Message msg(Owner, newNode->Data);
			AddEvent->Send(msg);
		}
	}
}

void List::InsertLast(List::Node* newNode)
{
	if (Last)
		InsertAfter(Last, newNode);
	else
		InsertFirst(newNode);
}

void List::Remove(List::Node* node)
{
	if (node->Prev)
		node->Prev->Next = node->Next;
	else
		First = node->Next;

	if (node->Next)
		node->Next->Prev = node->Prev;
	else
		Last = node->Prev;

	if (RemoveEvent)	
	{
		Container::Message msg(Owner, node->Data);
		RemoveEvent->Send(msg);
	}

	delete node;
}

void List::Clear()
{
	while (First)
		Remove(First);
}

bool List::operator == (List& list)
{
	return this == &list; // A stupid comparison to be able to use List in Array
}