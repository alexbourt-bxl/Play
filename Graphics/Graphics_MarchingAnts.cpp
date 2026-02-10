#include "Graphics.h"

#include <Types/Graphics\Image8.h>
#include <Types/Graphics\GdiImage8.h>
#include <Types/Geometric\Poly.h>

void Graphics::DrawMALine(const FPoint& p1, const FPoint& p2, 
						  const Color& _color1, const Color& _color2, 
						  int& step)
{
	if (typeof(m_image) == TYPEOF(Image8))
	{
		Color8 color1 = (Color)_color1;
		Color8 color2 = (Color)_color2;

		DrawMALine_Image8(
			(int*)((Image8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			p1.X, p1.Y, 
			p2.X, p2.Y,
			color1, color2,
			step);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage8))
	{
		Color8 color1 = (Color)_color1;
		Color8 color2 = (Color)_color2;

		DrawMALine_Image8(
			(int*)((GdiImage8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			p1.X, p1.Y, 
			p2.X, p2.Y,
			color1, color2,
			step);
	}
}

void Graphics::DrawMALine_Image8(int* buffer,
								 int width, int height, 
								 double x1, double y1,
								 double x2, double y2, 
								 Color8& color1, Color8& color2,
								 int& step)
{
	/*
		Draws a line using a modified Bresenham algorithm (with sub-pixel accuracy).
	*/

	int col1 = Color8::Premult(color1);
	int col2 = Color8::Premult(color2);

	// Clip the line

	if (!ClipLine(&x1, &y1, &x2, &y2, 0, 0, width, height))
		return;

	// Check line orientation

	double dx = x2 - x1;
	double dy = y2 - y1;

	bool steep = fabs(dy) > fabs(dx);

	if (steep)	 
	{ 
		SWAPF(x1, y1); 
		SWAPF(x2, y2); 
	}
	
	if (x1 > x2)
	{ 
		SWAPF(x1, x2); 
		SWAPF(y1, y2); 
	}

	int idx = (int)(fabs(x2 - x1) * 255);
	int idy = (int)(fabs(y2 - y1) * 255);

	int error = idx / 2;

	int y = (int)y1;
	int ystep = y1 < y2 ? 1 : -1;
	
	int ix1 = (int)x1;
	int ix2 = (int)x2;

	int col;
	int limit = 4;

	for (int x = ix1; x <= ix2; x++)
	{
		col = 
			((step++ / limit) % 2) > 0
			? col1
			: col2;

		if (steep) 
		{
			int* pixel = buffer + x * width + y;
			*pixel = Color8::Over(*pixel, col);
		}
		else
		{
			int* pixel = buffer + y * width + x;
			*pixel = Color8::Over(*pixel, col);
		}

		error -= idy;

		if (error < 0)
		{
			y += ystep;
			error += idx;
		}
	}
}

void Graphics::DrawMAPolyLine(Poly& poly, const Color& color1, const Color& color2)
{
	int step = 0;

	for (int i = 0; i < poly.Points.Count - 1; i++)
	{
		DrawMALine(
			poly.Points[i], poly.Points[i + 1], 
			color1, color2,
			step);
	}
}

void Graphics::DrawMAPoly(Poly& poly, const Color& color1, const Color& color2)
{
	int step = 0;

	for (int i = poly.Points.Count - 1, j = 0; j < poly.Points.Count; i = j++)
	{
		DrawMALine(
			poly.Points[i], poly.Points[j], 
			color1, color2,
			step);
	}
}