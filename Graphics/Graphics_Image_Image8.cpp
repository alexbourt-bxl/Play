#include "Graphics.h"

#include <Types/Geometric\Vector3.h>
#include <Types/Geometric\Matrix33.h>

void Graphics::DrawImage_Image8(int* dstBuffer,
								int dstBufferWidth, int dstBufferHeight,
								int xDst, int yDst, 
								int wDst, int hDst, 
								int* srcBuffer,
								int srcBufferWidth, int srcBufferHeight,
								int xSrc, int ySrc, 
								int wSrc, int hSrc, 
								int opacity)
{
	if (wDst == wSrc && hDst == hSrc)
	{
		DrawImage_Image8(
			dstBuffer,
			dstBufferWidth, dstBufferHeight,
			xDst, yDst,
			srcBuffer,
			srcBufferWidth, srcBufferHeight,
			xSrc, ySrc,
			wSrc, hSrc, 
			opacity);
	}
	else if (m_interpolation == Nearest)
	{
		DrawImage_Image8_Nearest(
			dstBuffer,
			dstBufferWidth, dstBufferHeight,
			xDst, yDst, 
			wDst, hDst, 
			srcBuffer,
			srcBufferWidth, srcBufferHeight,
			xSrc, ySrc, 
			wSrc, hSrc, 
			opacity);
	}
	else if (m_interpolation == Bilinear)
	{
		DrawImage_Image8_Bilinear(
			dstBuffer,
			dstBufferWidth, dstBufferHeight,
			xDst, yDst, 
			wDst, hDst, 
			srcBuffer,
			srcBufferWidth, srcBufferHeight,
			xSrc, ySrc, 
			wSrc, hSrc, 
			opacity);
	}
}

void Graphics::DrawImage_Image8(int* dstBuffer,
								int dstBufferWidth, int dstBufferHeight,
								int xDst, int yDst, 
								int* srcBuffer,
								int srcBufferWidth, int srcBufferHeight,
								int xSrc, int ySrc, 
								int wSrc, int hSrc, 
								int opacity)
{
	int xStart = max(0, xDst);
	int xEnd = min(xDst + wSrc, dstBufferWidth);

	int yStart = max(0, yDst);
	int yEnd = min(yDst + hSrc, dstBufferHeight);

	//Region clip = m_clipRgn;
	//clip.Clip(Rect(xDst, yDst, wSrc, hSrc));

	//if (clip)
	//{
	//	yStart = max(yStart, clip.Bounds.Top);
	//	yEnd = min(clip.Bounds.Bottom, yEnd);
	//}
	//else
	//{
	//	clip.Add(Rect(
	//		xDst,
	//		yDst,
	//		wSrc,
	//		hSrc));
	//}

	//Array<Span> spans;

	for (int y = yStart; y < yEnd; y++)
	{
		//clip.GetSpans(y, spans);

		int yWidthDst = y * dstBufferWidth;
		int yWidthSrc = (y - yDst + ySrc) * srcBufferWidth;

		//for (int i = 0; i < spans.Count; i++)
		//{
			//for (int x = spans[i].Start; x < spans[i].End; x++)
			for (int x = xStart; x < xEnd; x++)
				dstBuffer[yWidthDst + x] = Color8::Over(dstBuffer[yWidthDst + x], srcBuffer[yWidthSrc + x - xDst + xSrc], opacity);
		//}
	}
}

void Graphics::DrawImage_Image8_Nearest(int* dstBuffer,
										int dstBufferWidth, int dstBufferHeight,
										int xDst, int yDst, 
										int wDst, int hDst, 
										int* srcBuffer,
										int srcBufferWidth, int srcBufferHeight,
										int xSrc, int ySrc, 
										int wSrc, int hSrc, 
										int opacity)
{
	Point spScale = GetSubpixelScale();

	int xStart = max(0, xDst);
	int xEnd = min(xDst + wDst, dstBufferWidth);

	int yStart = max(0, yDst);
	int yEnd = min(yDst + hDst, dstBufferHeight);

	double xScale = (double)(wSrc - 1) / ((double)wDst * (double)spScale.X);
	double yScale = (double)(hSrc - 1) / ((double)hDst * (double)spScale.Y);

	int yWidthDst, yWidthSrc;
	double x_cur;

	Array<Span> spans;

	for (int y = yStart * spScale.Y; y < yEnd * spScale.Y; y++)
	{
		yWidthDst = y * dstBufferWidth;
		yWidthSrc = ((int)((double)(y - yDst) * yScale + 0.5) + ySrc) * srcBufferWidth;

		x_cur = (double)xSrc + (double)((xStart - xDst) * spScale.X) * xScale;

		int* scanline = SetCurrentScanline(y);
		int col;

		for (int x = xStart * spScale.X; x < xEnd * spScale.X; x++)
		{
			col = Color8::MultiplyScalar(srcBuffer[yWidthSrc + (int)(x_cur + 0.5)], opacity);
			scanline[x] = col;
			x_cur += xScale;
		}

		PlotScanline(xStart, xEnd);
	}
}

