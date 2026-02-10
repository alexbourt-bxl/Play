#ifndef FLOAT_PROPERTY_H
#define FLOAT_PROPERTY_H

#include <Properties\Property.h>

class Valuebox;
class ValueMessage;

class FloatProperty
	: public Property
{
	DECLARE_TYPE;

	typedef Valuebox _Valuebox;

public:
	FloatProperty();
	explicit FloatProperty(const FloatProperty& prop);
	~FloatProperty();

	Object* Copy();

	operator double();
	
	FloatProperty& operator = (double f);

	FloatProperty& operator += (double f);
	FloatProperty& operator -= (double f);
	FloatProperty& operator *= (double f);
	FloatProperty& operator /= (double f);

	double operator - ();

	double operator + (double f);
	double operator - (double f);
	double operator * (double f);
	double operator / (double f);

	bool operator == (double f);
	bool operator == (int i);
	bool operator != (double f);
	bool operator != (int i);
	bool operator >  (double f);
	bool operator >  (int i);
	bool operator >= (double f);
	bool operator >= (int i);
	bool operator <  (double f);
	bool operator <  (int i);
	bool operator <= (double f);
	bool operator <= (int i);

	_Control* GetControl() const;

	__declspec(property(get = GetValuebox))
	_Valuebox* Valuebox;
	_Valuebox* GetValuebox() const;

	void CheckCreateValuebox();

protected:
	void PerformAction();

private:
	double m_value;
	void OnChange(Property::Message& msg);

	_Valuebox* m_valuebox;
	void valuebox_OnValueChange(ValueMessage& msg);
};

#endif