#ifndef LINE_LAYOUT_H
#define LINE_LAYOUT_H

#include <Controls\Layout\Layout.h>
#include <Types/Geometric\Size.h>

class Spacer;

class LineLayout 
	: public Layout
{
	DECLARE_TYPE;

public:
	enum _Orientation 
	{ 
		Horizontal, 
		Vertical 
	};

public:
	LineLayout();

	_Size GetContentSize(_Size* refSize);

	bool AdjustSize(_Size& size);
	void PerformLayout();

	__declspec(property(get = GetOrientation, put = SetOrientation))
	_Orientation Orientation;
	_Orientation GetOrientation();
	void SetOrientation(_Orientation orientation);

	__declspec(property(get = GetWrap, put = SetWrap))
	bool Wrap;
	bool GetWrap();
	void SetWrap(bool wrap);

	__declspec(property(get = GetReverse, put = SetReverse))
	bool Reverse;
	bool GetReverse();
	void SetReverse(bool reverse);

	__declspec(property(get = GetGap, put = SetGap))
	Point& Gap;
	Point& GetGap();
	void SetGap(const Point& gap);
	void SetGap(int gap);

	__declspec(property(get = GetAnimate, put = SetAnimate))
	bool Animate;
	bool GetAnimate();
	void SetAnimate(bool animate);

private:
	inline int GetControlPositionAlong(Point& p);
	inline int GetControlPositionAcross(Point& p);
	inline int GetControlSizeAlong(Control* control, bool checkSpacer = true);
	inline int GetControlSizeAcross(Control* control);

	inline void SetControlPosition(Point& p, int along, int across);
	
	void PositionAlignControls(
		Array<Point>& points,
		int lineStartIndex, int nextLineStartIndex, 
		int lineSizeAlong, int lineSizeAcross, 
		int acrossPos,
		bool checkSpacers);

	inline int GetGapAlong();
	inline int GetGapAcross();

	/*	Use a cache to store control destinations. Then when new
		destinations are calculated, compare them to the cached ones.
		Replace the ones that differ and restart the animations.
		Otherwise just ignore the control. */
	Array<Point> m_destCache;

	_Orientation m_orientation;

	/*	NOTE: This comment is false, but I am keeping it here 
		      for now just in case.

		AutoSize and Wrap relate as follows:
	
		if Wrap is disabled, AutoSize fits the control rect to the content.
		If Wrap is enabled, AutoSize ignores the along size and fits the 
		control rect only in the across dimension. */
	bool m_wrap;

	/*	Align the controls in reverse order. */
	bool m_reverse;

	Point m_gap;

	bool m_animate;

	/*	When the control is invisible, it does not update its layout. However, 
		if the layout is animated, as soon as the control becomes visible again, 
		it needs to be "already updated". So a one-time hard layout flag is used 
		to update the layout without animation in this case. */
	bool m_forceNoAnimate;

	/*	A utility array used to 
		mark weighted spacers. */
	Array<Spacer*> m_spacers;

	_Size	m_cachedContentSize;

	//

	void OnPaint(Graphics::Message& msg);
	void OnPreUpdateLayout(::Message& msg);
	void OnPreVisibleChange(Control::Message& msg);

	void OnAddControl(Container::Message& msg);
	void OnRemoveControl(Container::Message& msg);
};

#endif