#ifndef COLOR_8_INL
#define COLOR_8_INL

#include <Math\Math.h>

#define ALPHA8(x) (((x) & 0xFF000000) >> 24)

__forceinline int Color8::Premult(int rgba)
{
	int r = (rgba & 0x00FF0000) >> 16;
	int g = (rgba & 0x0000FF00) >>  8;
	int b = (rgba & 0x000000FF);
	int a = (rgba & 0xFF000000) >> 24;
	int t;

	r = INT_MULT(r, a, t);
	g = INT_MULT(g, a, t);
	b = INT_MULT(b, a, t);

	return
		(r << 16) |
		(g <<  8) |
		(b      ) |
		(a << 24);
}

__forceinline int Color8::Unpremult(int rgba)
{
	int r = (rgba & 0x00FF0000) >> 16;
	int g = (rgba & 0x0000FF00) >>  8;
	int b = (rgba & 0x000000FF);
	int a = (rgba & 0xFF000000) >> 24;

	r = INT_DIV_M(r, a);
	g = INT_DIV_M(g, a);
	b = INT_DIV_M(b, a);

	return
		(r << 16) |
		(g <<  8) |
		(b      ) |
		(a << 24);
}

__forceinline int Color8::Add(int rgba1, int rgba2)
{
	int r1 = (rgba1 & 0x00FF0000) >> 16;
	int g1 = (rgba1 & 0x0000FF00) >>  8;
	int b1 = (rgba1 & 0x000000FF);
	int a1 = (rgba1 & 0xFF000000) >> 24;

	int r2 = (rgba2 & 0x00FF0000) >> 16;
	int g2 = (rgba2 & 0x0000FF00) >>  8;
	int b2 = (rgba2 & 0x000000FF);
	int a2 = (rgba2 & 0xFF000000) >> 24;

	int r = min(r1 + r2, 0xFF);
	int g = min(g1 + g2, 0xFF);
	int b = min(b1 + b2, 0xFF);
	int a = min(a1 + a2, 0xFF);

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::Subtract(int rgba1, int rgba2)
{
	int r1 = (rgba1 & 0x00FF0000) >> 16;
	int g1 = (rgba1 & 0x0000FF00) >>  8;
	int b1 = (rgba1 & 0x000000FF);
	int a1 = (rgba1 & 0xFF000000) >> 24;

	int r2 = (rgba2 & 0x00FF0000) >> 16;
	int g2 = (rgba2 & 0x0000FF00) >>  8;
	int b2 = (rgba2 & 0x000000FF);
	int a2 = (rgba2 & 0xFF000000) >> 24;

	int r = max(0, r1 - r2);
	int g = max(0, g1 - g2);
	int b = max(0, b1 - b2);
	int a = max(0, a1 - a2);

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::AddRGB(int rgba, int i)
{
	int r = (rgba & 0x00FF0000) >> 16;
	int g = (rgba & 0x0000FF00) >>  8;
	int b = (rgba & 0x000000FF);
	int a = (rgba & 0xFF000000) >> 24;

	r = min(r + i, 0xFF);
	g = min(g + i, 0xFF);
	b = min(b + i, 0xFF);
	//a = min(a + i, 0xFF);

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::SubtractRGB(int rgba, int i)
{
	int r = (rgba & 0x00FF0000) >> 16;
	int g = (rgba & 0x0000FF00) >>  8;
	int b = (rgba & 0x000000FF);
	int a = (rgba & 0xFF000000) >> 24;

	r = max(0, r - i);
	g = max(0, g - i);
	b = max(0, b - i);
	//a = max(0, a - i);

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::Multiply(int rgba1, int rgba2)
{
	int r1 = /*s_lutInvGamma8[*/(rgba1 & 0x00FF0000) >> 16/*]*/;
	int g1 = /*s_lutInvGamma8[*/(rgba1 & 0x0000FF00) >>  8/*]*/;
	int b1 = /*s_lutInvGamma8[*/(rgba1 & 0x000000FF)/*]*/;
	int a1 = (rgba1 & 0xFF000000) >> 24;

	int r2 = /*s_lutInvGamma8[*/(rgba2 & 0x00FF0000) >> 16/*]*/;
	int g2 = /*s_lutInvGamma8[*/(rgba2 & 0x0000FF00) >>  8/*]*/;
	int b2 = /*s_lutInvGamma8[*/(rgba2 & 0x000000FF)/*]*/;
	int a2 = (rgba2 & 0xFF000000) >> 24;

	int t;

	r1 = INT_MULT(r1, r2, t);
	g1 = INT_MULT(g1, g2, t);
	b1 = INT_MULT(b1, b2, t);
	a1 = INT_MULT(a1, a2, t);

	//r = s_lutGamma8[r];
	//g = s_lutGamma8[g];
	//b = s_lutGamma8[b];

	return
		(a1 << 24) | 
		(r1 << 16) |
		(g1 <<  8) |
		(b1      );
}

__forceinline int Color8::MultiplyRGB(int rgba, int i)
{
	int r = /*s_lutInvGamma8[*/(rgba & 0x00FF0000) >> 16/*]*/;
	int g = /*s_lutInvGamma8[*/(rgba & 0x0000FF00) >>  8/*]*/;
	int b = /*s_lutInvGamma8[*/(rgba & 0x000000FF)/*]*/;
	int a = (rgba & 0xFF000000) >> 24;

	int t;

	r = min(INT_MULT(r, i, t), 0xFF);
	g = min(INT_MULT(g, i, t), 0xFF);
	b = min(INT_MULT(b, i, t), 0xFF);
	//a = INT_MULT(a, i, t);

	//r = s_lutGamma8[r];
	//g = s_lutGamma8[g];
	//b = s_lutGamma8[b];

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::MultiplyScalar(int rgba, int i)
{
	int r = /*s_lutInvGamma8[*/(rgba & 0x00FF0000) >> 16/*]*/;
	int g = /*s_lutInvGamma8[*/(rgba & 0x0000FF00) >>  8/*]*/;
	int b = /*s_lutInvGamma8[*/(rgba & 0x000000FF)/*]*/;
	int a = (rgba & 0xFF000000) >> 24;

	int t;

	r = INT_MULT(r, i, t);
	g = INT_MULT(g, i, t);
	b = INT_MULT(b, i, t);
	a = INT_MULT(a, i, t);

	//r = s_lutGamma8[r];
	//g = s_lutGamma8[g];
	//b = s_lutGamma8[b];

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::Over(int rgba1, int rgba2)
{
	/*
		This function composites premultiplied colors.
	*/

	int t;

	int r1 = /*s_lutInvGamma8[*/(rgba1 & 0x00FF0000) >> 16/*]*/;
	int g1 = /*s_lutInvGamma8[*/(rgba1 & 0x0000FF00) >>  8/*]*/;
	int b1 = /*s_lutInvGamma8[*/(rgba1 & 0x000000FF)/*]*/;
	int a1 = (rgba1 & 0xFF000000) >> 24;

	int r2 = /*s_lutInvGamma8[*/(rgba2 & 0x00FF0000) >> 16/*]*/;
	int g2 = /*s_lutInvGamma8[*/(rgba2 & 0x0000FF00) >>  8/*]*/;
	int b2 = /*s_lutInvGamma8[*/(rgba2 & 0x000000FF)/*]*/;
	int a2 = (rgba2 & 0xFF000000) >> 24;

	int r = INT_OVER(r1, r2, a2, t);
	int g = INT_OVER(g1, g2, a2, t);
	int b = INT_OVER(b1, b2, a2, t);
	int a = INT_OVER(a1, a2, a2, t);

	//r = s_lutGamma8[r];
	//g = s_lutGamma8[g];
	//b = s_lutGamma8[b];

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::Over(int rgba1, int rgba2, int factor)
{
	/*
		This function composites premultiplied colors.
	*/

	int t;

	int r1 = /*s_lutInvGamma8[*/(rgba1 & 0x00FF0000) >> 16/*]*/;
	int g1 = /*s_lutInvGamma8[*/(rgba1 & 0x0000FF00) >>  8/*]*/;
	int b1 = /*s_lutInvGamma8[*/(rgba1 & 0x000000FF)/*]*/;
	int a1 = (rgba1 & 0xFF000000) >> 24;

	int r2 = INT_MULT(/*s_lutInvGamma8[*/(rgba2 & 0x00FF0000) >> 16/*]*/, factor, t);
	int g2 = INT_MULT(/*s_lutInvGamma8[*/(rgba2 & 0x0000FF00) >>  8/*]*/, factor, t);
	int b2 = INT_MULT(/*s_lutInvGamma8[*/(rgba2 & 0x000000FF)      /*]*/, factor, t);
	int a2 = INT_MULT((rgba2 & 0xFF000000) >> 24, factor, t);

	int r = INT_OVER(r1, r2, a2, t);
	int g = INT_OVER(g1, g2, a2, t);
	int b = INT_OVER(b1, b2, a2, t);
	int a = INT_OVER(a1, a2, a2, t);

	//r = s_lutGamma8[r];
	//g = s_lutGamma8[g];
	//b = s_lutGamma8[b];

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::OverSub(int rgba1, int rgba2, int suba)
{
	/*
		This function composites premultiplied colors with premultiplied alpha.
	*/

	int t;

	int r1 = (rgba1 & 0x00FF0000) >> 16;
	int g1 = (rgba1 & 0x0000FF00) >>  8;
	int b1 = (rgba1 & 0x000000FF);
	int a1 = (rgba1 & 0xFF000000) >> 24;

	int r2 = (rgba2 & 0x00FF0000) >> 16;
	int g2 = (rgba2 & 0x0000FF00) >>  8;
	int b2 = (rgba2 & 0x000000FF);
	int a2 = (rgba2 & 0xFF000000) >> 24;

	int ar = (suba & 0x00FF0000) >> 16;
	int ag = (suba & 0x0000FF00) >>  8;
	int ab = (suba & 0x000000FF);
	int aa = (suba & 0xFF000000) >> 24;

	//int r2 = INT_MULT((rgba2 & 0x00FF0000) >> 16, ar, t);
	//int g2 = INT_MULT((rgba2 & 0x0000FF00) >>  8, ag, t);
	//int b2 = INT_MULT((rgba2 & 0x000000FF)      , ab, t);
	//int a2 = INT_MULT((rgba2 & 0xFF000000) >> 24, aa, t);

	int r = INT_OVER(r1, r2, ar, t);
	int g = INT_OVER(g1, g2, ag, t);
	int b = INT_OVER(b1, b2, ab, t);
	int a = INT_OVER(a1, a2, aa, t);

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::Lerp(int rgba1, int rgba2, int factor)
{
	/*
		This function lerps premultiplied colors.
	*/

	int t;

	int r1 = /*s_lutInvGamma8[*/(rgba1 & 0x00FF0000) >> 16/*]*/;
	int g1 = /*s_lutInvGamma8[*/(rgba1 & 0x0000FF00) >>  8/*]*/;
	int b1 = /*s_lutInvGamma8[*/(rgba1 & 0x000000FF)/*]*/;
	int a1 = (rgba1 & 0xFF000000) >> 24;

	int r2 = /*s_lutInvGamma8[*/(rgba2 & 0x00FF0000) >> 16/*]*/;
	int g2 = /*s_lutInvGamma8[*/(rgba2 & 0x0000FF00) >>  8/*]*/;
	int b2 = /*s_lutInvGamma8[*/(rgba2 & 0x000000FF)/*]*/;
	int a2 = (rgba2 & 0xFF000000) >> 24;

	int r = INT_LERP(r1, r2, factor, t);
	int g = INT_LERP(g1, g2, factor, t);
	int b = INT_LERP(b1, b2, factor, t);
	int a = INT_LERP(a1, a2, factor, t);

	//r = s_lutGamma8[r];
	//g = s_lutGamma8[g];
	//b = s_lutGamma8[b];

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

__forceinline int Color8::CLerp(int rgba1, int rgba2, int factor)
{
	/*
		This function lerps premultiplied colors.
	*/

	int t;

	int r1 = /*s_lutInvGamma8[*/(rgba1 & 0x00FF0000) >> 16/*]*/;
	int g1 = /*s_lutInvGamma8[*/(rgba1 & 0x0000FF00) >>  8/*]*/;
	int b1 = /*s_lutInvGamma8[*/(rgba1 & 0x000000FF)/*]*/;
	int a1 = (rgba1 & 0xFF000000) >> 24;

	int r2 = /*s_lutInvGamma8[*/(rgba2 & 0x00FF0000) >> 16/*]*/;
	int g2 = /*s_lutInvGamma8[*/(rgba2 & 0x0000FF00) >>  8/*]*/;
	int b2 = /*s_lutInvGamma8[*/(rgba2 & 0x000000FF)/*]*/;
	int a2 = (rgba2 & 0xFF000000) >> 24;

	int r = INT_CLERP(r1, r2, factor, t);
	int g = INT_CLERP(g1, g2, factor, t);
	int b = INT_CLERP(b1, b2, factor, t);
	int a = INT_CLERP(a1, a2, factor, t);

	//r = s_lutGamma8[r];
	//g = s_lutGamma8[g];
	//b = s_lutGamma8[b];

	return
		(a << 24) | 
		(r << 16) |
		(g <<  8) |
		(b      );
}

inline int Color8::GetLuminance(int rgba)
{
	int r = (rgba & 0x00FF0000) >> 16;
	int g = (rgba & 0x0000FF00) >>  8;
	int b = (rgba & 0x000000FF);

	//return 0.2126 * r + 0.7152 * g + 0.0722 * b;
	return (54 * r + 183 * g + 18 * b) >> 8;
}

#endif