#include "ScrollLayout.h"

#include <Animation\Animation.h>

DEFINE_TYPE(ScrollLayout, Layout);

ScrollLayout::ScrollLayout()
{
	*Controls.AddEvent += EventHandler(this, &ScrollLayout::controls_OnAdd);

	MouseMoveEvent += EventHandler(this, &ScrollLayout::OnMouseMove);

	m_xOffset = 0;
	m_yOffset = 0;

	m_xOffset.ChangeEvent += EventHandler(this, &ScrollLayout::OnOffsetChange);
	m_yOffset.ChangeEvent += EventHandler(this, &ScrollLayout::OnOffsetChange);

	IgnoreHitTest = false;
	HitTestBeforeChildren = true;
}

void ScrollLayout::ScrollTo(int x, int y, bool animate)
{
	if (animate)
	{
		Animation::Suspend();

		PointAnimation* anim = new PointAnimation();
			anim->PropertyX = m_xOffset;
			anim->PropertyY = m_yOffset;
			anim->To = Point(x, y);
			anim->Speed = 3000.0;
			//anim->Duration = 0.6;
			anim->Curve = AnimationCurve::Smooth;
			anim->OnConflict = Animation::ReplaceCurrent;

		Animation::Add(anim);
		Animation::Resume();
	}
	else
	{
		/*	First cancel any animation. */

		Animation::Suspend();

		PointAnimation* anim = new PointAnimation();
			anim->PropertyX = m_xOffset;
			anim->PropertyY = m_yOffset;

		Animation::Remove(anim);
		Animation::Resume();

		/*	Now position the contents. */

		m_xOffset.DisableEvents();
		m_xOffset = x;
		m_xOffset.EnableEvents();

		m_yOffset = y;
	}
}