#ifndef OP_CROP_H
#define OP_CROP_H

#include <Operators\Operator.h>
#include <Properties\IntProperty\IntProperty.h>

#include <Types/Graphics\Image8.h>
#include <Types/Graphics\Skin.h>

#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Controls\Button\Button.h>

class ImageView;

class OpCrop
	: public Operator
{
	DECLARE_TYPE;

public:
	OpCrop();
	OpCrop(const OpCrop& opCrop);
	~OpCrop();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	IntProperty Left;
	IntProperty Top;
	IntProperty Right;
	IntProperty Bottom;

	FloatProperty Hide;

	void	in_OnConnect(Input::Message& msg);

	void	ConnectView(OpView* view);
	void	DisconnectView(OpView* view);

private:
	Input	m_in;

	Output	m_out;
	Image*	m_outImage;
	Point	m_outOffset;

	Size	m_inSize;
	bool	m_init; // Decides whether to set the initial dimensions

	LineLayout m_layoutVertical;
	LineLayout m_layoutHorizontal;

	Button	m_btnTrim;
	void	btnTrim_OnPress(Control::Message& msg);

	/*	Working values. */
	int		m_left;
	int		m_top;
	int		m_right;
	int		m_bottom;
	double	m_hide;

	void	Left_PreChange(Property::Message& msg);
	void	Top_PreChange(Property::Message& msg);
	void	Right_PreChange(Property::Message& msg);
	void	Bottom_PreChange(Property::Message& msg);

	// View controls

	static const int GrabNone   = 0;
	static const int GrabLeft   = 1;
	static const int GrabRight  = 2;
	static const int GrabTop    = 4;
	static const int GrabBottom = 8;

	int		m_grab;
	int		m_grabSize;

	Point	m_dragOffsetTL;
	Point	m_dragOffsetBR;

	int GetGrabAspect(ImageView* view, const Point& p);
	 
	//

	static GdiImage8 s_imgTL;
	static GdiImage8 s_imgTR;
	static GdiImage8 s_imgBL;
	static GdiImage8 s_imgBR;

	//static Skin s_skinShadow;
	//static void skinShadow_Init(Skin* skin);

	void	OnPaint(Graphics::Message& msg);
	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);
};

#endif