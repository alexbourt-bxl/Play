#include "Graphics.h"

#include <Types/Graphics\Image8.h>
#include <Types/Graphics\GdiImage.h>
#include <Types/Graphics\GdiImage8.h>
#include <Types/Geometric\Poly.h>
#include <Types/Geometric\Shape.h>

void Graphics::DrawLine(double x1, double y1, double x2, double y2, const Color& color, double width)
{
	if (width <= 1)
	{
		Color _color((Color&)color, color.A * width);

		if (typeof(m_image) == TYPEOF(Image8))
		{
			Color8 color8 = _color;

			DrawLine_Image8(
				(int*)((Image8*)m_image)->Buffer,
				m_image->Width, m_image->Height,
				x1, y1, 
				x2, y2, 
				color8);
		}
		else if (typeof(m_image) == TYPEOF(GdiImage8))
		{
			Color8 color8 = _color;

			DrawLine_Image8(
				(int*)((GdiImage8*)m_image)->Buffer,
				m_image->Width, m_image->Height,
				x1, y1, 
				x2, y2, 
				color8);
		}
		else if (typeof(m_image) == TYPEOF(GdiImage))
		{
			Color8 color8 = _color;

			DrawLine_GdiImage(
				((GdiImage*)m_image)->Hdc,
				(int)x1, (int)y1,
				(int)x2, (int)y2, 
				color8);
		}
	}
	else
	{
		double dx = x2 - x1;
		double dy = y2 - y1;
		double _1_d = Math::InvSqrt(dx*dx + dy*dy);

		double fx = dx * _1_d;
		double fy = dy * _1_d;

		double tfx_2 = width * fy / 2;
		double tfy_2 = width * fx / 2;

		FPoint p0(x1 + tfx_2, y1 - tfy_2);
		FPoint p1(x2 + tfx_2, y2 - tfy_2);
		FPoint p2(x2 - tfx_2, y2 + tfy_2);
		FPoint p3(x1 - tfx_2, y1 + tfy_2);

		Poly poly;

		poly.Points.Add(p0);
		poly.Points.Add(p1);
		poly.Points.Add(p2);
		poly.Points.Add(p3);

		FillPoly(&poly, 1, color);
	}
}

void Graphics::DrawLine(const FPoint& p1, const FPoint& p2, const Color& color, double width)
{
	DrawLine(p1.X, p1.Y, p2.X, p2.Y, color, width);
}

void Graphics::DrawLine(int x1, int y1, int x2, int y2, const Color& color, double width)
{
	double dx = 0;
	double dy = 0;

	if (abs(x2 - x1) > abs(y2 - y1))
	{	
		dx = 0;
		dy = 0.5;
	}
	else if (abs(x2 - x1) < abs(y2 - y1))
	{
		dx = 0.5;
		dy = 0;
	}

	DrawLine(
		(double)x1 + dx, 
		(double)y1 + dy, 
		(double)x2 + dx, 
		(double)y2 + dy, 
		color, 
		width);
}

void Graphics::DrawLine(const Point& p1, const Point& p2, const Color& color, double width)
{
	DrawLine(p1.X, p1.Y, p2.X, p2.Y, color, width);
}

bool Graphics::ClipLine(double* x1, double* y1, double* x2, double* y2, int left, int top, int right, int bottom)
{
	/*
		Clip the line to the rectangle using the Liang-Barsky algorithm.
	
		Return:	true if the line intersects the rectangle.
				false if the line does not intersect the rectangle.
	*/

	double t0 = 0;
	double t1 = 1;

	double dx = *x2 - *x1;
	double dy = *y2 - *y1;

	if (!CheckClipEdge(-dx, -((double)left - *x1), &t0, &t1)) 
		return false;
	
	if (!CheckClipEdge(dx, (double)(right - 1) - *x1, &t0, &t1)) 
		return false;
	
	if (!CheckClipEdge(-dy, -((double)top - *y1), &t0, &t1)) 
		return false;
	
	if (!CheckClipEdge(dy, (double)(bottom - 1) - *y1, &t0, &t1)) 
		return false;

	if (t1 < 1)
	{
		*x2 = *x1 + t1 * dx;
		*y2 = *y1 + t1 * dy;
	}

	if (t0 > 0)
	{
		*x1 = *x1 + t0 * dx;
		*y1 = *y1 + t0 * dy;
	}

	return true;
}

inline bool Graphics::CheckClipEdge(double p, double q, double* t0, double* t1)
{
	double r = q / p;

	if (p == 0 && q < 0)
	{
		return false;
	}
	else if (p < 0)
	{
		if (r > *t1) return false;
		else if (r > *t0) *t0 = r;
	}
	else if (p > 0)
	{
		if (r < *t0) return false;
		else if (r < *t1) *t1 = r;
	}

	return true;
}