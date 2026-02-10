#include "Window.h"

#include <Graphics\Graphics.h>
#include <Types/Graphics\GdiImage8.h>

void Window::OnPaint(Graphics::Message& msg)
{
	msg.Graphics.FillRect(
		ClientRect, 
		BackColor);
}

void Window::OnMove(Control::Message& /*msg*/) 
{ 	
	/*	In this case no repainting should be done when the window is moved,
		because the operating system automatically handles it. */

	// TODO: for windows XP- the window SHOULD be repainted.
}

void Window::OnSize(Control::Message& /*msg*/)
{
	/*	If the client rect is null, as when the window
		is minimized, the buffer remains untouched. */
	if (ClientRect)
		m_buffer.SetSize(ClientRect.Size);
}

void Window::position_OnChange(Property::Message& /*msg*/)
{
	if (UpdatesSuspended)
		return;

	if (!m_suppressRectChanges)
	{
		SetWindowPos(
			Hwnd, null,
			X, Y,
			0, 0,
			SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
	}
}

void Window::size_OnChange(Property::Message& /*msg*/)
{
	if (UpdatesSuspended)
		return;

	if (!m_suppressRectChanges)
	{
		::SetWindowPos(
			Hwnd, null,
			0, 0,
			Width, Height,
			SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
	}
}

void Window::shakeTimer_OnTick(Control* control)
{
	Window* wnd = (Window*)control;
	wnd->StopTimer(wnd->m_shakeTimerId);
	wnd->m_possibleShake = false;
}

void Window::holdTimer_OnTick(Control* control)
{
	Window* wnd = control->Window;
	wnd->StopTimer(wnd->m_holdTimerId);

	//wnd->m_mouseDown = false;

	//if (Control::Captured)
	//	Control::Captured->Uncapture();

	//if (!wnd->m_lockedCapture)
	//	::ReleaseCapture();

	wnd->m_holdIgnoreButtonUp = true;
	control->LButtonHoldEvent.Send(wnd->m_holdArgs);
}

void Window::floating_OnEndAnimation(Animation::Message& msg)
{
	FloatAnimation* anim = (FloatAnimation*)msg.Object;
	Control* control = dynamic_cast<Control*>(anim->Property->Container);

	m_floating.Remove(control);

	control->Parent = null;
}

#ifdef _DEBUG
void Window::keysShowUpdateRgn_OnPress(::Message& /*msg*/)
{
	showUpdateRgn = !showUpdateRgn;
	//Update();
}
#endif