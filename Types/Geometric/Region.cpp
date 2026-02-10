#include "Region.h"

#include <stdlib.h>
#include <assert.h>

#include "Math.h"

DEFINE_TYPE(Region, Object);

Region::Region()
{

}

Region::Region(const Region& rgn)
{
	Add(rgn);
}

Region::Region(const Rect& rect)
{
	Rects.Add(rect);
}

Object* Region::Copy()
{
	return new Region(*this);
}

void Region::Add(const Rect& rect)
{
	if (!Contains(rect))
		Rects.Add(rect);
}

void Region::Add(const Region& rgn)
{
	for (int i = 0; i < ((Region&)rgn).Rects.Count; i++)
	{
		if (!Contains(((Region&)rgn).Rects[i]))
			Rects.Add(((Region&)rgn).Rects[i]);
	}
}

bool Region::GetIsNull()
{
	return m_rects.Count == 0;
}

Rect Region::GetBounds()
{
	Rect bounds;

	for (int i = 0; i < Rects.Count; i++)
		bounds.Expand(Rects[i]);

	return bounds;
}

void Region::Clear()
{
	Rects.Clear();
}

void Region::Clip(Region& rgn)
{
	Region _rgn = *this;

	Clear();

	for (int i = 0; i < _rgn.Rects.Count; i++)
	{
		for (int j = 0; j < rgn.Rects.Count; j++)
		{
			Rect rect = _rgn.Rects[i];
			rect.Clip(rgn.Rects[j]);
			Add(rect);
		}
	}
}

void Region::Clip(const Rect& rect)
{
	Region _rgn = *this;

	Clear();

	for (int i = 0; i < _rgn.Rects.Count; i++)
	{
		Rect _rect = _rgn.Rects[i];
		_rect.Clip(rect);
		Add(_rect);
	}
}

void Region::Offset(const Point& p)
{
	for (int i = 0; i < Rects.Count; i++)
		Rects[i].Offset(p);
}

bool Region::Contains(const Rect& /*rect*/)
{
	// TODO

	return false;
}

bool Region::Intersects(const Rect& rect)
{
	for (int i = 0; i < Rects.Count; i++)
	{
		if (((Rect&)rect).Intersects(Rects[i]))
			return true;
	}

	return false;
}

Region& Region::operator = (const Region& rgn)
{
	Clear();
	Add(rgn);
	return *this;
}

bool Region::operator == (const Region& _rgn)
{
	Region& rgn = (Region&)_rgn;

	if (Rects.Count == 0 && rgn.Rects.Count == 0)
		return true;

	return false;
}

bool Region::operator != (const Region& rgn)
{
	return !(*this == rgn);
}

Region::operator bool()
{
	return !IsNull;
}

Array<Rect>& Region::GetRects()
{
	return m_rects;
}

void Region::GetSpans(int line, Array<Span>& spans)
{
	spans.Clear();

	/*	Find all rects that intersect the scanline. */

	for (int i = 0; i < m_rects.Count; i++)
	{
		if (line >= m_rects[i].Top
			&& line < m_rects[i].Bottom)
		{
			/*	Compare the span of this rect with the spans
				that have already been added. */

			Span span(m_rects[i].Left, m_rects[i].Right);

			bool overlap = false;

			for (int j = 0; j < spans.Count; j++)
			{
				/*	If they are overlaid, combine them. */

				if (!(span.Start > spans[j].End
					|| span.End < spans[j].Start))
				{
					spans[j].Start = min(spans[j].Start, span.Start);
					spans[j].End = max(spans[j].End, span.End);

					overlap = true;

					/*	This one check here is enough because
						every new span is tested against already
						added spans, so they will add correctly. */

					break;
				}
			}

			/*	If the span doesn't overlap any 
				existing spans, add it directly. */

			if (!overlap)
				spans.Add(span);
		}
	}
}

void Region::GetSpans(int line, int start, int end, Array<Span>& spans)
{
	spans.Clear();

	/*	Find all rects that intersect the scanline. */

	for (int i = 0; i < m_rects.Count; i++)
	{
		if (line >= m_rects[i].Top
			&& line < m_rects[i].Bottom)
		{
			/*	Compare the span of this rect with the spans
				that have already been added. */

			Span span(
				max(start, m_rects[i].Left), 
				min(m_rects[i].Right, end));

			if (span.Start >= span.End)
				continue;

			bool overlap = false;

			for (int j = 0; j < spans.Count; j++)
			{
				/*	If they are overlaid, combine them. */

				if (!(span.Start > spans[j].End
					|| span.End < spans[j].Start))
				{
					spans[j].Start = min(spans[j].Start, span.Start);
					spans[j].End = max(spans[j].End, span.End);

					overlap = true;

					/*	This one check here is enough because
						every new span is tested against already
						added spans, so they will add correctly. */

					break;
				}
			}

			/*	If the span doesn't overlap any 
				existing spans, add it directly. */

			if (!overlap)
				spans.Add(span);
		}
	}
}

Region Region::Null;

//

Span::Span()
{

}

Span::Span(const Span& span)
{
	Start = span.Start;
	End = span.End;
}

Span::Span(int start, int end)
{
	Start = start;
	End = end;
}

Span& Span::operator = (const Span& span)
{
	Start = span.Start;
	End = span.End;

	return *this;
}

bool Span::operator == (const Span& span)
{
	return Start == span.Start 
		&& End == span.End;
}