#include "Graphics.h"

bool Graphics::GetSubpixel()
{
	return m_subpixel;
}

void Graphics::SetSubpixel(bool /*subpixel*/)
{
	m_subpixel = false;//subpixel;
	UpdateScanlineBuffer();
}

double Graphics::GetSubpixelFilter()
{
	return m_spFilter;
}

void Graphics::SetSubpixelFilter(double spFilter)
{
	m_spFilter = min(max(0.0, spFilter), 1.0);
}

bool Graphics::GetSubpixelHorizontal()
{
	return m_spHorizontal;
}

void Graphics::SetSubpixelHorizontal(bool spHorizontal)
{
	m_spHorizontal = spHorizontal;
	UpdateScanlineBuffer();
}

bool Graphics::GetSubpixelRGB()
{
	return m_spRGB;
}

void Graphics::SetSubpixelRGB(bool spRGB)
{
	m_spRGB = spRGB;
}

Point Graphics::GetSubpixelScale()
{
	if (!m_subpixel)
		return Point(1, 1);

	return
		m_spHorizontal
		? Point(3, 1)
		: Point(1, 3);
}