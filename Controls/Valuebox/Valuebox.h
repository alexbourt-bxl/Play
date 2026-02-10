#ifndef VALUEBOX_H
#define VALUEBOX_H

#include <Controls\TextControl\TextControl.h>
#include <Graphics\Graphics.h>
#include <Controls\Textbox\Textbox.h>
#include <Types/Graphics\GdiImage8.h>
#include <Types/Graphics\Skin.h>

class ValueMessage;

class Valuebox 
	: public TextControl
{
	DECLARE_TYPE;

	friend class ActChangeIntProperty;

public:
	Valuebox();
	~Valuebox();

	_Rect GetFullPaintRect();	

	__declspec(property(get = GetValueColor, put = SetValueColor))
	Color	ValueColor;
	Color	GetValueColor();
	void	SetValueColor(const Color& color);

	__declspec(property(get = GetValue, put = SetValue))
	double	Value;
	double	GetValue();
	void	SetValue(double value);
	void	SetValue(int value);

	__declspec(property(get = GetAllowNA, put = SetAllowNA))
	bool	AllowNA;
	bool	GetAllowNA();
	void	SetAllowNA(bool allowNA);

	__declspec(property(get = GetAllowInfinite, put = SetAllowInfinite))
	bool	AllowInfinite;
	bool	GetAllowInfinite();
	void	SetAllowInfinite(bool allowInfinite);

	__declspec(property(get = GetEditOnLButtonUp, put = SetEditOnLButtonUp))
	bool	EditOnLButtonUp;
	bool	GetEditOnLButtonUp();
	void	SetEditOnLButtonUp(bool editOnLButtonUp);

	__declspec(property(get = GetShowBorder, put = SetShowBorder))
	bool	ShowBorder;
	bool	GetShowBorder();
	void	SetShowBorder(bool showBorder);

	void	SetNA();
	void	SetInfinite();

	__declspec(property(get = GetDisplayMultiplier, put = SetDisplayMultiplier))
	double	DisplayMultiplier;
	double	GetDisplayMultiplier();
	void	SetDisplayMultiplier(double multiplier);

	__declspec(property(get = GetGranularity, put = SetGranularity))
	double	Granularity;
	double	GetGranularity();
	void	SetGranularity(double granularity);

	__declspec(property(get = GetDecimalPlaces, put = SetDecimalPlaces))
	int		DecimalPlaces;
	int		GetDecimalPlaces();
	void	SetDecimalPlaces(int decimalPlaces);

	__declspec(property(get = GetMinValue, put = SetMinValue))
	double	MinValue;
	double	GetMinValue();
	void	SetMinValue(double min);
	void	SetMinValue(int min);

	__declspec(property(get = GetMaxValue, put = SetMaxValue))
	double	MaxValue;
	double	GetMaxValue();
	void	SetMaxValue(double max);
	void	SetMaxValue(int max);

	__declspec(property(get = GetRelative, put = SetRelative))
	bool	Relative;
	bool	GetRelative();
	void	SetRelative(bool relative);

	__declspec(property(get = GetMinVisible, put = SetMinVisible))
	double	MinVisible;
	double	GetMinVisible();
	void	SetMinVisible(double minVisible);
	void	SetMinVisible(int minVisible);

	__declspec(property(get = GetMaxVisible, put = SetMaxVisible))
	double	MaxVisible;
	double	GetMaxVisible();
	void	SetMaxVisible(double maxVisible);
	void	SetMaxVisible(int maxVisible);

	__declspec(property(get = GetAdjustSpeed, put = SetAdjustSpeed))
	double	AdjustSpeed; // Units per pixel
	double	GetAdjustSpeed();
	void	SetAdjustSpeed(double unitsPerPixel);

	__declspec(property(get = GetValueName, put = SetValueName))
	String	ValueName;
	String	GetValueName();
	void	SetValueName(const String& name);

	__declspec(property(get = GetPrefix, put = SetPrefix))
	String	Prefix;
	String	GetPrefix();
	void	SetPrefix(const String& prefix);

	__declspec(property(get = GetSuffix, put = SetSuffix))
	String	Suffix;
	String	GetSuffix();
	void	SetSuffix(const String& suffix);

	__declspec(property(get = GetNAText, put = SetNAText))
	String	NAText;
	String	GetNAText();
	void	SetNAText(const String& text);

	__declspec(property(get = GetInfiniteText, put = SetInfiniteText))
	String	InfiniteText;
	String	GetInfiniteText();
	void	SetInfiniteText(const String& text);

	String	GetValueString();

	Event<ValueMessage&> PreValueChangeEvent;
	Event<ValueMessage&> ValueChangeEvent;

	/*	CommitChangeEvent allows things like undo, whereby changes are made 
		instantly, but only recorded when this event is sent. It is sent when 
		the user has definitely finished adjusting the value. */
	Event<ValueMessage&> CommitChangeEvent;

	void	OnPaint(Graphics::Message& msg);
	//void	OnMouseEnter(Mouse::Message& msg);
	//void	OnMouseLeave(Mouse::Message& msg);
	void	OnLButtonDown(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);
	//void	OnPenMove(Tablet::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	PerformLButtonUp();
	void	OnMouseWheel(Mouse::Message& msg);
	void	OnMouseHWheel(Mouse::Message& msg);

private:
	Textbox	m_textbox;

	Color	m_valueColor;

	double	m_value;
	double	m_displayMultiplier;
	double	m_granularity; // "value snap"

	bool	m_allowNA;
	bool	m_allowInfinite;
	bool	m_editOnLButtonUp;
	bool	m_showBorder;

	/*	I use a timer to control mouse up events as 
		a workaround for the springy pen tip issue
		when clicking and dragging. */
	//int		m_penUpTimerId;
	//bool	m_deferPenUp;
	//static void penUp_TimerProc(Control* control);

	//Float	m_arrowOpacity;
	//static void	arrowOpacity_OnChange(void* sender, Message& msg);

	int		m_decimalPlaces;

	double	m_min;
	double	m_max;

	bool	m_relative;

	double	m_minVisible;
	double	m_maxVisible;

	bool	m_mouseMoved;
	bool	m_adjusting;

	int		m_overshootRight;
	int		m_overshootLeft;

	//bool	m_showStripes;
	FloatProperty m_stripeOpacity;
	//int		m_hideStripesTimerId;
	//static void OnHideStripesTimerTick(Control* control);

	double	m_adjustSpeed; // units per pixel
	double	m_pressureFactor;

	double	m_storedValue;
	double	m_valueStart;
	double	m_valueAccum;

	Point	m_posStart;
	bool	m_mouseDown;

	Point	m_posPrev;
	FPoint	m_posPrevF;

	int		m_upDownSnapThreshold;

	//bool	m_changeMode;

	String	m_valueName;
	String	m_prefix;
	String	m_suffix;
	String	m_naText;
	String	m_infiniteText;

	int		m_finalTimerId;
	static void OnFinalTimerTick(Control* control);

	//

	static Skin	s_skinBorder;
	static void	skinBorder_Init(Skin* skin);

	static GdiImage8 s_imgShadowL;
	static GdiImage8 s_imgShadowR;
	static GdiImage8 s_imgAbsolute;
	static GdiImage8 s_imgArrowLeft;
	static GdiImage8 s_imgArrowRight;
	static GdiImage8 s_imgArrowUp;
	static GdiImage8 s_imgArrowDown;

	void   textbox_OnFinishEditing(Textbox::Message& msg);
};

//

class ValueMessage
	: public ::Message
{
public:
	double* Value;

	ValueMessage(double* value)
		: Value(value) {}
};

#endif