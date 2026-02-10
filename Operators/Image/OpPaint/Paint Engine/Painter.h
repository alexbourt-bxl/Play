#ifndef PAINTER_H
#define PAINTER_H

#include <Types/Object.h>
#include <Threads\Thread.h>
#include <Threads\ThreadLock.h>
#include <Containers/List.h>
#include <Events/Event.h>

#include "BrushStroke.h"

#define STROKE_BUFFER_STEP	16

class Image;
class Space;
class Rect;

class Painter
	: public Object, 
	  public Thread
{
	DECLARE_TYPE;

public:
	class Message;

public:
	Painter();
	Painter(const Painter& painter);
	~Painter();

	Object*			Copy();

	__declspec(property(get = GetSlowSpeed, put = SetSlowSpeed))
	double SlowSpeed;
	double GetSlowSpeed();
	void   SetSlowSpeed(double slowSpeed);

	__declspec(property(get = GetFastSpeed, put = SetFastSpeed))
	double FastSpeed;
	double GetFastSpeed();
	void   SetFastSpeed(double fastSpeed);

	__declspec(property(get = GetLock))
	ThreadLock&	Lock;
	ThreadLock& GetLock();

	void			AddStroke(BrushStroke* stroke);

	uint			ThreadProc();

	void			DrawStroke(BrushStroke* stroke);

	void 			DrawCurve(BrushStroke* stroke, StrokePoint p1, StrokePoint p2, StrokePoint p3, StrokePoint p4, Rect* rect = null); // Draws a curve segment
	
	//static void 	DrawColor(Space* space, double x, double y, int color, double size, double hardness, double antialiasing, double opacity, double intensity, double aspect, double angle, Rect* rect = null);
	static void 	DrawColor(Image* image, double x, double y, Color color, double size, double hardness, double antialiasing, double opacity, double intensity, double aspect, double angle, Rect* rect = null);

	//static void 	DrawErase(Space* space, double x, double y, double erase, double size, double hardness, double antialiasing, double opacity, double intensity, double aspect, double angle, Rect* rect = null);
	static void 	DrawErase(Image* image, double x, double y, double erase, double size, double hardness, double antialiasing, double opacity, double intensity, double aspect, double angle, Rect* rect = null);

	//static void 	DrawDepth(Space* space, double x, double y, double relief, double size, double hardness, double antialiasing, double aspect, double angle, Rect* rect = null);
	//static void 	DrawDepth(Image* image, double x, double y, double relief, double size, double hardness, double antialiasing, double aspect, double angle, Rect* rect = null);

	//static void	ApplyDepth(Space* space, Space* spcDepth, Rect& rect);
	//static void	ApplyDepth(Image* image, Space* spcDepth, Rect& rect);
	//static void	ApplyDepth(Image* image, Image* imgDepth, Rect& rect);

private:
	List			m_strokes;

	double			m_slowSpeed;		// The speed of a slow stroke (in pixels/second). Anything slower is treated as the same value.
	double			m_fastSpeed;		// The speed of a fast stroke (in pixels/second). Anything faster is treated as the same value.

	int				m_maxUpdatePoints;

	ThreadLock		m_lock;
};

//

class Painter::Message
	: public ::Message
{
	typedef Rect _Rect;

public:
	//Space* space;
	_Rect Rect;

	Message(/*Space* _space,*/ const _Rect& rect)
		: /*space(_space),*/ Rect(rect) {}
};

#endif