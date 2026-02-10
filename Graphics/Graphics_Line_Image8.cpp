#include "Graphics.h"

void Graphics::DrawLine_Image8(int* buffer,
							   int width, int height,
							   double x1, double y1, 
							   double x2, double y2, 
							   Color8& color)
{
	if (m_antialias != Graphics::None)
	{
		DrawLine_Image8_Antialias(
			buffer,
			width, height,
			x1, y1, 
			x2, y2, 
			color);
	}
	else
	{
		DrawLine_Image8_NoAntialias(
			buffer,
			width, height,
			x1, y1, 
			x2, y2, 
			color);
	}
}

void Graphics::DrawLine_Image8_Antialias(int* buffer,
										 int bufferWidth, int bufferHeight,
										 double x1, double y1, 
										 double x2, double y2, 
										 Color8& color)
{
	// TODO: go through subpixel filter
	// TODO: find a way to do polyline drawing with overlapping edge pixels at 100% combined opacity

	/*
		Draws an antialiased line using Xiaolin Wu's algorithm.
	*/

	/*	To match the polygon filling code, the coordinates 
		have to be in the middle of the pixel. */
	x1 -= 0.5;
	x2 -= 0.5;
	y1 -= 0.5;
	y2 -= 0.5;

	//int top    = max(0, (int)min(y1, y2));
	//int bottom = min((int)(max(y1, y2) + 0.999999), bufferHeight);

	//int left   = max(0, (int)min(x1, x2));
	//int right  = min((int)(max(x1, x2) + 0.999999), bufferWidth);

	// Set up clip region

	//Region clip = m_clipRgn;

	//if (clip)
	//{
	//	top    = max(top, clip.Bounds.Top);
	//	bottom = min(clip.Bounds.Bottom, bottom);

	//	left   = max(left, clip.Bounds.Left);
	//	right  = min(clip.Bounds.Right, right);
	//}
	//else
	//{
	//	clip.Add(Rect(
	//		left,
	//		top,
	//		right - left,
	//		bottom - top));
	//}

	if (!ClipLine(&x1, &y1, &x2, &y2, 0, 0, bufferWidth, bufferHeight))
		return;

	int col = Color8::Premult(color);

	// Check line orientation

	double dx = x2 - x1;
	double dy = y2 - y1;

	if (fabs(dx) > fabs(dy))
	{ 
		if (x1 > x2) 
		{ 
			SWAPF(x1, x2); 
			SWAPF(y1, y2); 
		}

		double gradient = dy / dx;

		double xend, yend, xgap, iy;
		int ixstart, iystart, ixend, iyend;
		int col_op;

		// Handle first endpoint

		xend = FROUND(x1);
		yend = y1 + gradient * (xend - x1);
		xgap = RFPART(x1 + 0.5);
		ixstart = (int)xend;
		iystart = IPART(yend);

		col_op = Color8::MultiplyScalar(col, Math::ToInt8(RFPART(yend) * xgap));
		int* pixel = buffer + iystart * bufferWidth + ixstart;
		*pixel = Color8::Over(*pixel, col_op);
		
		if (iystart + 1 < bufferHeight)
		{
			col_op = Color8::MultiplyScalar(col, Math::ToInt8(FPART(yend) * xgap));
			pixel += bufferWidth; // y += 1
			*pixel = Color8::Over(*pixel, col_op);
		}

		iy = yend + gradient; // first y-intersection for the line

		// Handle second endpoint

		xend = FROUND(x2);
		yend = y2 + gradient * (xend - x2);
		xgap = FPART(x2 + 0.5);
		ixend = (int)xend;
		iyend = IPART(yend);

		col_op = Color8::MultiplyScalar(col, Math::ToInt8(RFPART(yend) * xgap));
		pixel = buffer + iyend * bufferWidth + ixend;
		*pixel = Color8::Over(*pixel, col_op);

		if (iyend + 1 < bufferHeight)
		{
			col_op = Color8::MultiplyScalar(col, Math::ToInt8(FPART(yend) * xgap));
			pixel += bufferWidth; // y += 1
			*pixel = Color8::Over(*pixel, col_op);
		}

		// Draw the line

		for (int x = ixstart + 1; x < ixend; x++)
		{
			col_op = Color8::MultiplyScalar(col, Math::ToInt8(RFPART(iy)));
			pixel = buffer + IPART(iy) * bufferWidth + x;
			*pixel = Color8::Over(*pixel, col_op);

			col_op = Color8::MultiplyScalar(col, Math::ToInt8(FPART(iy)));
			pixel += bufferWidth; // y += 1
			*pixel = Color8::Over(*pixel, col_op);

			iy += gradient;
		}
	}
	else
	{
		if (y1 > y2) 
		{ 
			SWAPF(x1, x2); 
			SWAPF(y1, y2); 
		}

		double gradient = dx / dy;

		double xend, yend, ygap, ix;
		int ixstart, iystart, ixend, iyend;
		int col_op;

		// Handle first endpoint

		yend = FROUND(y1);
		xend = x1 + gradient * (yend - y1);
		ygap = RFPART(y1 + 0.5);
		iystart = (int)yend;
		ixstart = IPART(xend);

		col_op = Color8::MultiplyScalar(col, Math::ToInt8(RFPART(xend) * ygap));
		int* pixel = buffer + iystart * bufferWidth + ixstart;
		*pixel = Color8::Over(*pixel, col_op);

		if (ixstart + 1 < bufferWidth)
		{
			col_op = Color8::MultiplyScalar(col, Math::ToInt8(FPART(xend) * ygap));
			pixel += 1; // x += 1
			*pixel = Color8::Over(*pixel, col_op);
		}

		ix = xend + gradient; // first y-intersection for the line

		// Handle second endpoint

		yend = FROUND(y2);
		xend = x2 + gradient * (yend - y2);
		ygap = FPART(y2 + 0.5);
		iyend = (int)yend;
		ixend = IPART(xend);

		col_op = Color8::MultiplyScalar(col, Math::ToInt8(RFPART(xend) * ygap));
		pixel = buffer + iyend * bufferWidth + ixend;
		*pixel = Color8::Over(*pixel, col_op);

		if (ixend + 1 < bufferWidth)
		{
			col_op = Color8::MultiplyScalar(col, Math::ToInt8(FPART(xend) * ygap));
			pixel += 1; // x += 1
			*pixel = Color8::Over(*pixel, col_op);
		}

		// Draw the line

		for (int y = iystart + 1; y < iyend; y++)
		{
			col_op = Color8::MultiplyScalar(col, Math::ToInt8(RFPART(ix)));
			pixel = buffer + y * bufferWidth + IPART(ix);
			*pixel = Color8::Over(*pixel, col_op);

			col_op = Color8::MultiplyScalar(col, Math::ToInt8(FPART(ix)));
			pixel += 1; // x += 1
			*pixel = Color8::Over(*pixel, col_op);

			ix += gradient;
		}
	}
}

void Graphics::DrawLine_Image8_NoAntialias(int* buffer,
										   int width, int height,
										   double x1, double y1, 
										   double x2, double y2, 
										   Color8& color)
{
	/*
		Draws a line using a modified Bresenham algorithm (with sub-pixel accuracy).
	*/

	int col = Color8::Premult(color);

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

	/*	TODO: This lacks proper subpixel positioning
		and it seems the error needs to take the subpixel position into account.
		Right now when drawing bezier curves they are joined weird, not smooth at all.
		
		Maybe the error and ystep and idy need to be more precise or something... */

	int error = idx / 2;

	int y = (int)y1;
	int ystep = y1 < y2 ? 1 : -1;
	
	int ix1 = (int)x1;
	int ix2 = (int)x2;

	for (int x = ix1; x <= ix2; x++)
	{
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