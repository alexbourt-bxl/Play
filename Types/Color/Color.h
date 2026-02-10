#ifndef COLOR_H
#define COLOR_H

#include <Types/Object.h>
#include <Types/Color/Color8.h>

class Color
	: public Object
{
	DECLARE_TYPE;

public:
	double R;
	double G;
	double B;
	double A;

public:
	Color();
	Color(const Color& color);
	Color(const Color& color, double a);
	Color(double r, double g, double b, double a);
	Color(double r, double g, double b);

	Object* Copy();

	Color& operator = (const Color& color);
	Color& operator = (const Color8& color8);

	operator Color8();

	bool operator == (const Color& color);
	bool operator != (const Color& color);

	void			ToHsv(double* h, double* s, double* v);
	void			ToHsl(double* h, double* s, double* l);

	static Color	FromHsv(double h, double s, double v);
	static Color	FromHsl(double h, double s, double l);

	//

	static inline Color	Premult(const Color& color);
	static inline Color	Unpremult(const Color& color);

	static inline Color	Add(const Color& color1, const Color& color2);
	static inline Color	Multiply(const Color& color, double f);
	static inline Color	Over(const Color& color1, const Color& color2);	
	static inline Color	Over(const Color& color1, const Color& color2, double factor);	
	static inline Color	Lerp(const Color& color1, const Color& color2, double factor);
	static inline Color	CLerp(const Color& color1, const Color& color2, double factor);

	static inline double GetLuminance(const Color& color);

public: // Presets

	static Color 	Transparent;

	static Color 	Black;
	static Color 	DarkGray;
	static Color 	Gray;
	static Color 	LightGray;
	static Color 	White;
	
	static Color 	Red;
	static Color 	Orange;
	static Color 	Yellow;
	static Color 	Chartreuse;
	static Color 	Green;
	static Color 	SpringGreen;
	static Color 	Cyan;
	static Color 	Azure;
	static Color 	Blue;
	static Color 	Violet;
	static Color 	Magenta;
	static Color 	Rose;

	static Color	Back; 
	static Color	Panel;
	static Color	PlayYellow;
};

#include "Color.inl"

#endif