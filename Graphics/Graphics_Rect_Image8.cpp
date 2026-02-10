#include "Graphics.h"

void Graphics::DrawRect_Image8(int* buffer,
							   int bufferWidth, int bufferHeight,
							   int x, int y, 
							   int width, int height, 
							   Color8& color)
{
	DrawLine_Image8_NoAntialias(
		buffer,
		bufferWidth, bufferHeight,
		(double)x, (double)y, 
		(double)(x + width), (double)y, 
		color);
	
	DrawLine_Image8_NoAntialias(
		buffer,
		bufferWidth, bufferHeight,
		(double)x, (double)(y + height), 
		(double)(x + width), (double)(y + height), 
		color);
	
	DrawLine_Image8_NoAntialias(
		buffer,
		bufferWidth, bufferHeight,
		(double)x, (double)(y + 1), 
		(double)x, (double)(y + height - 1), 
		color);
	
	DrawLine_Image8_NoAntialias(
		buffer,
		bufferWidth, bufferHeight,
		(double)(x + width), (double)(y + 1), 
		(double)(x + width), (double)(y + height - 1), 
		color);
}

void Graphics::FillRect_Image8(int* buffer,
							   int bufferWidth, int bufferHeight,
							   int _x, int _y, 
							   int width, int height, 
							   Color8& color)
{
	int col = Color8::Premult(color);
	int* pixel;

	int yStart = max(0, _y);
	int yEnd = min(_y + height, bufferHeight);

	Region clip = m_clipRgn;
	clip.Clip(Rect(0, 0, bufferWidth, bufferHeight));

	if (clip)
	{
		yStart = max(yStart, clip.Bounds.Top);
		yEnd = min(clip.Bounds.Bottom, yEnd);
	}
	else
	{
		clip.Add(Rect(
			max(0, _x),
			yStart,
			min(_x + width, bufferWidth),
			yEnd - yStart));
	}

	Array<Span> spans;

	for (int y = yStart; y < yEnd; y++)
	{
		clip.GetSpans(y, spans);

		//if (clip.IsNull)
		//{
		//	spans.Add(Span(
		//		max(0, _x), 
		//		min(_x + width, bufferWidth)));
		//}

		int yWidth = y * bufferWidth;

		for (int i = 0; i < spans.Count; i++)
		{
			if (color.A == 0xFF)
			{
				for (int x = spans[i].Start; x < spans[i].End; x++)
					buffer[yWidth + x] = col;
			}
			else
			{
				for (int x = spans[i].Start; x < spans[i].End; x++)
				{
					pixel = buffer + yWidth + x;
					*pixel = Color8::Over(*pixel, col);
				}
			}
		}
	}
}

void Graphics::ClearRect_Image8(int* buffer,
								int bufferWidth, int bufferHeight,
								int _x, int _y, 
								int width, int height)
{
	int yStart = max(0, _y);
	int yEnd = min(_y + height, bufferHeight);

	Region clip = m_clipRgn;
	clip.Clip(Rect(_x, _y, width, height));

	if (clip)
	{
		yStart = max(yStart, clip.Bounds.Top);
		yEnd = min(clip.Bounds.Bottom, yEnd);
	}
	else
	{
		clip.Add(Rect(
			max(0, _x),
			yStart,
			min(_x + width, bufferWidth),
			yEnd - yStart));
	}

	Array<Span> spans;

	for (int y = yStart; y < yEnd; y++)
	{
		clip.GetSpans(y, spans);

		//if (clip.IsNull)
		//{
		//	spans.Add(Span(
		//		max(0, _x), 
		//		min(_x + width, bufferWidth)));
		//}

		int yWidth = y * bufferWidth;

		for (int i = 0; i < spans.Count; i++)
		{
			for (int x = spans[i].Start; x < spans[i].End; x++)
				buffer[yWidth + x] = 0;
		}
	}
}