void Graphics::DrawImage_Image8_Bilinear(int* dstBuffer,
										 int dstBufferWidth, int dstBufferHeight,
										 int xDst, int yDst, 
										 int wDst, int hDst, 
										 int* srcBuffer,
										 int srcBufferWidth, int srcBufferHeight,
										 int xSrc, int ySrc, 
										 int wSrc, int hSrc, 
										 int opacity)
{
	Point spScale = GetSubpixelScale();

	int xStart = max(0, xDst);
	int xEnd = min(xDst + wDst, dstBufferWidth);

	int yStart = max(0, yDst);
	int yEnd = min(yDst + hDst, dstBufferHeight);

	double xScale = (double)(wSrc - 1) /  ((double)wDst * (double)spScale.X);
	double yScale = (double)(hSrc - 1) /  ((double)hDst * (double)spScale.Y);

	int x0, y0;
	double y_, x_;
	int dx, dy, _dx, _dy;
	int dxdy, _dxdy, dx_dy, _dx_dy;
	int col00, col01, col10, col11;
	int index;
	int p, q, r, t;

	int yWidthSrc;
	int yWidthDst;

	Array<Span> spans;

	for (int y = yStart * spScale.Y; y < yEnd * spScale.Y; y++)
	{
		y_ = (double)(y - yDst) * yScale;
		y0 = (int)y_;
		dy = Math::ToInt8(y_ - (double)y0);
		_dy = 0xFF - dy;

		yWidthDst = y * dstBufferWidth;
		yWidthSrc = (y0 + ySrc) * srcBufferWidth;

		int* scanline = SetCurrentScanline(y);
		int col;

		for (int x = xStart * spScale.X; x < xEnd * spScale.X; x++)
		{
			x_ = (double)(x - xDst * spScale.X) * xScale;
			x0 = (int)x_; 
			dx = Math::ToInt8(x_ - (double)x0);
			_dx = 0xFF - dx;

			index = yWidthSrc + x0 + xSrc;

			col00 = srcBuffer[index];
			col01 = srcBuffer[index + 1];
			col10 = srcBuffer[index + srcBufferWidth];
			col11 = srcBuffer[index + srcBufferWidth + 1];

			dxdy = INT_MULT(dx, dy, t);
			_dxdy = INT_MULT(_dx, dy, t);
			dx_dy = INT_MULT(dx, _dy, t);
			_dx_dy = INT_MULT(_dx, _dy, t);

			col = Color8::Add(
				Color8::Add(Color8::MultiplyScalar(col00, _dx_dy), Color8::MultiplyScalar(col01, dx_dy)),
				Color8::Add(Color8::MultiplyScalar(col10, _dxdy), Color8::MultiplyScalar(col11, dxdy)));

			col = Color8::MultiplyScalar(col, opacity);
			scanline[x] = col;
		}

		PlotScanline(xStart, xEnd);

		// BUG: fix the scaling problem (0.5 pixel offset or something like that, compare with x2 scaling in Photoshop and GDI)
	}
}

void Graphics::DrawImage_Image8(int* dstBuffer,
								int dstBufferWidth, int dstBufferHeight,
								const FPoint& p0, const FPoint& p1, const FPoint& p2, const FPoint& p3,
								int* srcBuffer, 
								int srcBufferWidth, int srcBufferHeight, 
								int opacity)
{
	Matrix33 mapping;
	
	if (!Math::GetProjectiveMapping(p0, p1, p2, p3, &mapping))
		return;

	if (mapping.Determinant == 0)
		return;

	Matrix33 invMapping = mapping.Inverse;

	// Get polygon bounds

	double xMin = min(min(p0.X, p1.X), min(p2.X, p3.X));
	double yMin = min(min(p0.Y, p1.Y), min(p2.Y, p3.Y));

	int xStart = max(0, (int)xMin);
	int xEnd   = min((int)(max(max(p0.X, p1.X), max(p2.X, p3.X))), dstBufferWidth);
	int yStart = max(0, (int)yMin);
	int yEnd   = min((int)(max(max(p0.Y, p1.Y), max(p2.Y, p3.Y))), dstBufferHeight);

	// Draw the image

	double float_srcWidth  = (double)srcBufferWidth;
	double float_srcHeight = (double)srcBufferHeight;

	//

	if (m_interpolation == Nearest) 
	{
		DrawImage_Image8_Nearest(
			dstBuffer,
			dstBufferWidth, dstBufferHeight,
			invMapping, 
			xStart, xEnd, 
			yStart, yEnd, 
			xMin, yMin, 
			srcBuffer, 
			srcBufferWidth, srcBufferHeight, 
			float_srcWidth, float_srcHeight, 
			opacity);
	}
	else if (m_interpolation == Bilinear)  
	{
		DrawImage_Image8_Bilinear(
			dstBuffer,
			dstBufferWidth, dstBufferHeight,
			invMapping, 
			xStart, xEnd, 
			yStart, yEnd, 
			xMin, yMin, 
			srcBuffer, 
			srcBufferWidth, srcBufferHeight, 
			float_srcWidth, float_srcHeight, 
			opacity);
	}
}

