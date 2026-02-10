#include "OpComposite.h"

#include <Types\Graphics\Image8.h>
#include <Types\Graphics\GdiImage8.h>

#include <assert.h>

void OpComposite::Composite_Image8(int* outBuffer,
								   int outBufferWidth, int outBufferHeight,
								   int outX, int outY,
								   //
								   int* inBuffer,
								   int inBufferWidth, int inBufferHeight,
								   int inX, int inY,
								   int inWidth, int inHeight,
								   //
								   int opacity,
								   Region& _clip)
{
	/*	Determine comp region. */

	Rect outRect(
		outX, 
		outY, 
		inWidth, 
		inHeight);

	outRect.Clip(Rect(
		0,
		0,
		outBufferWidth,
		outBufferHeight));

	Region clip = _clip;
	clip.Clip(outRect);

	int yStart = outRect.Top;			
	int yEnd = outRect.Bottom;	

	if (clip)
	{
		yStart = max(yStart, clip.Bounds.Top);
		yEnd = min(clip.Bounds.Bottom, yEnd);
	}

	/*	Composite the image. */

	for (int y = yStart, yIn = inY; y < yEnd; y++, yIn++)
	{
		Array<Span> spans = clip.GetSpans(y);

		if (clip.IsNull)
			spans.Add(Span(outRect.Left, outRect.Right));

		int yWidthOut = y * outBufferWidth;
		int yWidthIn = (y - outRect.Y + inY) * inBufferWidth;

		for (int i = 0; i < spans.Count; i++)
		{
			if (opacity == 0xFF)
			{
				int cout, cin;

				for (int x = spans[i].Start, xIn = inX; x < spans[i].End; x++, xIn++)
				{
					cin = inBuffer[yWidthIn + xIn];
					cout = outBuffer[yWidthOut + x];
					outBuffer[yWidthOut + x] = Color::Over8(cout, cin);
				}
			}
			else
			{
				int cout, cin;

				for (int x = spans[i].Start, xIn = inX; x < spans[i].End; x++, xIn++)
				{
					cin = inBuffer[yWidthIn + xIn];
					cout = outBuffer[yWidthOut + x];
					outBuffer[yWidthOut + x] = Color::Over8(cout, cin, opacity);
				}
			}
		}

		UpdateProgress((float)(y - yStart) / (float)(yEnd - yStart));
	}
}