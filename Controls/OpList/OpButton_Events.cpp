#include "OpButton.h"

#include <Controls\Views\TreeView\TreeView.h>

void OpButton::OnPaint(Graphics::Message& msg)
{
	_Rect paintRect = PaintRect;

	msg.Graphics.FillRect(
		paintRect.X + 2,
		paintRect.Y + 2,
		paintRect.Width - 4,
		paintRect.Height - 4,
		Color(0.19, 0.19, 0.19, m_pressed && m_over ? 0.75 : 0.88));

	/*	Draw the skin. */

	msg.Graphics.DrawSkin(
		paintRect, 
		TreeView::s_skinOp, 
		m_pressed && m_over
			? 0.35
			: 0.5);

	/*	Draw the shadow. */

	Font font(L"Segoe UI", 18.0, Font::SemiBold);
	String name = m_type->DisplayName;

	msg.Graphics.DrawText(
		name,
		font,
		paintRect.X + paintRect.Width / 2 - 1,
		paintRect.Y + paintRect.Height / 2 - 1,
		Color(0.19, 0.19, 0.19, 0.88),
		&paintRect,
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		name,
		font,
		paintRect.X + paintRect.Width / 2 + 1,
		paintRect.Y + paintRect.Height / 2 - 1,
		Color(0.19, 0.19, 0.19, 0.88),
		&paintRect,
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		name,
		font,
		paintRect.X + paintRect.Width / 2,
		paintRect.Y + paintRect.Height / 2 - 2,
		Color(0.19, 0.19, 0.19, 0.88),
		&paintRect,
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		name,
		font,
		paintRect.X + paintRect.Width / 2,
		paintRect.Y + paintRect.Height / 2,
		Color(0.19, 0.19, 0.19, 0.88),
		&paintRect,
		Align::Center | Align::Middle);

	/*	Draw the name. */

	msg.Graphics.DrawText(
		name,
		font,
		paintRect.X + paintRect.Width / 2,
		paintRect.Y + paintRect.Height / 2 - 1,
		m_pressed && m_over
			? Color(Color::White, 0.75)
			: Color(Color::White, 0.94),
		&paintRect,
		Align::Center | Align::Middle);
}