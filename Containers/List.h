#ifndef LIST_H
#define LIST_H

#include <Containers/Container.h>

class List
	: public Container
{
public:
	class Node;

	Node* First;
	Node* Last;

public:
	List();
	List(bool createMessages);
	~List();

	__declspec(property(get = GetCount)) 
	int  Count;
	int	 GetCount();

	void InsertAfter(Node* node, Node* newNode);
	void InsertBefore(Node* node, Node* newNode);

	void InsertFirst(Node* newNode);
	void InsertLast(Node* newNode);

	void Remove(Node* node);

	void Clear();

	bool operator == (List& list);
};

//

class List::Node
{
public:
	void* Data;
	Node* Next;
	Node* Prev;

	Node(void* data)
		: Data(data), Next(null), Prev(null) {}
};

#endif