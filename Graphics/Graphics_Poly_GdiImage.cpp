#include "Graphics.h"

#include <Native\Gdi.h>
#include <Types/Geometric\Poly.h>
#include <Types/Geometric\FRect.h>

void Graphics::FillPoly_GdiImage(HDC hdc,
								 Poly* poly, 
								 int nPolys,
								 Color8& color)
{
	if (color.A == 0)
		return;

	HRGN hrgnClip = null;
	
	if (m_clipRgn)
	{
		hrgnClip = Gdi::ToHRGN(m_clipRgn);
		::SelectClipRgn(hdc, hrgnClip);
	}

	//

	Rect bounds;

	for (int i = 0; i < nPolys; i++)
	{
		FRect _bounds = poly[i].Bounds;

		bounds.Expand(Rect(
			(int)_bounds.X,
			(int)_bounds.Y,
			(int)_bounds.Width + 1,
			(int)_bounds.Height + 1));
	}
	
	int rgb = color.ToGdi() & 0x00FFFFFF; // GDI doesn't handle the alpha byte correctly

	_PreOpacity_Gdi(hdc, &bounds, (byte)color.A);

	HPEN hp      = ::CreatePen(PS_SOLID, 1, rgb);
	HPEN hpOld   = (HPEN)::SelectObject(hdc, hp);
	HBRUSH hb    = ::CreateSolidBrush(rgb);
	HBRUSH hbOld = (HBRUSH)::SelectObject(hdc, hb);

	Array<POINT> points;
	Array<int> nPoints;

	for(int i = 0; i < nPolys; i++)
	{
		for (int j = 0; j < poly[i].Points.Count; j++)
		{
			POINT p = {
				(int)poly[i].Points[j].X,
				(int)poly[i].Points[j].Y };

			points.Add(p);
		}

		nPoints.Add(poly[i].Points.Count);
	}

	::SetPolyFillMode(hdc, m_nonZeroFill ? WINDING : ALTERNATE);

	::PolyPolygon(
		hdc, 
		points,
		nPoints,
		nPolys);
	
	::DeleteObject(::SelectObject(hdc, hpOld));
	::DeleteObject(::SelectObject(hdc, hbOld));

	_PostOpacity_Gdi(hdc, &bounds, (byte)color.A);

	//

	if (hrgnClip)
	{
		::SelectClipRgn(hdc, null);
		::DeleteObject(hrgnClip);
	}
}