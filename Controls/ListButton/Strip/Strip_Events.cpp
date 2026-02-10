#include "Strip.h"

void Strip::OnPaint(Graphics::Message& msg)
{
	if (m_skinNeck)
	{
		msg.Graphics.DrawSkin(
			PaintRect,
			*m_skinNeck);
	}
}