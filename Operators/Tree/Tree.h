#ifndef TREE_H
#define TREE_H

/*	Actions comprise a large part of manipulating the operators. 

	Creating, deleting and connecting operators should be done through 
	the tree, ensuring that each tree manages its own operators. */

#include <Types/Object.h>
#include <Containers/Array.h>
#include <Threads/Thread.h>
#include <Threads/ThreadLock.h>

class Operator;
class Connection;
class View;
class OpView;

class Tree
	: public Object,
	  public Array<Operator*>,
	  public Thread, public ThreadLock
{
	DECLARE_TYPE;

public:
	Tree();
	~Tree();

	Object* Copy();

	__declspec(property(get = GetSelected))
	Array<Operator*>& Selected;
	Array<Operator*>& GetSelected();

	__declspec(property(get = GetViews))
	Array<View*>& Views;
	Array<View*>& GetViews();

	Array<OpView*> GetOpViews(Operator* op);

	// Updates

	void Update(Operator* op);
	uint ThreadProc();

	// Events

	class Message;

	Event<Tree::Message&> PreUpdateEvent;
	Event<Tree::Message&> UpdateEvent;

private:
	Array<View*> m_views;

	/*	This working copy of the updated op also  acts 
		as a flag to update, cleared by ThreadProc(). */
	Operator* m_op; 
	ThreadLock m_opLock; // protects the working op pointer

	void OnAddOperator(Container::Message& msg);
	void OnRemoveOperator(Container::Message& msg);

	/*	Keeping a synchronized selection list
		makes working with selections much easier. */

	Array<Operator*> m_selected;

	void selected_OnAdd(Container::Message& msg);
	void selected_OnRemove(Container::Message& msg);
};

//

class Tree::Message
	: public ::Message
{
public:
	__declspec(property(get = GetOperators, put = SetOperators))
	Array<Operator*>* Operators;
	Array<Operator*>* GetOperators() { return m_ops; }
	void SetOperators(Array<Operator*>* ops) { m_ops = ops; }

	__declspec(property(get = GetConnections, put = SetConnections))
	Array<Connection*>* Connections;
	Array<Connection*>* GetConnections() { return m_connections; }
	void SetConnections(Array<Connection*>* connections) { m_connections = connections; }

	Message()
		: m_ops(null), m_connections(null) {}

private:
	Array<Operator*>* m_ops;
	Array<Connection*>* m_connections;
};

#endif