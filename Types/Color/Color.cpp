#include "Color.h"

#include <Math\Math.h>

DEFINE_TYPE(Color, Object);

Color::Color()
{
	R = 0;
	G = 0;
	B = 0;
	A = 0;
}

Color::Color(const Color& color)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = color.A;
}

Color::Color(const Color& color, double a)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = a;
}

Color::Color(double r, double g, double b, double a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

Color::Color(double r, double g, double b)
{
	R = r;
	G = g;
	B = b;
	A = 1;
}

Object* Color::Copy()
{
	return new Color(*this);
}

Color& Color::operator = (const Color& color)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = color.A;

	return *this;
}

Color& Color::operator = (const Color8& color8)
{
	R = (double)color8.R / 255;
	G = (double)color8.G / 255;
	B = (double)color8.B / 255;
	A = (double)color8.A / 255;

	return *this;
}

Color::operator Color8()
{
	return Color8(
		Math::ToInt8(R),
		Math::ToInt8(G),
		Math::ToInt8(B),
		Math::ToInt8(A));
}

bool Color::operator == (const Color& color)
{
	return 
		   R == color.R 
		&& G == color.G 
		&& B == color.B
		&& A == color.A;
}

bool Color::operator != (const Color& color)
{
	return 
		   R != color.R 
		|| G != color.G 
		|| B != color.B
		|| A != color.A;
}

void Color::ToHsv(double* h, double* s, double* v)
{
	double fmin, fmax, delta;

	fmin = min(min(R, G), B);
	fmax = max(max(R, G), B);

	delta = fmax - fmin;

	*v = fmax;

	if (fmax != 0)
	{
		*s = delta / fmax;
	}
	else
	{
		// R = G = B = 0
		// h is undefined, so I don't touch it; this way when the saturation comes back up, it will be the same.
		// s = 0
		*s = 0;
		return;
	}
	
	if      (fmax == R) *h = fmod(60 * (G - B) / delta + 360, 360); // between yellow and magenta
	else if (fmax == G) *h = 60 * (B - R) / delta + 120; // between cyan and yellow
	else if (fmax == B) *h = 60 * (R - G) / delta + 240; // between between magenta and cyan
}

void Color::ToHsl(double* h, double* s, double* l)
{
	double fmin, fmax, delta;

	fmin = min(min(R, G), B);
	fmax = max(max(R, G), B);

	delta = fmax - fmin;

	*l = (fmax + fmin) / 2;

	if (fmax == fmin) /* In this case I leave the hue alone. */;
	else if (fmax == R) *h = fmod(60 * (G - B) / delta + 360, 360);
	else if (fmax == G) *h = 60 * (B - R) / delta + 120;
	else if (fmax == B) *h = 60 * (R - G) / delta + 240;

	if (fmax == fmin)    *s = 0;
	else if (*l <= 0.5) *s = delta / (2 * *l);
	else if (*l >  0.5) *s = delta / (2 - 2 * *l);
}

Color Color::FromHsv(double h, double s, double v)
{
	if (h < 0)
	{
		double th = -(double)((int)h / 360);
		h += (th + 1) * 360;
	}
	else if (h > 360)
	{
		double th = (double)((int)h / 360);
		h -= th * 360;
	}

	if (s == 0)
		return Color(v, v, v); // achromatic

	h /= 60;

	int i = (int)h;
	double f = h - (double)i;
	
	double p = v * (1 - s);
	double q = v * (1 - s * f);
	double t = v * (1 - s * (1 - f));

	switch (i)
	{
	case 0:  return Color(v, t, p);
	case 1:  return Color(q, v, p);
	case 2:  return Color(p, v, t);
	case 3:  return Color(p, q, v);
	case 4:  return Color(t, p, v);
	default: return Color(v, p, q);
	}
}

Color Color::FromHsl(double h, double s, double l)
{
	double q = l < 0.5
		? l * (1 + s)
		: l + s - l * s;

	double p = 2 * l - q;

	double hk = h / 360;

	double tR = hk + 1.0 / 3;
	double tG = hk;
	double tB = hk - 1.0 / 3;

	if (tR < 0) tR += 1; if (tR > 1) tR -= 1;
	if (tG < 0) tG += 1; if (tG > 1) tG -= 1;
	if (tB < 0) tB += 1; if (tB > 1) tB -= 1;

	double r, g, b;

	double qp6 = (q - p) * 6; 

	if      (tR < 1.0 / 6)				r = p + qp6 * tR;
	else if (tR >= 1.0 / 6 && tR < 0.5)	r = q;
	else if (tR >= 0.5 && tR < 2.0 / 3)	r = p + qp6 * (2.0 / 3 - tR);
	else									r = p;
	
	if      (tG < 1.0 / 6)				g = p + qp6 * tG;
	else if (tG >= 1.0 / 6 && tG < 0.5)	g = q;
	else if (tG >= 0.5 && tG < 2.0 / 3) 	g = p + qp6 * (2.0 / 3 - tG);
	else									g = p;

	if      (tB < 1.0 / 6)				b = p + qp6 * tB;
	else if (tB >= 1.0 / 6 && tB < 0.5) 	b = q;
	else if (tB >= 0.5 && tB < 2.0 / 3) 	b = p + qp6 * (2.0 / 3 - tB);
	else									b = p;

	return Color(r, g, b);
}

//bool Color::InitGamma()
//{
//	double gamma = 2.2;
//
//	for (int i = 0; i < 256; i++)
//		s_lutGamma8[i] = (byte)(pow((double)i / 255, 1 / gamma) * 255);
//
//	for (int i = 0; i < 256; i++)
//		s_lutInvGamma8[i] = (byte)(pow((double)i / 255, gamma) * 255);
//
//	return true;
//}

// Presets

Color Color::Transparent	= Color(0, 0, 0, 0);

Color Color::Black			= Color(0, 0, 0);
Color Color::DarkGray		= Color(0.25, 0.25, 0.25);
Color Color::Gray			= Color(0.5, 0.5, 0.5);
Color Color::LightGray		= Color(0.75, 0.75, 0.75);
Color Color::White			= Color(1, 1, 1);

Color Color::Red			= Color(1, 0, 0);
Color Color::Orange			= Color(1, 0.5, 0);
Color Color::Yellow			= Color(1, 1, 0);
Color Color::Chartreuse		= Color(0.5, 1, 0);
Color Color::Green			= Color(0, 1, 0);
Color Color::SpringGreen	= Color(0, 1, 0.5);
Color Color::Cyan			= Color(0, 1, 1);
Color Color::Azure			= Color(0, 0.5, 1);
Color Color::Blue			= Color(0, 0, 1);
Color Color::Violet			= Color(0.5, 0, 1);
Color Color::Magenta		= Color(1, 0, 1);
Color Color::Rose			= Color(1, 0, 0.5);

Color Color::Back			= Color(0.31, 0.31, 0.31);
Color Color::Panel			= Color(Color::Back, 0.82);
Color Color::PlayYellow		= Color(1, 0.81, 0.06);