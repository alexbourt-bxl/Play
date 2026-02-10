#ifndef OP_VIEW_H
#define OP_VIEW_H

#include <Controls\Views\View.h>
#include <Operators\Operator.h>

class OpView 
	: public View
{
	DECLARE_ABSTRACT_TYPE;

	typedef Operator _Operator;

public:
	OpView();
	~OpView();

	__declspec(property(get = GetViewedOperator, put = SetViewedOperator))
	_Operator* ViewedOperator;
	_Operator* GetViewedOperator() const;
	bool SetViewedOperator(_Operator* op);

	__declspec(property(get = GetActiveOperator, put = SetActiveOperator))
	_Operator* ActiveOperator;
	_Operator* GetActiveOperator() const;
	bool SetActiveOperator(_Operator* op);

	virtual void UpdateView() = 0;

	Event<Operator::Message&> SetViewedOperatorEvent;
	Event<Operator::Message&> SetActiveOperatorEvent;

private:
	_Operator* m_opViewed;
	_Operator* m_opActive;
};

#endif