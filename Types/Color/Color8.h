#ifndef COLOR_8_H
#define COLOR_8_H

#include <Types/Object.h>

class Color;

class Color8
	: public Object
{
	DECLARE_TYPE;

public:
	int R;
	int G;
	int B;
	int A;

public:
	Color8();
	Color8(const Color8& color);
	Color8(const Color8& color, int a);
	Color8(int rgb, int a);
	Color8(int rgba);
	Color8(int r, int g, int b, int a);
	Color8(int r, int g, int b);

	Object* Copy();

	Color8& operator = (int rgba);
	Color8& operator = (const Color8& color);

	bool operator == (const Color8& color);
	bool operator != (const Color8& color);

	operator int();
	int	ToGdi();

	operator Color();

	static Color8 FromGdi(int gdi);

	//

	static __forceinline int Premult(int rgba);
	static __forceinline int Unpremult(int rgba);

	static __forceinline int Add(int rgba1, int rgba2);
	static __forceinline int Subtract(int rgba1, int rgba2); // rgba1 - rgba2
	static __forceinline int AddRGB(int rgba, int i);
	static __forceinline int SubtractRGB(int rgba, int i);
	static __forceinline int Multiply(int rgba1, int rgba2);
	static __forceinline int MultiplyRGB(int rgba, int i);
	static __forceinline int MultiplyScalar(int rgba, int i);
	static __forceinline int Over(int rgba1, int rgba2);
	static __forceinline int OverSub(int rgba1, int rgba2, int suba); // subpixel premultiplied over
	static __forceinline int Over(int rgba1, int rgba2, int factor);	
	static __forceinline int Lerp(int rgba1, int rgba2, int factor);
	static __forceinline int CLerp(int rgba1, int rgba2, int factor);

	static inline int	GetLuminance(int rgba);

//private:
//	static int			s_lutGamma8[256];
//	static int			s_lutInvGamma8[256];
//	static bool			s_initGamma;
//	static bool			InitGamma();
};

#include "Color8.inl"

#endif