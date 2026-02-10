#include "Color8.h"

#include <Types/Color\Color.h>

//byte Color8::s_lutGamma8[256];
//byte Color8::s_lutInvGamma8[256];
//bool Color8::s_initGamma = Color8::InitGamma();

DEFINE_TYPE(Color8, Object);

Color8::Color8()
{
	R = 0;
	G = 0;
	B = 0;
	A = 0;
}

Color8::Color8(const Color8& color)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = color.A;
}

Color8::Color8(const Color8& color, int a)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = a;
}

Color8::Color8(int rgb, int a)
{
	R = (rgb & 0x00FF0000) >> 16;
	G = (rgb & 0x0000FF00) >>  8;
	B = (rgb & 0x000000FF);
	A = a;
}

Color8::Color8(int rgba)
{
	R = (rgba & 0x00FF0000) >> 16;
	G = (rgba & 0x0000FF00) >>  8;
	B = (rgba & 0x000000FF);
	A = (rgba & 0xFF000000) >> 24;
}

Color8::Color8(int r, int g, int b, int a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

Color8::Color8(int r, int g, int b)
{
	R = r;
	G = g;
	B = b;
	A = 0xFF;
}

Object* Color8::Copy()
{
	return new Color8(*this);
}

Color8& Color8::operator = (int rgba)
{
	R = (rgba & 0x00FF0000) >> 16;
	G = (rgba & 0x0000FF00) >>  8;
	B = (rgba & 0x000000FF);
	A = (rgba & 0xFF000000) >> 24;

	return *this;
}

Color8& Color8::operator = (const Color8& color)
{
	R = color.R;
	G = color.G;
	B = color.B;
	A = color.A;

	return *this;
}

bool Color8::operator == (const Color8& color)
{
	return 
		   R == color.R 
		&& G == color.G 
		&& B == color.B
		&& A == color.A;
}

bool Color8::operator != (const Color8& color)
{
	return 
		   R != color.R 
		|| G != color.G 
		|| B != color.B
		|| A != color.A;
}

Color8::operator int()
{
	return
		(A << 24) | 
		(R << 16) |
		(G <<  8) |
		(B      );
}

int Color8::ToGdi()
{
	return
		(A << 24) | 
		(R      ) |
		(G <<  8) |
		(B << 16);
}

Color8::operator Color()
{
	return Color(
		(double)R / 255.0,
		(double)G / 255.0,
		(double)B / 255.0,
		(double)A / 255.0);
}

Color8 Color8::FromGdi(int gdi)
{
	return Color8(
		 (gdi & 0x00FF0000) >> 16,
		 (gdi & 0x0000FF00) >>  8,
		 (gdi & 0x000000FF),
		  0xFF);
}

//bool Color8::InitGamma()
//{
//	double gamma = 2.2;
//
//	for (int i = 0; i < 256; i++)
//		s_lutGamma8[i] = (byte)(pow((double)i / 255.0, 1.0 / gamma) * 255.0);
//
//	for (int i = 0; i < 256; i++)
//		s_lutInvGamma8[i] = (byte)(pow((double)i / 255.0, gamma) * 255.0);
//
//	return true;
//}