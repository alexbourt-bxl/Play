#ifndef INT_PROPERTY_H
#define INT_PROPERTY_H

#include <Properties\Property.h>

class Valuebox;
class ValueMessage;

class IntProperty
	: public Property
{
	DECLARE_TYPE;

	friend class ActChangeIntProperty;

	typedef Valuebox _Valuebox;

public:
	IntProperty();
	explicit IntProperty(const IntProperty& prop);
	~IntProperty();

	Object* Copy();
	
	operator int();
	
	IntProperty& operator = (int i);

	IntProperty& operator += (int i);
	IntProperty& operator -= (int i);
	IntProperty& operator *= (int i);
	IntProperty& operator /= (int i);

	int operator - ();

	int operator + (int i);
	int operator - (int i);
	int operator * (int i);
	int operator / (int i);

	bool operator == (int i);
	bool operator != (int i);
	bool operator >  (int i);
	bool operator >= (int i);
	bool operator <  (int i);
	bool operator <= (int i);

	_Control* GetControl() const;

	__declspec(property(get = GetValuebox))
	_Valuebox* Valuebox;
	_Valuebox* GetValuebox() const;

	void CheckCreateValuebox();

protected:
	void PerformAction();

private:
	int m_value;
	int m_oldValue;
	void OnChange(Property::Message& msg);

	_Valuebox* m_valuebox;
	void valuebox_OnValueChange(ValueMessage& msg);
	void valuebox_OnCommitChange(ValueMessage& msg);
};

#endif