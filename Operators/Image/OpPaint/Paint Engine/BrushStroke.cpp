#include "BrushStroke.h"

#include <Types/Graphics\Image.h>
#include "Brush.h"

#include <limits>

BrushStroke::BrushStroke()
{
	m_brush      = null;

	m_imgPaint   = null;
	m_imgErase   = null;
	m_imgTarget  = null;

	_advance     = 0;

	Seed         = rand() % 0xFFFF;

	NextStroke   = null;
	DisplayScale = 1;

	m_stepDistanceCount	= 0;

	_firstStampMade = false;
	_remainingStrokeDistance = 0;

	_accumFlow = 0;
}

BrushStroke::~BrushStroke()
{
	//space.Clear();
	//spcErase.Clear();

	for (int i = 0; i < Points.Count; i++)
		delete Points[i];

	Points.Clear();
}

void BrushStroke::Reset()
{
	m_L = *Points[0];
	m_P = *Points[0];

	m_prevAngle = std::numeric_limits<double>::quiet_NaN();
	m_angleWinds = 0;

	_firstStampMade = false;
	_remainingStrokeDistance = 0.0f;

	//space.Clear();
	//spcErase.Clear();
}

Brush* BrushStroke::GetBrush() const
{
	return m_brush;
}

void BrushStroke::SetBrush(_Brush* brush)
{
	m_brush = brush;
}

Color BrushStroke::GetColor()
{
	return m_color;
}

void BrushStroke::SetColor(const _Color color)
{
	m_color = color;
}

Image* BrushStroke::GetTargetImage() const
{
	return m_imgTarget;
}

void BrushStroke::SetTargetImage(Image* imgTarget)
{
	m_imgTarget = imgTarget;
}

Image* BrushStroke::GetPaintLayer() const
{
	return m_imgPaint;
}

void BrushStroke::SetPaintLayer(Image* imgPaint)
{
	m_imgPaint = imgPaint;
}

Image* BrushStroke::GetEraseLayer() const
{
	return m_imgErase;
}

void BrushStroke::SetEraseLayer(Image* imgErase)
{
	m_imgErase = imgErase;
}

Array<StrokePoint*>& BrushStroke::GetPoints()
{
	return m_points;
}