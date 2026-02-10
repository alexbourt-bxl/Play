#ifndef ACT_CREATE_OP_H
#define ACT_CREATE_OP_H

#include <Actions\Action.h>
#include <Operators\Operator.h>

class Tree;
class ActConnect;

class ActCreateOp
	: public Action
{
	DECLARE_TYPE;

	typedef Type _Type;
	typedef Tree _Tree;

public:
	ActCreateOp();
	ActCreateOp(_Tree* tree, __Type* type);
	~ActCreateOp();

	__declspec(property(get = GetTree, put = SetTree))
	_Tree* Tree;
	_Tree* GetTree();
	void   SetTree(_Tree* tree);

	__declspec(property(get = GetOpType, put = SetOpType))
	__Type* Type;
	__Type* GetOpType() const;
	void    SetOpType(_Type type);

	__declspec(property(get = GetConnectAfter, put = SetConnectAfter))
	Operator* ConnectAfter;
	Operator* GetConnectAfter();
	void      SetConnectAfter(Operator* op);

	bool Perform();
	void Restore();

private:
	_Tree*  m_tree;
	__Type* m_type;

	Operator* m_newOp;
	bool m_undone;
	
	/*	If an op is being created and previous op
		is specified, the two will be connected. */
	Operator* m_prevOp;
	ActConnect* m_actConnect;
};

#endif