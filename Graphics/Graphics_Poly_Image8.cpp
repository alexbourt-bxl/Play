#include "Graphics.h"

#include <Types/Geometric\Poly.h>
#include <Types/Geometric\FRect.h>

void Graphics::FillPoly_Image8(int* buffer,
								int bufferWidth, int bufferHeight,
								Poly* poly, 
								int nPolys,
								Color8& color)
{
	int col = Color8::Premult(color);

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

	int nNodes = 0;

	int top    = max(0, bounds.Y - 1);
	int bottom = min(bounds.Bottom + 1, bufferHeight);
	int left   = max(0, bounds.X - 1);
	int right  = min(bounds.Right + 1, bufferWidth);

	/*	Set up subpixel scaling. */

	Point spScale = GetSubpixelScale();

	left   *= spScale.X;
	right  *= spScale.X;
	top    *= spScale.Y;
	bottom *= spScale.Y;

	// Set up clip region

	Region clip = m_clipRgn;

	if (clip)
	{
		top    = max(top, clip.Bounds.Top);
		bottom = min(clip.Bounds.Bottom, bottom);
		left   = max(left, clip.Bounds.Left);
		right  = min(clip.Bounds.Right, right);
	}
	else
	{
		clip.Add(Rect(
			left,
			top,
			right - left,
			bottom - top));
	}

	Array<Span> spans;

	if (left >= right)
		return;

	//

	int nSamples;
	double* offsets;
	int maskValue;
	GetAntialiasVars(m_antialias, &nSamples, &offsets, &maskValue);
	
	double offset;

	const double _nSamples = (double)nSamples;
	const double _1_nSamples = 1 / _nSamples;
	const double halfSampleOffsetY = 1 / (_nSamples * 2);

	int maskWidth = (right - left) * spScale.X;
	byte* mask = (byte*)malloc(maskWidth);

	int opacity;
	int* pixel;
	ushort sadd;

	////////////////////////////////////////////////////////////

	AllocPolyBuffers();
	AllocLineBuffer();
	
	int nLines = 0;

	//memset(m_polyLines, 0, MAX_BUFFER * sizeof(LINE));

	LINE* line;
	FPoint* p0;
	FPoint* p1;
	double sy, ssy;

	double xScale = spScale.X;
	double yScale = spScale.Y;

	for (int k = 0; k < nPolys; k++)
	{
		Array<FPoint>& pPoints = poly[k].Points;
		int ppCount = pPoints.Count;

		for (int i = ppCount - 1, j = 0; j < ppCount; i = j++)
		{
			if (pPoints[i].Y < pPoints[j].Y)
			{
				p0 = &pPoints[i];
				p1 = &pPoints[j];
			}
			else
			{
				p0 = &pPoints[j];
				p1 = &pPoints[i];
			}

			line = &m_polyLines[nLines++];

			line->yMin = p0->Y * yScale;
			line->yMax = p1->Y * yScale;
			line->dx = ((p1->X - p0->X) * xScale) / (line->yMax - line->yMin) * _1_nSamples;
			line->dir = pPoints[i].Y < pPoints[j].Y ? 1 : -1;
			
			/*	The initial X has to be offset by the X component 
				of the distance to the next sample scanline, and
				if yMin < 0 then also by the equivalent X. */
			sy = line->yMin + halfSampleOffsetY;
			ssy = sy + (line->yMin < 0 ? -line->yMin : 0);

			line->x = p0->X * xScale + (ceil(ssy * _nSamples) * _1_nSamples - sy) * _nSamples * line->dx;

			// TODO: fast ceil
		}
	}

	////////////////////////////////////////////////////////////

	for (int y = top; y < bottom; y++)
	{
		int yWidth = y * bufferWidth;
		double fy = (double)y + halfSampleOffsetY;

		// clear the mask
		byte* _mask = mask;
		byte* _maskEnd = mask + maskWidth;
		while (_mask < _maskEnd)
			*(_mask++) = 0;

		for (int s = 0; s < nSamples; s++)
		{
			/*	The offset is adjusted for correct rounding by shifting it by 
				half of the smallest non-zero value in the N-rooks pattern. */

			offset = offsets[s] + (1 - (_nSamples - 1) *_1_nSamples) / 2;

			// Find intersecting nodes for this subpixel scanline

			nNodes = 0;
			
			for (int i = 0; i < nLines; i++)
			{
				line = &m_polyLines[i];

				if (fy >= line->yMin && fy < line->yMax)
				{
					m_polyNodes[nNodes] = (int)(line->x + offset);
					line->x += line->dx;
					m_polyDirs[nNodes] = line->dir;
					nNodes++;
				}
			}

			// Sort nodes

			int n = 0;

			while (n < nNodes - 1)
			{
				if (m_polyNodes[n] > m_polyNodes[n + 1])
				{ 
					SWAP(m_polyNodes[n], m_polyNodes[n + 1]); 
					SWAP(m_polyDirs[n], m_polyDirs[n + 1]); 
					if (n) n--;
				}
				else n++;
			}

			// Add to the subpixel mask

			int node0, node1;
			int i = 0;
			short wind;

			while (i < nNodes)
			{
				// Get the next node pair

				node0 = m_polyNodes[i];

				if (m_nonZeroFill) // non-zero winding
				{
					wind = m_polyDirs[i];

					while (wind)
						wind += m_polyDirs[++i];

					node1 = m_polyNodes[i++];
				}
				else // even-odd
				{
					node1 = m_polyNodes[i + 1];
					i += 2;
				}

				// Validate nodes

				if (node0 > right)
					break;

				if (node1 <= left)
					continue;

				if (node0 < left)
					node0 = left;

				if (node1 > right)
					node1 = right;

				// Fill scanline run

				int _x;

				for (int x = node0; x < node1; x++)
				{
					_x = x - left;

					// Perform a saturated add
					sadd = (ushort)mask[_x] + (ushort)maskValue;
					mask[_x] = -(sadd >> 8) | (byte)sadd;
				}
			}

			//


			fy += _1_nSamples;
		}

		// Plot the scanline

		//clip.GetSpans(
		//	y, 
		//	left, 
		//	right, 
		//	spans);

		//for (int i = 0; i < spans.Count; i++)
		//{
		//	for (int x = spans[i].Start; x < spans[i].End; x++)
		//	{
		//		pixel = buffer + yWidth + x;
		//		*pixel = Color8::Over(*pixel, col, mask[x - left]);
		//	}
		//}

		// TODO: only draw the parts of the scanline that have content,
		// _left-_right is too wide for the top of a shape like V

		int* scanline = SetCurrentScanline(y);

		for (int x = left; x < right; x++)
			scanline[x] = Color8::MultiplyScalar(col, mask[x - left]);

		PlotScanline(left, right);
	}

	free(mask);
}