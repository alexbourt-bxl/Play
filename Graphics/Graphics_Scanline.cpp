#include "Graphics.h"

#include <Types/Graphics\Image8.h>
#include <Types/Graphics\GdiImage8.h>
#include <Types/Graphics\GdiImage.h>

void Graphics::UpdateScanlineBuffer()
{
	if (m_scanline)
	{
		free(m_scanline);
		m_scanline = null;
	}

	if (m_scanlineF)
	{
		free(m_scanlineF);
		m_scanlineF = null;
	}

	if (m_spBufferLines)
	{
		free(m_spBufferLines);
		m_spBufferLines = null;
	}

	__Type* type = null;

	if (typeof(m_image) == TYPEOF(GdiImage)
		|| typeof(m_image) == TYPEOF(GdiImage8)
		|| typeof(m_image) == TYPEOF(Image8))
		type = TYPEOF(Image8);

	assert(type);

	Point spScale = GetSubpixelScale();

	int xPadding2 = 
		m_subpixel && m_spHorizontal
		? m_spPadding * 2
		: 0;

	int yPadding2 = 
		m_subpixel && !m_spHorizontal
		? m_spPadding * 2
		: 0;

	m_spBufferHeight = m_subpixel && !m_spHorizontal
		? spScale.Y + yPadding2
		: 1;

	m_scanline = (byte*)calloc((m_image->Width * spScale.X + xPadding2) * m_spBufferHeight, sizeof(int));
	m_scanlineF = (byte*)calloc(m_image->Width * spScale.X * spScale.Y, sizeof(int));

	// Init buffer pointer stack

	if (m_subpixel && !m_spHorizontal)
	{
		m_spBufferLines = (int**)calloc(m_spBufferHeight, sizeof(int*));

		for (int i = 0; i < m_spBufferHeight; i++)
			m_spBufferLines[i] = (int*)m_scanline + i * (m_image->Width * spScale.X + xPadding2);
	}
}

int* Graphics::SetCurrentScanline(int y)
{
	m_currentScanline = y;

	/*	No subpixel rendering. */

	if (!m_subpixel)
		m_scanlineBuffer = (int*)m_scanline;

	/*	Horizontal subpixel layout. */

	else if (m_spHorizontal)
	{
		int xPadding = 
			m_subpixel
			? m_spPadding
			: 0;

		m_scanlineBuffer = (int*)m_scanline + xPadding;
	}
	
	/*	Vertical subpixel layout. */

	else
	{
		int yPadding = 
			m_subpixel
			? m_spPadding
			: 0;

		/*	Rotate pointers. */

		int* first = m_spBufferLines[0];

		for (int i = 0; i < m_spBufferHeight - 1; i++)
			m_spBufferLines[i] = m_spBufferLines[i + 1];

		m_spBufferLines[m_spBufferHeight - 1] = first;

		/*	Get next scanline. */

		m_scanlineBuffer = m_spBufferLines[m_spBufferHeight - 1];
	}

	return m_scanlineBuffer;
}

// TODO: move clip region stuff here

