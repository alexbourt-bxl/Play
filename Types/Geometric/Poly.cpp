#include "Poly.h"

#include <Math\Math.h>
#include <Types/Geometric\FRect.h>

#include <stdlib.h>
#include <assert.h>
#include <math.h>

DEFINE_TYPE(Poly, Object);

Poly::Poly()
{

}

Poly::Poly(const Poly& poly)
{
	for (int i = 0; i < ((Poly&)poly).m_points.Count; i++)
		m_points.Add(poly.m_points[i]);
}

Poly::~Poly()
{

}

Object* Poly::Copy()
{
	return new Poly(*this);
}

Poly& Poly::operator = (const Poly& poly)
{
	m_points.Clear();

	for (int i = 0; i < ((Poly&)poly).m_points.Count; i++)
		m_points.Add(poly.m_points[i]);

	return *this;
}

Array<FPoint>& Poly::GetPoints()
{
	return m_points;
}

FRect Poly::GetBounds()
{
	FRect bounds;

	for (int i = 0; i < m_points.Count; i++)
		bounds.Expand(m_points[i]);

	return bounds;
}

FPoint Poly::GetCenter()
{
	double x = 0.0;
	double y = 0.0;

	for (int i = 0; i < m_points.Count; i++)
	{
		x += m_points[i].X;
		y += m_points[i].Y;
	}

	x /= (double)m_points.Count;
	y /= (double)m_points.Count;

	return FPoint(x, y);
}

bool Poly::Contains(const FPoint& p)
{
	bool oddNodes = false;
	
	for (int i = 0, j = m_points.Count - 1; i < m_points.Count; j = i++)
	{
		if ((m_points[i].Y < p.Y && m_points[j].Y >= p.Y) || (m_points[j].Y < p.Y && m_points[i].Y >= p.Y))
		{
			if (m_points[i].X + (p.Y - m_points[i].Y) / (m_points[j].Y - m_points[i].Y) * (m_points[j].X - m_points[i].X) < p.X)
				oddNodes = !oddNodes;
		}
	}

	return oddNodes;
}

void Poly::Offset(const FPoint& p)
{
	for (int i = 0; i < m_points.Count; i++)
		m_points[i] += p;
}

void Poly::Inset(double dist) 
{
	if (m_points.Count < 3) 
		return; // The polygon must have at least three corners to be inset.

	FPoint p, p1, p2, p3;

	p2 = m_points[m_points.Count - 1]; 
	p3 = m_points[0];

	int i;

	for (i = 0; i < m_points.Count - 1; i++) 
	{
		p1 = p2; 
		p2 = p3;
		p3 = m_points[i + 1];

		InsetCorner(p1, p2, p3, &m_points[i], dist); 
	}

	InsetCorner(p2, p3, p, &m_points[i], dist);
}

void Poly::InsetCorner(FPoint& p1, FPoint& p2, FPoint& p3, FPoint* p, double dist) 
{
	FPoint pp1 = p2;
	FPoint pp2 = p2;
	
	// Calculate the lengths of the segments
	
	double dx1 = p2.X - p1.X; 
	double dy1 = p2.Y - p1.Y; 
	double _1_dist1 = Math::InvSqrt(dx1*dx1 + dy1*dy1); //sqrt(SQR(dx1) + SQR(dy1));
	
	double dx2 = p3.X - p2.X; 
	double dy2 = p3.Y - p2.Y; 
	double _1_dist2 = Math::InvSqrt(dx2*dx2 + dy2*dy2); //sqrt(SQR(dx2) + SQR(dy2));

	// Exit if either segment has zero length

	//if (dist1 == 0.0 || dist2 == 0.0) 
	if ((dx1 == 0 && dy1 == 0) || (dx2 == 0 && dy2 == 0))
		return;

	// Inset the segments
	
	double insetX = dy1 * dist * _1_dist1; // / dist1; 
	p1.X += insetX; 
	pp1.X += insetX;

	double insetY = -dx1 * dist * _1_dist1; // / dist1; 
	p1.Y += insetY; 
	pp1.Y += insetY;

	insetX = dy2 * dist * _1_dist2; // / dist2; 
	p3.X += insetX; 
	pp2.X += insetX;

	insetY =- dx2 * dist * _1_dist2; // / dist2; 
	p3.Y += insetY;
	pp2.Y += insetY;

	// If the inset segments connect exactly, return the connection point
	
	if (pp1 == pp2) 
	{
		*p = pp1;
		return; 
	}

	// If the inset segments intersect, return the intersection point
	
	FPoint pi(insetX, insetY);

	if (Math::IntersectLines(p1, pp1, pp2, p3, pi, false)) 
		*p = pi;
}

void Poly::Reverse()
{
	FPoint temp;

	for (int i = 0; i < m_points.Count / 2; i++)
	{
		temp = m_points[i];
		m_points[i] = m_points[m_points.Count - 1 - i];
		m_points[m_points.Count - 1 - i] = temp;
	}
}