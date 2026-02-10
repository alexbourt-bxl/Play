#include "Textbox.h"

#include <Controls\Window\Window.h>

//#pragma warning(disable:4238)

void Textbox::OnPaint(Graphics::Message& msg)
{
	_Rect textRect = GetTextRect();
	_Rect paintRect = PaintRect;

	// Draw the gradients

	//Gradient grad1(Color(0xFFD2D2D2), Color(0xFFEDEDED)); //D3
	//Gradient grad2(Color(0xFFEDEDED), Color(0xFFD2D2D2));

	//msg.Graphics.FillRect(PaintRect.X, PaintRect.Y, PaintRect.Width / 2, PaintRect.Height, grad1);
	//msg.Graphics.FillRect(PaintRect.X + PaintRect.Width / 2, PaintRect.Y, PaintRect.Width - PaintRect.Width / 2, PaintRect.Height, grad2);

	msg.Graphics.FillRect(
		PaintRect,
		BackColor);

	// Draw the text

	_Size size = msg.Graphics.MeasureText(m_text, m_font);
	int alignOffset = 0;

	if		(TextAlign == Align::Left)	 alignOffset = 0;
	else if (TextAlign == Align::Center) alignOffset = (textRect.Width - size.Width) / 2;
	else if (TextAlign == Align::Right)  alignOffset = textRect.Width - size.Width;

	msg.Graphics.DrawText(
		m_text, 
		m_font,
		PaintRect.X + textRect.X + alignOffset + m_xOffset, 
		PaintRect.Y + textRect.Y + textRect.Height / 2, 
		TextColor, 
		&paintRect, 
		Align::Left | Align::Middle);

	if (m_selLength != 0)
	{
		// Draw the selection rectangle

		int start = min(m_selStart, m_selStart + m_selLength);
		int end   = max(m_selStart, m_selStart + m_selLength);

		_Size sizeStart = msg.Graphics.MeasureText(Text.SubString(0, start), TextFont);
		_Size sizeEnd   = msg.Graphics.MeasureText(Text.SubString(0, end), TextFont);

		_Rect selRect(
			PaintRect.X + textRect.X + sizeStart.Width + alignOffset + m_xOffset, 
			PaintRect.Y + textRect.Y + 1, 
			sizeEnd.Width - sizeStart.Width, 
			textRect.Height - 1);

		msg.Graphics.DrawSkin(
			selRect,
			s_skinSelShadow, 0.75);

		msg.Graphics.FillRect(
			selRect, 
			Color(0.17, 0.37, 0.64));

		// Draw the text

		String strSelection = Text.SubString(start, end - start);

		msg.Graphics.DrawText(
			strSelection, 
			m_font, 
			PaintRect.X + textRect.X + sizeStart.Width + alignOffset + m_xOffset, 
			PaintRect.Y + textRect.Y + textRect.Height / 2, 
			Color(0.95, 0.95, 0.95), 
			&selRect, 
			Align::Left | Align::Middle);
	}

	// Draw the skin

	msg.Graphics.DrawSkin(PaintRect, s_skinBorder);

	// Draw the caret

	if (s_showCaret)
	{
		if (m_insert && m_selLength == 0)
		{
			int start = m_selStart;
			int end = min(m_selStart + 1, (signed)m_text.Length);

			_Size sizeStart = msg.Graphics.MeasureText(Text.SubString(0, start), TextFont);
			_Size sizeEnd = start != end
				? msg.Graphics.MeasureText(Text.SubString(0, end), TextFont)
				: _Size(sizeStart.Width + m_font.AverageCharWidth, sizeStart.Height);

			_Rect insRect(
				PaintRect.X + textRect.X + sizeStart.Width + alignOffset + m_xOffset, 
				PaintRect.Y + textRect.Y + 1, 
				sizeEnd.Width - sizeStart.Width, 
				textRect.Height - 1);

			msg.Graphics.FillRect(
				insRect, 
				Color(0.06, 0.06, 0.06));

			// Draw the text

			if (start != end)
			{
				String strOver = Text.SubString(start, end - start);

				msg.Graphics.DrawText(
					strOver, 
					m_font, 
					PaintRect.X + textRect.X + sizeStart.Width + alignOffset + m_xOffset, 
					PaintRect.Y + textRect.Y + textRect.Height / 2, 
					Color(0.95, 0.95, 0.95), 
					&insRect, 
					Align::Left | Align::Middle);
			}
		}
		else
		{
			_Size szCaret = msg.Graphics.MeasureText(
				m_text.SubString(0, m_selStart + m_selLength), 
				m_font); // The caret is always at the logical end of the selection, hence the +selLength.
			
			msg.Graphics.FillRect(
				PaintRect.X + textRect.X + szCaret.Width + alignOffset + m_xOffset, 
				PaintRect.Y + textRect.Y + 1, 
				1, 
				textRect.Height - 1, 
				Color::Black);
		}
	}
}