void Graphics::DrawImage_Image8_Nearest(int* dstBuffer,
										int dstBufferWidth, int dstBufferHeight,
										Matrix33& invMapping, 
										int xStart, int xEnd, 
										int yStart, int yEnd, 
										double xMin, double yMin, 
										int* srcBuffer, 
										int srcBufferWidth, int srcBufferHeight, 
										double float_srcWidth, double float_srcHeight, 
										int opacity)
{
	Vector3 v;
	int x0, y0;
	int col;
	int yWidthDst;

	for (int y = yStart; y < yEnd; y++)
	{
		yWidthDst = y * dstBufferWidth;

		for (int x = xStart; x < xEnd; x++)
		{
			v.X = (double)x;// - xMin;
			v.Y = (double)y;// - yMin;
			v.Z = 1.0;

			v *= invMapping;
			
			v.X *= float_srcWidth;
			v.Y *= float_srcHeight;

			x0 = (int)v.X;
			y0 = (int)v.Y;

			if (x0 >= 0 && y0 >= 0 && x0 < srcBufferWidth && y0 < srcBufferHeight)
			{
				col = srcBuffer[y0 * srcBufferWidth + x0];
				dstBuffer[yWidthDst + x] = Color8::Over(dstBuffer[yWidthDst + x], col, opacity);
			}
		}
	}
}

void Graphics::DrawImage_Image8_Bilinear(int* dstBuffer,
										 int dstBufferWidth, int dstBufferHeight,
										 Matrix33& invMapping, 
										 int xStart, int xEnd, 
										 int yStart, int yEnd,
										 double xMin, double yMin, 
										 int* srcBuffer, 
										 int srcBufferWidth, int srcBufferHeight, 
										 double float_srcWidth, double float_srcHeight, 
										 int opacity)
{
	Vector3 v;
	int x0, x1, y0, y1;
	int dx, dy;
	int col00, col01, col10, col11;
	int p, q, r;
	int yWidthDst;

	for (int y = yStart; y < yEnd; y++)
	{
		yWidthDst = y * dstBufferWidth;

		for (int x = xStart; x < xEnd; x++)
		{
			v.X = (double)x;// - xMin;
			v.Y = (double)y;// - yMin;
			v.Z = 1.0;

			v *= invMapping;

			v.X *= float_srcWidth;
			v.Y *= float_srcHeight;

			x0 = (int)v.X;
			x1 = min(x0 + 1, srcBufferWidth - 1);
			dx = Math::ToInt8(v.X - (double)x0);

			y0 = (int)v.Y;
			y1 = min(y0 + 1, srcBufferHeight - 1);
			dy = Math::ToInt8(v.Y - (double)y0);

			if (x0 >= 0 && y0 >= 0 && x0 < srcBufferWidth && y0 < srcBufferHeight)
			{
				col00 = srcBuffer[y0 * srcBufferWidth + x0];
				col10 = srcBuffer[y0 * srcBufferWidth + x1];
				col01 = srcBuffer[y1 * srcBufferWidth + x0];
				col11 = srcBuffer[y1 * srcBufferWidth + x1];

				p = Color8::Lerp(col00, col10, dx);
				q = Color8::Lerp(col01, col11, dx);
				r = Color8::Lerp(p, q, dy);

				dstBuffer[yWidthDst + x] = Color8::Over(dstBuffer[yWidthDst + x], r, opacity);
			}
		}
	}
}

void Graphics::CopyImage_Image8(int* dstBuffer,
								int dstBufferWidth, int dstBufferHeight,
								int xDst, int yDst, 
								int* srcBuffer, 
								int srcBufferWidth, int srcBufferHeight,
								int xSrc, int ySrc, 
								int wSrc, int hSrc)
{
	int yStart = max(0, yDst);
	int yEnd = min(yDst + hSrc, dstBufferHeight);

	Region clip = m_clipRgn;
	clip.Clip(Rect(xDst, yDst, wSrc, hSrc));

	if (clip)
	{
		yStart = max(yStart, clip.Bounds.Top);
		yEnd = min(clip.Bounds.Bottom, yEnd);
	}
	else
	{
		clip.Add(Rect(xDst, yDst, wSrc, hSrc));
	}

	Array<Span> spans;

	for (int y = yStart; y < yEnd; y++)
	{
		clip.GetSpans(y, spans);

		int yWidthDst = y * dstBufferWidth;
		int yWidthSrc = (y - yDst + ySrc) * srcBufferWidth;

		for (int i = 0; i < spans.Count; i++)
		{
			for (int x = spans[i].Start; x < spans[i].End; x++)
				dstBuffer[yWidthDst + x] = srcBuffer[yWidthSrc + x - xDst + xSrc];
		}
	}
}