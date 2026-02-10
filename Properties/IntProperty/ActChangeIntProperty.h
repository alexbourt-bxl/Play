#ifndef ACT_CHANGE_INT_PROPERTY_H
#define ACT_CHANGE_INT_PROPERTY_H

#include <Actions/Action.h>
#include <Operators/Operator.h>

class IntProperty;

class ActChangeIntProperty
	: public Action
{
	DECLARE_TYPE;

public:
	ActChangeIntProperty();
	ActChangeIntProperty(IntProperty* prop, int newValue, int oldValue);

	__declspec(property(get = GetProperty, put = SetProperty))
	IntProperty* Property;
	IntProperty* GetProperty() const;
	void SetProperty(IntProperty* prop);

	__declspec(property(get = GetNewValue, put = SetNewValue))
	int  NewValue;
	int  GetNewValue();
	void SetNewValue(int value);

	__declspec(property(get = GetOldValue, put = SetOldValue))
	int  OldValue;
	int  GetOldValue();
	void SetOldValue(int value);

	bool Perform();
	void Restore();

private:
	IntProperty* m_prop;
	int	m_newValue;
	int	m_oldValue;
};

#endif