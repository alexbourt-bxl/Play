#ifndef FONT_H
#define FONT_H

#include <Types/Object.h>
#include <Native/Native.h>
#include <Types/Basic/String.h>

class Graphics;

class Font
	: public Object
{
	DECLARE_TYPE;

	friend class Graphics;

public:	// Types

	enum _Weight
	{
		Normal,
		SemiBold,
		Bold
	};

public:
	Font();
	Font(const Font& font);
	Font(const String& name, double size, _Weight weight = Normal);
	~Font();

	Object* Copy();

	Font& operator = (const Font& font);

	__declspec(property(get = GetName))
	String Name;
	String GetName();

	__declspec(property(get = GetSize))
	double Size; // ugly, but must be so
	double GetSize();
	
	__declspec(property(get = GetWeight))
	_Weight Weight;
	_Weight GetWeight();

	__declspec(property(get = GetAverageCharWidth))
	int AverageCharWidth;
	int	GetAverageCharWidth();

private:
	void	Construct(const String& name, double size, _Weight weight);
	void	Destruct();

	int		m_mapMode;

	String	m_name;
	double	m_size;
	_Weight	m_weight;

public: // TODO: this is for Label, remove later
	HFONT	m_hfont;

public:
	static Font Default;
	static Font Label;
	static Font Button;
};

typedef Font Font;

#endif