void Textbox::OnKeyDown(Key::Message& msg)
{
	// If the textbox is not in focus, 
	// don't react to the keyboard.

	if (!HasFocus)
		return;

	if (msg.Key == Key::Enter)
	{
		Key_Enter();	
		msg.SendChar = false;
	}
	else if (msg.Key == Key::Escape)
	{
		Key_Escape();	
		msg.SendChar = false;
	}
	else if (msg.Key == Key::Insert)
	{
		Key_Insert();
	}
	else if (msg.Key == Key::Left)
	{
		Key_Left();
	}
	else if (msg.Key == Key::Right)
	{
		Key_Right();
	}
	else if (msg.Key == Key::End)
	{
		Key_End();
	}
	else if (msg.Key == Key::Home)
	{
		Key_Home();
	}
	else if (msg.Key == Key::Backspace)
	{
		Key_Backspace();
		msg.SendChar = false;
	}
	else if (msg.Key == Key::Delete)
	{
		Key_Delete();
		msg.SendChar = false;
	}
	else if (msg.Key == Key::Tab)
	{
		Key_Tab();
		msg.SendChar = false;
	}	
	else if (msg.Key == Key::A)
	{
		msg.SendChar = !Key_A();
	}
	else if (msg.Key == Key::D)
	{
		msg.SendChar = !Key_D();
	}
	else if (msg.Key == Key::X)
	{
		msg.SendChar = !Key_X();
	}
	else if (msg.Key == Key::C)
	{
		msg.SendChar = !Key_C();
	}
	else if (msg.Key == Key::V)
	{
		msg.SendChar = !Key_V();
	}
}

void Textbox::OnKeyUp(Key::Message& /*msg*/)
{

}

void Textbox::OnChar(Key::Message& msg)
{
	// If the textbox is not editable or in focus, no characters can be entered

	if (!HasFocus)
		return;

	if (Key::Control.IsDown)
		return;

	switch (msg.Char)
	{

	/****** ASCII control codes ******/

	case 0x07: // bell (\a)
		System::Beep();
		break;

	case 0x08: // backspace (\b)
		Key_Backspace();
		break;

	case 0x09: // horizontal tab (\t)
		Key_Tab();
		break;

	//case 0x0D: // carriage return (\r)
	//	Key_Enter();
	//	break;

	//case 0x1B: // escape (\msg)
	//	Key_Escape();
	//	break;

	case 0x7F: // delete
		Key_Delete();
		break;

	/*********************************/

	default:
		{
			// Type the character

			wchar_t wc[2] = { msg.Char, null };

			if (m_insert && m_selLength == 0 && m_selStart < (signed)m_text.Length)
				m_selLength = 1;

			CaretPos += Insert(wc);

			s_showCaret = true;
			Update();

			break;
		}
	}
}

void Textbox::OnMouseMove(Mouse::Message& msg) 
{
	int pos = GetCaretPos(msg.Position);

	if (m_selecting)
	{
		int selLengthOld = m_selLength;
		Select(m_selStart, pos - m_selStart);

		if (m_selLength != selLengthOld) // update once per char, not once per pixel
		{
			s_showCaret = true;
			Update();
		}
	}
}

void Textbox::OnLButtonDown(Mouse::Message& msg)
{
	if (!m_allowEdit)
		return;

	CaretPos = GetCaretPos(msg.Position);
	m_selecting = true;
	s_showCaret = true;
	Update();
}

void Textbox::OnLButtonUp(Mouse::Message& /*msg*/) 
{
	m_selecting = false;
}

void Textbox::OnGetFocus(Control::Message& /*msg*/)
{
	if (!m_editing)
		StartEditing();

	s_showCaret = true;

	Window->StartTimer(
		s_blinkTimerId, 
		s_blinkInterval, 
		this,
		CaretBlink);
}

void Textbox::OnLoseFocus(Control::Message& /*msg*/)
{
	if (m_editing)
		FinishEditing(true);
	
	s_showCaret = false;

	Window->StopTimer(s_blinkTimerId);
}

void Textbox::CaretBlink(Control* /*control*/)
{
	s_showCaret = !s_showCaret;

	if (Control::Focused)
		Control::Focused->Update();
}