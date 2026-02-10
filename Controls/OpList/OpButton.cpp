#include "OpButton.h"

#include <Controls\Views\TreeView\TreeView.h>

DEFINE_TYPE(OpButton, Button);

OpButton::OpButton()
{
	PaintEvent.Clear();
	PaintEvent += EventHandler(this, &OpButton::OnPaint);

	SuspendUpdates();

	SetSize(107, 31);

	ResumeUpdates();
}

Rect OpButton::GetFullPaintRect()
{
	_Rect fullPaintRect = PaintRect;

	fullPaintRect.X += TreeView::s_skinOp.OX;
	fullPaintRect.Y += TreeView::s_skinOp.OY;
	fullPaintRect.Width += TreeView::s_skinOp.OW;
	fullPaintRect.Height += TreeView::s_skinOp.OH;

	return fullPaintRect;
}

__Type* OpButton::GetOpType()
{
	return m_type;
}

void OpButton::SetOpType(__Type* type)
{
	m_type = type;
}