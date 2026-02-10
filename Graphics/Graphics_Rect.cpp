#include "Graphics.h"

#include <Types/Graphics\Image8.h>
#include <Types/Graphics\GdiImage.h>
#include <Types/Graphics\GdiImage8.h>
#include <Types/Geometric\FRect.h>
#include <Types/Geometric\Poly.h>

void Graphics::DrawRect(int x, int y, int width, int height, const Color& _color)
{
	if (width <= 0 || height <= 0)
		return;

	Color& color = (Color&)_color;

	if (color.A == 0.0)
		return;

	if (typeof(m_image) == TYPEOF(Image8))
	{
		Color8 color8 = color;

		DrawRect_Image8(
			(int*)((Image8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			x, y, 
			width, height, 
			color8);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage8)
		&& m_preferNonGdi)
	{
		Color8 color8 = color;

		DrawRect_Image8(
			(int*)((GdiImage8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			x, y, 
			width, height, 
			color8);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage8))
	{
		Color8 color8 = color;

		DrawRect_Image8(
			(int*)((GdiImage8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			x, y, 
			width, height, 
			color8);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage))
	{
		Color8 color8 = color;

		DrawRect_GdiImage(
			((GdiImage*)m_image)->Hdc,
			x, y, 
			width, height, 
			color8);
	}
}

void Graphics::DrawRect(const Rect& rect, const Color& color)
{
	DrawRect(rect.X, rect.Y, rect.Width, rect.Height, color);
}

void Graphics::DrawRect(double x, double y, double width, double height, const Color& color, double thickness)
{
	if (width <= 0 || height <= 0)
		return;

	Poly poly;

	poly.Points.Add(FPoint(x, y));
	poly.Points.Add(FPoint(x, y + height));
	poly.Points.Add(FPoint(x + width, y + height));
	poly.Points.Add(FPoint(x + width, y));

	DrawPoly(poly, color, thickness);
}

void Graphics::DrawRect(const FRect& rect, const Color& color, double thickness)
{
	DrawRect(rect.X, rect.Y, rect.Width, rect.Height, color, thickness);
}

void Graphics::FillRect(int x, int y, int width, int height, const Color& _color)
{
	if (width <= 0 || height <= 0)
		return;

	Color& color = (Color&)_color;

	if (color.A == 0)
		return;

	if (typeof(m_image) == TYPEOF(Image8))
	{
		Color8 color8 = color;

		FillRect_Image8(
			(int*)((Image8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			x, y, 
			width, height, 
			color8);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage8)
		&& m_preferNonGdi)
	{
		Color8 color8 = color;

		FillRect_Image8(
			(int*)((GdiImage8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			x, y, 
			width, height, 
			color8);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage8))
	{
		Color8 color8 = color;

		FillRect_GdiImage(
			((GdiImage8*)m_image)->Hdc,
			x, y, 
			width, height, 
			color8);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage))
	{
		Color8 color8 = color;

		FillRect_GdiImage(
			((GdiImage*)m_image)->Hdc,
			x, y, 
			width, height, 
			color8);
	}
}

void Graphics::FillRect(const Rect& rect, const Color& color)
{
	FillRect(rect.X, rect.Y, rect.Width, rect.Height, color);
}

void Graphics::ClearRect(int x, int y, int width, int height)
{
	if (width <= 0 || height <= 0)
		return;

	if (typeof(m_image) == TYPEOF(Image8))
	{
		ClearRect_Image8(
			(int*)((Image8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			x, y, 
			width, height);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage8))
	{
		ClearRect_Image8(
			(int*)((GdiImage8*)m_image)->Buffer,
			m_image->Width, m_image->Height,
			x, y, 
			width, height);
	}
	else if (typeof(m_image) == TYPEOF(GdiImage))
	{
		ClearRect_GdiImage(
			((GdiImage*)m_image)->Hdc,
			x, y, 
			width, height);
	}
}

void Graphics::ClearRect(const Rect& rect)
{
	ClearRect(rect.X, rect.Y, rect.Width, rect.Height);
}