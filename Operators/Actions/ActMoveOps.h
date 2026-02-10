#ifndef ACT_MOVE_OPS_H
#define ACT_MOVE_OPS_H

#include <Actions\Action.h>
#include <Operators\Operator.h>
#include <Types/Geometric\FPoint.h>

class ActMoveOps
	: public Action
{
	DECLARE_TYPE;

public:
	ActMoveOps();
	ActMoveOps(Array<Operator*>& ops, FPoint offset); // for common offset

	__declspec(property(get = GetOperators))
	Array<Operator*>& Operators;
	Array<Operator*>& GetOperators();

	__declspec(property(get = GetOffsets))
	Array<FPoint>& Offsets;
	Array<FPoint>& GetOffsets();

	bool Perform();
	void Restore();

private:
	Array<Operator*> m_ops;
	Array<FPoint> m_offsets;
};

#endif