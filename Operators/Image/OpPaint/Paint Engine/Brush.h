#ifndef BRUSH_H
#define BRUSH_H

/*
	Using the brush:

	In most cases, 'step' should initially be set to 1. However, when the user zooms
	into the image, 'step' should be reduced by the same factor, because now an image
	pixel becomes larger than a display pixel. So, the application should set:

		initialStep = 1.0;
		step = initialStep / displayScale;

	How the brush works:
		
		While the stroke is in progress, the brush is drawn to a temporary buffer, which is then
		composited on top of the underlying actual buffer. The reason for this is that this gives
		you the option to adjust overlap and opacity separately. Changing only the opacity guarantees 
		that even if the stroke crosses itself, there will never be an over-opacity. This WILL happen
		if you adjust the overlap.
*/

#include <Types/Object.h>
#include <Threads\ThreadLock.h>
#include <Containers/List.h>

//#include "BlendMode.h"
#include "BrushParameter.h"
#include "Noise.h"
//#include "Iserializable.h"
#include "BrushMessage.h"
//#include "UpdateEvent.h"
#include "Painter.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const UINT	Brush_Parameter_Type_ID						= 0x0100;
const UINT	Brush_Parameter_ColorMixMode_ID				= 0x0200;
const UINT	Brush_Parameter_BlendMode_ID				= 0x0300;
const UINT	Brush_Parameter_ColorHue_ID					= 0x0400;
const UINT	Brush_Parameter_ColorSaturation_ID			= 0x0500;
const UINT	Brush_Parameter_ColorLightness_ID			= 0x0600;
const UINT	Brush_Parameter_Size_ID						= 0x0700;
const UINT	Brush_Parameter_Hardness_ID					= 0x0800;
const UINT	Brush_Parameter_Opacity_ID					= 0x0900;
const UINT	Brush_Parameter_Intensity_ID				= 0x0A00;
const UINT	Brush_Parameter_Aspect_ID					= 0x0B00;
const UINT	Brush_Parameter_Angle_ID					= 0x0C00;
const UINT	Brush_Parameter_AngleFromStrokeDirection_ID	= 0x0D00;
const UINT	Brush_Parameter_AngleFromPenOrientation_ID	= 0x0E00;
const UINT	Brush_Parameter_AngleFromPenRotation_ID		= 0x0F00;
const UINT	Brush_Parameter_Spacing_ID					= 0x1000;
const UINT	Brush_Parameter_ScatterAcross_ID			= 0x1100;
const UINT	Brush_Parameter_ScatterAcrossScale_ID		= 0x1200;
const UINT	Brush_Parameter_ScatterAlong_ID				= 0x1300;
const UINT	Brush_Parameter_ScatterAlongScale_ID		= 0x1400;
const UINT	Brush_Parameter_Apply_ID					= 0x1500;
const UINT	Brush_Parameter_Lift_ID						= 0x1600;
const UINT	Brush_Parameter_Hold_ID						= 0x1700;
const UINT	Brush_Parameter_Erase_ID					= 0x1800;
const UINT	Brush_Parameter_Flow_ID						= 0x1900;
const UINT	Brush_Parameter_Step_ID						= 0x1A00;
const UINT	Brush_Parameter_Antialias_ID				= 0x1B00;
const UINT	Brush_Parameter_StrokeFilter_ID				= 0x1C00;
const UINT	Brush_Parameter_SpeedFilter_ID				= 0x1D00;
const UINT	Brush_Parameter_PressureCurve_ID			= 0x1E00;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Stroke;
class StrokePoint;
class BrushControl;

