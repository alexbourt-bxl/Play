#include "Layout.h"

DEFINE_TYPE(Layout, Control);

Layout::Layout()
{
	IgnoreHitTest = true;

	m_align = Align::Default;
	m_autoSize = false;
	m_padding = 0;

	PaintEvent += EventHandler(this, &Layout::OnPaint);

	*Controls.AddEvent    += EventHandler(this, &Layout::OnAddControl);
	*Controls.RemoveEvent += EventHandler(this, &Layout::OnRemoveControl);

	ClipChildren = true;

	MarkForLayout();
}

Layout::~Layout()
{

}

//Rect Layout::GetClientRect()
//{
//	_Rect clientRect = Control::GetClientRect();
//
//	clientRect.Left   += m_padding;
//	clientRect.Top    += m_padding;
//	clientRect.Right  -= m_padding;
//	clientRect.Bottom -= m_padding;
//
//	return clientRect;
//}

void Layout::PerformLayout()
{
	_Rect clientRect = ClientRect;

	clientRect.Left   += m_padding;
	clientRect.Top    += m_padding;
	clientRect.Right  -= m_padding;
	clientRect.Bottom -= m_padding;

	for (int i = 0; i < Controls.Count; i++)
	{
		Control* control = Controls[i];

		control->SetRefRect(clientRect);

		if		(m_align == Align::Left)	 control->X = clientRect.Left;
		else if (m_align == Align::Center)	 control->X = clientRect.Left + (clientRect.Width - control->Width) / 2;
		else if (m_align == Align::Right)	 control->X = clientRect.Right - control->Width;
	
		if		(m_align == Align::Top)		 control->Y = clientRect.Top;
		else if (m_align == Align::Middle)	 control->Y = clientRect.Top + (clientRect.Height - control->Height) / 2;
		else if (m_align == Align::Bottom)	 control->Y = clientRect.Bottom - control->Height;
		else if (m_align == Align::Baseline) control->Y = clientRect.Bottom - control->Height;
	}
}

bool Layout::AdjustSize(_Size& size)
{
	if (m_autoSize)
	{
		size.Width  -= Padding * 2;
		size.Height -= Padding * 2;

		size = GetContentSize(&size);
		
		size.Width  += Padding * 2;
		size.Height += Padding * 2;

		return true;
	}

	return false;
}

Size Layout::GetContentSize(_Size* /*refSize*/)
{
	/*	Determine control extents, then offset all controls	based on 
		the extents and the alignment, with a forced limit at (0, 0), 
		because positioning at negative coords here is undefined. */

	_Rect contentRect;

	for (int i = 0; i < Controls.Count; i++)
	{
		_Rect rect = Controls[i]->Rect;

		/*	The extents need to be shrunk
			as much as possible. */

		if		(m_align == Align::Left)	 rect.X = 0;
		else if (m_align == Align::Center)	 rect.X = 0;
		else if (m_align == Align::Right)	 rect.X = 0;

		if		(m_align == Align::Top)		 rect.Y = 0;
		else if (m_align == Align::Middle)	 rect.Y = 0;
		else if (m_align == Align::Bottom)	 rect.Y = 0;
		else if (m_align == Align::Baseline) rect.Y = 0;

		contentRect.Expand(rect);
	}

	return contentRect.Size;
}

Align Layout::GetAlign()
{
	return m_align;
}

void Layout::SetAlign(const _Align& align)
{
	m_align = align;
	UpdateLayout();
}

bool Layout::GetAutoSize()
{
	return m_autoSize;
}

void Layout::SetAutoSize(bool autoSize)
{
	m_autoSize = autoSize;
	UpdateLayout();
}

int Layout::GetPadding()
{
	return m_padding;
}

void Layout::SetPadding(int padding)
{
	m_padding = padding;
	UpdateLayout();
}