#include "Label.h"

DEFINE_TYPE(Label, TextControl);

Label::Label()
{
	PaintEvent += EventHandler(this, &Label::OnPaint);

	SuspendUpdates();

	m_autoSize = false;
	m_wrap     = true;
	TextFont   = Font::Label;
	TextColor  = Color(0.94, 0.94, 0.94);

	ResumeUpdates();
}

Rect Label::GetFullPaintRect()
{
	if (!Window)
		return PaintRect;

	Graphics g(this);

	/////////////////////////////////////////////////////
	// TODO: replace with platform-neutral wrapped code

	HDC hdc = ((GdiImage*)g.TargetImage)->Hdc;

	HFONT hfOld = (HFONT)::SelectObject(hdc, TextFont.m_hfont);

	TEXTMETRIC tm;
	::GetTextMetrics(hdc, &tm);

	::SelectObject(hdc, hfOld);

	/////////////////////////////////////////////////////

	_Rect fullPaintRect = PaintRect;

	fullPaintRect.Top -= tm.tmInternalLeading;
	fullPaintRect.Top -= tm.tmExternalLeading;

	return fullPaintRect;
}

bool Label::AdjustSize(_Size& size)
{
	if (m_autoSize && Window)
	{
		Graphics g(this);
		g.WrapText = m_wrap;

		//_Rect rect(
		//	0, 0, 
		//	size.Width, 
		//	size.Height);

		size = g.MeasureText(Text, TextFont/*, &rect*/);

		/////////////////////////////////////////////////////
		// TODO: replace with platform-neutral wrapped code

		HDC hdc = ((GdiImage*)g.TargetImage)->Hdc;

		HFONT hfOld = (HFONT)::SelectObject(hdc, TextFont.m_hfont);

		TEXTMETRIC tm;
		::GetTextMetrics(hdc, &tm);

		::SelectObject(hdc, hfOld);

		/////////////////////////////////////////////////////
		
		size.Height -= tm.tmInternalLeading;
		size.Height -= tm.tmExternalLeading;

		return true;
	}

	return false;
}

bool Label::GetAutoSize()
{
	return m_autoSize;
}

void Label::SetAutoSize(bool autoSize)
{
	m_autoSize = autoSize;

	if (Parent)
		Parent->UpdateLayout();
}

bool Label::GetWrap()
{
	return m_wrap;
}

void Label::SetWrap(bool wrap)
{
	m_wrap = wrap;

	if (Parent)
		Parent->UpdateLayout();
}