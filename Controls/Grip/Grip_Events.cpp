#include "Grip.h"

//void Grip::OnPaint(Graphics::Message& /*msg*/)
//{
	//Font font(L"Arial", 8);
	//Color color = Color::Red;
	//Text::Align align = Text::AlignCenter | Text::AlignMiddle;

	//msg.graphics->DrawRect(PaintRect, color);
	
	//switch (m_result)
	//{
	//case Grip::Client:		msg.graphics->DrawText(L"Cli",		font, PaintRect, color, null, align);	break;

	//case Grip::Caption:		msg.graphics->DrawText(L"Caption",	font, PaintRect, color, null, align);	break;
	//case Grip::Icon:			msg.graphics->DrawText(L"Sys",		font, PaintRect, color, null, align);	break;
	//case Grip::Minimize:		msg.graphics->DrawText(L"Min",		font, PaintRect, color, null, align);	break;
	//case Grip::Maximize:		msg.graphics->DrawText(L"Max",		font, PaintRect, color, null, align);	break;
	//case Grip::Close:			msg.graphics->DrawText(L"Cls",		font, PaintRect, color, null, align);	break;

	//case Grip::Left:			msg.graphics->DrawText(L"L",			font, PaintRect, color, null, align);	break;
	//case Grip::Right:			msg.graphics->DrawText(L"R",			font, PaintRect, color, null, align);	break;
	//case Grip::Top:			msg.graphics->DrawText(L"T",			font, PaintRect, color, null, align);	break;
	//case Grip::Bottom:		msg.graphics->DrawText(L"B",			font, PaintRect, color, null, align);	break;

	//case Grip::TopLeft:		msg.graphics->DrawText(L"TL",			font, PaintRect, color, null, align);	break;
	//case Grip::TopRight:		msg.graphics->DrawText(L"TR",			font, PaintRect, color, null, align);	break;
	//case Grip::BottomLeft:	msg.graphics->DrawText(L"BL",			font, PaintRect, color, null, align);	break;
	//case Grip::BottomRight:	msg.graphics->DrawText(L"BR",			font, PaintRect, color, null, align);	break;
	//}
//}

//void Grip::OnLButtonDown(void* /*source*/, Mouse::Message& msg)
//{
//	Graphics g(this);
//
//	g.DrawLine(msg.position.x - 10, msg.position.y, msg.position.x + 10, msg.position.y, Color::Red);
//	g.DrawLine(msg.position.x, msg.position.y - 10, msg.position.x, msg.position.y + 10, Color::Red);
//	
//	Update(false);
//}