void Graphics::PlotScanline(int xStart, int xEnd, bool finalize)
{
	/*	Get the target buffer. */

	int* imageBuffer = null;

	// TODO: store the target buffer permanently
	// GetImageBuffer()
	if (typeof(m_image) == TYPEOF(Image8))
		imageBuffer = (int*)((Image8*)m_image)->Buffer;
	else if (typeof(m_image) == TYPEOF(GdiImage8))
		imageBuffer = (int*)((GdiImage8*)m_image)->Buffer;
	// TODO: handle GdiImage case

	assert(imageBuffer);

	/*	Pixels. */

	if (!m_subpixel)
	{
		int* imageScanline = imageBuffer + m_currentScanline * m_image->Width;
		int* scanline = m_scanlineBuffer;
	
		for (int x = xStart; x < xEnd; x++)
			imageScanline[x] = Color8::Over(imageScanline[x], scanline[x]);
	}

	/*	Subpixels. */

	else
	{
		Point spScale = GetSubpixelScale();

		int _xStart = xStart / spScale.X;
		int _xEnd = xEnd / spScale.X;

		int nSubline = m_currentScanline % spScale.Y;

		int* imageScanline = imageBuffer + m_currentScanline / spScale.Y * m_image->Width;

		int* scanline = m_scanlineBuffer;
		int* scanlineF = (int*)m_scanlineF + nSubline * m_image->Width;

		int xPadding = m_spHorizontal ? m_spPadding : 0;
		int yPadding = m_spHorizontal ? 0 : m_spPadding;

		int t, t1, t2, t3, t4;

		/*	To the user the filter range is 0-1, 
			which translates internally to 1-0.5 */

		int iFilter = Math::Round((1 - m_spFilter / 2) * 255);
		int _iFilter = 255 - iFilter;

		/*	Downsample and filter the subpixels 
			as separate color channels. */

		int fr, fg, fb, fa;
		int kr, kg, kb, ka;
		int sx;

		//int** _scanline = malloc

		int nSamples = 2 * m_spPadding + 1;

		static int kernel[64];

		kernel[0] = _iFilter / 3;
		kernel[1] =  iFilter / 3;
		kernel[2] =  85;
		kernel[3] =  iFilter / 3;
		kernel[4] = _iFilter / 3;

		/*	TODO: better control over the filter 
			kernel shape and number of taps. */

		/*	Horizontal subpixel layout. */

		if (m_spHorizontal)
		{
			int pixel, weight;

			for (int x = xStart; x < xEnd; x++)
			{
				fr = fg = fb = fa = 0;

				for (int s = 0; s < nSamples; s++)
				{
					sx = x - m_spPadding + s;

					pixel = scanline[sx];
					weight = kernel[s];

					fr += INT_MULT(((pixel >> 16) & 0xFF), weight, t);
					fg += INT_MULT(((pixel >>  8) & 0xFF), weight, t);
					fb += INT_MULT(((pixel      ) & 0xFF), weight, t);
					fa += INT_MULT(((pixel >> 24) & 0xFF), weight, t);
				}

				scanlineF[x] =
					  (fr << 16) 
					| (fg <<  8) 
					| (fb      )
					| (fa << 24);
			}
		}

		/*	Vertical subpixel layout. */

		else
		{
			int yWidth = m_image->Width + m_spPadding * 2;

			// Clear the result buffer
			for (int x = xStart; x < xEnd; x++)
				scanlineF[x] = 0;

			int pixel, weight;
			
			for (int x = xStart; x < xEnd; x++)
			{
				fr = fg = fb = fa = 0;

				for (int s = 0; s < nSamples; s++)
				{
					pixel = m_spBufferLines[s][x];
					weight = kernel[s];

					fr += INT_MULT((pixel >> 16) & 0xFF, weight, t);
					fg += INT_MULT((pixel >>  8) & 0xFF, weight, t);
					fb += INT_MULT((pixel      ) & 0xFF, weight, t);
					fa += INT_MULT((pixel >> 24) & 0xFF, weight, t);
				}

				scanlineF[x] = Color8::Add(
					scanlineF[x],
					  (fr << 16) 
					| (fg <<  8) 
					| (fb      )
					| (fa << 24));
			}
		}

		int src, dst;
		int pr, pg, pb, pa;
		int mr, mg, mb, ma;
		int sr, sg, sb;

		/*	Subpixel order (RGB/BGR). */

		int shR = 16, 
			shG =  8, 
			shB =  0;

		if (!m_spRGB)
		{
			shR =  0;
			shG =  8;
			shB = 16;
		}

		/*	Plot the subpixels. */

		/*	Horizontal subpixel layout. */

		if (m_spHorizontal)
		{
			for (int x = xStart; x < xEnd; x += spScale.X)
			{
				int _x = x / spScale.X;

				dst = imageScanline[_x];

				pr = (dst >> shR) & 0xFF;
				pg = (dst >> shG) & 0xFF;
				pb = (dst >> shB) & 0xFF;
				pa = (dst >>  24) & 0xFF;

				mr = (scanlineF[x    ] >> 24) & 0xFF;
				mg = (scanlineF[x + 1] >> 24) & 0xFF;
				mb = (scanlineF[x + 2] >> 24) & 0xFF;
				ma = (54 * mr + 182 * mg + 18 * mb) >> 8;

				mr = INT_LERP(ma, mr, pa, t); // Subpixel rendering doesn't work on transparent
				mg = INT_LERP(ma, mg, pa, t); // images, so the amount of subpixel separation is 
				mb = INT_LERP(ma, mb, pa, t); // proportional to the opacity of the target pixel.

				sr = (scanlineF[x    ] >> shR) & 0xFF;
				sg = (scanlineF[x + 1] >> shG) & 0xFF;
				sb = (scanlineF[x + 2] >> shB) & 0xFF;
				
				pr = INT_OVER(pr, sr, mr, t);
				pg = INT_OVER(pg, sg, mg, t);
				pb = INT_OVER(pb, sb, mb, t);
				pa = INT_OVER(pa, ma, ma, t);

				// TODO: move the subpixel over to Color8:: with optional gamma.

				imageScanline[_x] =
					  (pr << shR) 
					| (pg << shG) 
					| (pb << shB)
					| (pa <<  24);
			}
		}

		/*	Vertical subpixel layout. */

		else if (nSubline == spScale.Y - 1)
		{
			int* _scanlineF = (int*)m_scanlineF;

			for (int x = xStart; x < xEnd; x++)
			{
				dst = imageScanline[x];

				pr = (dst >> shR) & 0xFF;
				pg = (dst >> shG) & 0xFF;
				pb = (dst >> shB) & 0xFF;
				pa = (dst >>  24) & 0xFF;

				mr = (_scanlineF[x                     ] >> 24) & 0xFF;
				mg = (_scanlineF[x + m_image->Width    ] >> 24) & 0xFF;
				mb = (_scanlineF[x + 2 * m_image->Width] >> 24) & 0xFF;
				ma = (54 * mr + 182 * mg + 18 * mb) >> 8;

				mr = INT_LERP(ma, mr, pa, t); // Subpixel rendering doesn't work on transparent
				mg = INT_LERP(ma, mg, pa, t); // images, so the amount of subpixel separation is 
				mb = INT_LERP(ma, mb, pa, t); // proportional to the opacity of the target pixel.

				sr = (_scanlineF[x                     ] >> shR) & 0xFF;
				sg = (_scanlineF[x + m_image->Width    ] >> shG) & 0xFF;
				sb = (_scanlineF[x + 2 * m_image->Width] >> shB) & 0xFF;
				
				pr = INT_OVER(pr, sr, mr, t);
				pg = INT_OVER(pg, sg, mg, t);
				pb = INT_OVER(pb, sb, mb, t);
				pa = INT_OVER(pa, ma, ma, t);

				// TODO: move the subpixel over to Color8:: with optional gamma.

				imageScanline[x] =
					  (pr << shR) 
					| (pg << shG) 
					| (pb << shB)
					| (pa <<  24);
			}
		}
	}
}