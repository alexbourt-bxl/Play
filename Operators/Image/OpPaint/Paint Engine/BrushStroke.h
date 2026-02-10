#ifndef BRUSH_STROKE_H
#define BRUSH_STROKE_H

#include <Containers/Array.h>
#include <Types/Geometric\Rect.h>
#include <Types/Color\Color.h>
#include <Events/Event.h>

#include "StrokePoint.h"
//#include "Space.h"
#include "Noise.h"

////////////////////////////////////////////////////////////////////////////////////

class BrushStroke;
class StrokeMessage 
	: public Message
{
public:
	Rect* UpdateRect;
	BrushStroke* Stroke;

	StrokeMessage()
		: UpdateRect(null), Stroke(null) {}
};

////////////////////////////////////////////////////////////////////////////////////

class Brush;
class Image;

class BrushStroke
{
	friend class Painter;

	typedef Brush _Brush;
	typedef Color _Color;

public:
	BrushStroke();
	~BrushStroke();

	void Reset();

	__declspec(property(get = GetBrush, put = SetBrush))
	_Brush* Brush;
	_Brush*	GetBrush() const;
	void	SetBrush(_Brush* brush);

	__declspec(property(get = GetColor, put = SetColor))
	_Color Color;
	_Color GetColor();
	void   SetColor(const _Color color);

	__declspec(property(get = GetTargetImage, put = SetTargetImage))
	Image* TargetImage;
	Image* GetTargetImage() const;
	void   SetTargetImage(Image* imgTarget);

	__declspec(property(get = GetPaintLayer, put = SetPaintLayer))
	Image* PaintLayer;
	Image* GetPaintLayer() const;
	void   SetPaintLayer(Image* imgPaint);

	__declspec(property(get = GetEraseLayer, put = SetEraseLayer))
	Image* EraseLayer;
	Image* GetEraseLayer() const;
	void   SetEraseLayer(Image* imgErase);

	__declspec(property(get = GetPoints))
	Array<StrokePoint*>& Points;
	Array<StrokePoint*>& GetPoints();

	//

	uint			Seed;
	
	BrushStroke*	NextStroke;

	//Space			space;
	//Space			spcErase;
	double			DisplayScale;

	_Color			_mixedColor;

	int				_advance; // The position of the end of the completed part of the stroke
	Rect			_bounds; // The rectangle that contains the entire stroke; grows as the stroke progresses

	bool			_firstStampMade;			// Indicates whether the first stamp has been drawn
	StrokePoint		_bpl;						// The last brush point
	double			_remainingStrokeDistance;	// Remaining distance before the next brush point

	double			_accumFlow;

private:
	_Brush*			m_brush;
	_Color			m_color;

	Image*			m_imgPaint;
	Image*			m_imgErase;
	Image*			m_imgTarget; // the target from which colors are sampled

	Array<StrokePoint*> m_points;

	double			m_prevAngle;
	int				m_angleWinds;

	StrokePoint		m_L;
	StrokePoint		m_P;
	double			m_stepDistanceCount;	// The counter used to decide whether to draw a stroke update
	double			m_dTime;
};

#endif