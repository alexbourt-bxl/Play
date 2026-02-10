#ifndef REGION_H
#define REGION_H

#include <Types/Object.h>
#include <Containers/Array.h>
#include <Types/Geometric/Rect.h>

class Span
{
public:
	int	Start;
	int	End;

	Span();
	Span(const Span& span);
	Span(int start, int end);
	Span& operator = (const Span& span);

	bool operator == (const Span& span);
};

//

class Region
	: public Object
{
	DECLARE_TYPE;

public:
	Region();
	Region(const Region& rgn);
	Region(const Rect& rect);

	Object* Copy();

	void Add(const Rect& rect);
	void Add(const Region& rgn);

	__declspec(property(get = GetIsNull))
	bool IsNull;
	bool GetIsNull();

	__declspec(property(get = GetBounds))
	Rect Bounds;
	Rect GetBounds();

	void Clear();

	void Clip(Region& rgn);
	void Clip(const Rect& rect);
	void Offset(const Point& p);

	bool Contains(const Rect& rect);
	bool Intersects(const Rect& rect);

	Region&	operator = (const Region& rgn);

	bool operator == (const Region& rgn);
	bool operator != (const Region& rgn);

	operator bool();

	__declspec(property(get = GetRects))
	Array<Rect>& Rects;
	Array<Rect>& GetRects();

	void GetSpans(int line, Array<Span>& spans);
	void GetSpans(int line, int start, int end, Array<Span>& spans);

private:
	Array<Rect> m_rects;

public:
	static Region Null;
};


#endif