class Brush 
	: public Object, 
	  public ThreadLock,
	  public EventSender//, 
	  //public ISerializable
{
	DECLARE_TYPE;

	friend class Painter;

public:
	//BrushControl*	control;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Parameters

	enum BrushType
	{
		Stylus,
		Pencil,
		BallpointPen,
		CaligraphyPen,
		Chalk,
		Marker,
		Paintbrush,
		Airbrush,
		Eraser
	};

	enum ColorMixMode
	{
		Add,
		Subtract
	};

	BrushType		type;

	ColorMixMode	colorMixMode;

	//BlendMode		blendMode;

	BrushParameter	ColorHue;
	BrushParameter	ColorSaturation;
	BrushParameter	ColorLightness;

	BrushParameter	Size;
	BrushParameter	Hardness;
	BrushParameter	Opacity;
	BrushParameter	Intensity;

	BrushParameter	Aspect;
	BrushParameter	Angle;

	BrushParameter	AngleFromStrokeDirection;
	BrushParameter	AngleFromPenOrientation;
	BrushParameter	AngleFromPenRotation;

	BrushParameter	Spacing;

	BrushParameter	ScatterAcross;
	BrushParameter	ScatterAcrossScale;
	Noise			ScatterAcrossNoise;

	BrushParameter	ScatterAlong;
	BrushParameter	ScatterAlongScale;
	Noise			ScatterAlongNoise;

	BrushParameter	Apply;	// determines how much color to apply (opacity of the brush color, basically)
	BrushParameter	Lift;	// determines how much of the existing color to lift off and mix with the stroke color
	BrushParameter	Hold;	// determines how long to hold the lifted color
	BrushParameter	Erase;

	BrushParameter	Flow;	// the amount of color to apply per unit time
	
	BrushParameter	Step;	// The distance against which m_stepDistanceCount is tested

	Noise			SourceNoise;

	double			Antialias;	// The thickness of the antialiased border (in pixels)

	double			StrokeFilter;
	double			SpeedFilter;

	double			PressureCurve;

	// NOTE: When adding a parameter, make sure it's taken care of everywhere (m_paramList, ReadFromBuffer, etc.)

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	Brush(BrushType type = Stylus);
	Brush(const Brush& brush);
	~Brush();

	Object*	Copy();

	__declspec(property(get = GetPosition))
	FPoint Position;
	FPoint GetPosition();

	__declspec(property(get = GetPressure))
	double Pressure;
	double GetPressure();

	__declspec(property(get = GetOrientation))
	double Orientation; // azimuth
	double GetOrientation();
	
	__declspec(property(get = GetTilt))
	double Tilt; // elevation
	double GetTilt();

	__declspec(property(get = GetTwist))
	double Twist; // rotation
	double GetTwist();
	
	BrushStroke*	GetCurrentStroke() const;

	void			SetSeed(uint seed = 0);

	Color			GetAdjustedColor(BrushStroke* stroke, double pressure, double speed, double tilt, const Color& lifted);
	double			GetNormalizedSpeed(double dTime, double distance, double zoom);
	double			GetNormalizedTilt(double elevation);

	// Drawing

	BrushStroke*	Down(Image* imgPaint, Image* imgErase, Image* imgTarget, const Color& color);
	void			Move(FPoint& p, double pressure, double azimuth, double elevation, double twist, double dTime);
	void			Move(Array<StrokePoint>& points);
	void			Up();

	void			CheckStraight();
	void			CheckStraight(double& x, double& y);

	//void			MoveStroke(BrushStroke* stroke, Rect& updateRect);
	//void			EndStroke(BrushStroke* stroke, Rect& updateRect);

	// Selialize

	//int			GetSizeInBuffer();
	//void			WriteToBuffer(char* buffer, int* pos);
	//bool			ReadFromBuffer(char* buffer, int* pos);

	Event<StrokeMessage&> MoveStrokeEvent;
	Event<StrokeMessage&> EndStrokeEvent;

private:
	List			m_paramList;
	void			InitParamList();
	BrushParameter*	GetParamFromId(UINT paramId);

	FPoint			m_position;
	double			m_pressure;
	double			m_orientation;
	double			m_tilt;
	double			m_twist;
	double			m_dTime; // time difference from previous position
	
	BrushStroke*	m_currentStroke;

public:
	enum Straight
	{
		NA = 0,
		Horizontal,
		Vertical
	};

	Straight		straight;
	Array<StrokePoint> straightPoints;

	static Painter	painter;

	//Event<::Message> MoveEvent;
	//Event<Painter::Message> UpdateEvent;
};

#endif