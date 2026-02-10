#ifndef ACT_DELETE_OPS_H
#define ACT_DELETE_OPS_H

#include <Actions/Action.h>
#include <Operators/Operator.h>
#include <Containers/Array.h>

class Tree;
class Connection;

class ActDeleteOps
	: public Action
{
	DECLARE_TYPE;

public:
	ActDeleteOps();
	ActDeleteOps(Array<Operator*>& ops);
	~ActDeleteOps();

	__declspec(property(get = GetOperators))
	Array<Operator*>& Operators;
	Array<Operator*>& GetOperators();

	bool Perform();
	void Restore();

private:
	Tree* m_tree;
	Array<Operator*> m_ops;

	/*	When operators are deleted, their connections
		are also deleted, and some of the connections
		are replaced with other connections. */
	Array<Connection*> m_deletedConnections;
	Array<Connection*> m_createdConnections;

	bool m_undone;
};

#endif