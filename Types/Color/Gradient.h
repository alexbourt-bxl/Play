#ifndef GRADIENT_H
#define GRADIENT_H

#include <Types/Object.h>
#include <Types/Color/Color.h>
#include <Containers/Array.h>

class Gradient
	: public Object
{
	DECLARE_TYPE;

public:
	class Color;

public:
	Gradient();
	Gradient(const Gradient& grad);

	Object* Copy();

	Gradient& operator = (const Gradient& grad);

	__declspec(property(get = GetColors))
	Array<Color>& Colors;
	Array<Color>& GetColors();

private:
	Array<Color> m_colors;
};

class Gradient::Color
{
public:
	Color();
	Color(::Color color, double position);

	operator ::Color();
	Color& operator = (const ::Color& color);

	__declspec(property(get = GetPosition, put = SetPosition))
	double Position;
	double GetPosition();
	void   SetPosition(double position);

private:
	::Color m_color;
	double  m_position;
};

#endif