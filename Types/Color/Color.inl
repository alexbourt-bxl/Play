#ifndef COLOR_INL
#define COLOR_INL

#include <Math\Math.h>

inline Color Color::Premult(const Color& color)
{
	return Color(
		color.R * color.A,
		color.G * color.A,
		color.B * color.A,
		color.A);
}

inline Color Color::Unpremult(const Color& color)
{
	double a = color.A;

	if (a == 0.0)
		a = ZERO;

	return Color(
		color.R / a,
		color.G / a,
		color.B / a,
		color.A);
}

inline Color Color::Add(const Color& color1, const Color& color2)
{
	return Color(
		color1.R + color2.R,
		color1.G + color2.G,
		color1.B + color2.B,
		color1.A + color2.A);
}

inline Color Color::Multiply(const Color& color, double f)
{
	return Color(
		color.R * f,
		color.G * f,
		color.B * f,
		color.A * f);
}

inline Color Color::Over(const Color& color1, const Color& color2)
{
	return Color(
		color2.R + color1.R - color1.R * color2.A,
		color2.G + color1.G - color1.G * color2.A,
		color2.B + color1.B - color1.B * color2.A,
		color2.A + color1.A - color1.A * color2.A);
}

inline Color Color::Over(const Color& color1, const Color& color2, double factor)
{
	Color color = Color::Multiply(color2, factor);

	return Color(
		color.R + color1.R - color1.R * color.A,
		color.G + color1.G - color1.G * color.A,
		color.B + color1.B - color1.B * color.A,
		color.A + color1.A - color1.A * color.A);
}

inline Color Color::Lerp(const Color& color1, const Color& color2, double factor)
{
	return Color(
		color1.R + (color2.R - color1.R) * factor,
		color1.G + (color2.G - color1.G) * factor,
		color1.B + (color2.B - color1.B) * factor,
		color1.A + (color2.A - color1.A) * factor);
}

inline Color Color::CLerp(const Color& color1, const Color& color2, double factor)
{
	double _1_factor = 1.0 - factor;

	return Color(
		color1.R * factor + color2.R * _1_factor,
		color1.G * factor + color2.G * _1_factor,
		color1.B * factor + color2.B * _1_factor,
		color1.A * factor + color2.A * _1_factor);
}

inline double Color::GetLuminance(const Color& color)
{
	return 0.2126 * color.R + 0.7152 * color.G + 0.0722 * color.B;
}

#endif