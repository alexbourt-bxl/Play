#include "WindowFrame.h"

#include <Controls/Window/Window.h>
#include <Types/Graphics/GdiImage8.h>

DEFINE_TYPE(WindowFrame, Object);

WindowFrame::WindowFrame(/*Skin* sknActive, Skin* sknInactive*/)
{
	m_active = false;

	// Bottom and Right edges should be affected first,
	// because they are the most noticed.

	m_hwndBottom  = null;
	m_hwndRight   = null;
	m_hwndTop     = null;
	m_hwndLeft    = null;

	m_imgBottom = null;
	m_imgRight  = null;
	m_imgTop    = null;
	m_imgLeft   = null;

	//m_sknActive   = sknActive;
	//m_sknInactive = sknInactive;

	//if (m_sknActive || m_sknInactive)
	//{
	//	// Register the window class

	//	WNDCLASSEX wcx;

	//	wcx.cbSize			= sizeof(WNDCLASSEX);
	//	wcx.style			= null;
	//	wcx.lpfnWndProc		= WindowProcStatic;
	//	wcx.cbClsExtra		= 0;
	//	wcx.cbWndExtra		= 0;
	//	wcx.hInstance		= HINST_PLAY;
	//	wcx.hIcon			= null;
	//	wcx.hCursor			= null;
	//	wcx.hbrBackground	= null;
	//	wcx.lpszMenuName	= null;
	//	wcx.lpszClassName	= L"WindowFrame";
	//	wcx.hIconSm			= null;

	//	if (::RegisterClassEx(&wcx))
	//	{
	//		// Create window

	//		m_hwndBottom = ::CreateWindowEx(
	//			WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
	//			wcx.lpszClassName,
	//			null,
	//			WS_POPUP,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			(HWND)null,
	//			(HMENU)null,
	//			HINST_PLAY,
	//			(LPVOID)this);

	//		m_hwndRight = ::CreateWindowEx(
	//			WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
	//			wcx.lpszClassName,
	//			null,
	//			WS_POPUP,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			(HWND)null,
	//			(HMENU)null,
	//			HINST_PLAY,
	//			(LPVOID)this);

	//		m_hwndTop = ::CreateWindowEx(
	//			WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
	//			wcx.lpszClassName,
	//			null,
	//			WS_POPUP,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			(HWND)null,
	//			(HMENU)null,
	//			HINST_PLAY,
	//			(LPVOID)this);

	//		m_hwndLeft = ::CreateWindowEx(
	//			WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
	//			wcx.lpszClassName,
	//			null,
	//			WS_POPUP,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			CW_USEDEFAULT,
	//			(HWND)null,
	//			(HMENU)null,
	//			HINST_PLAY,
	//			(LPVOID)this);
	//	}

	//	m_imgBottom = new GdiImage8(1, 1);
	//	m_imgRight  = new GdiImage8(1, 1);
	//	m_imgTop    = new GdiImage8(1, 1);
	//	m_imgLeft   = new GdiImage8(1, 1);
	//	
	//	::ShowWindow(m_hwndBottom, SW_SHOW);
	//	::ShowWindow(m_hwndRight, SW_SHOW);
	//	::ShowWindow(m_hwndTop, SW_SHOW);
	//	::ShowWindow(m_hwndLeft, SW_SHOW);
	//}
}

WindowFrame::~WindowFrame()
{
	if (m_imgBottom) delete m_imgBottom;
	if (m_imgRight)  delete m_imgRight;
	if (m_imgTop)    delete m_imgTop;
	if (m_imgLeft)   delete m_imgLeft;

	if (m_hwndBottom)  ::DestroyWindow(m_hwndBottom);
	if (m_hwndRight)   ::DestroyWindow(m_hwndRight);
	if (m_hwndTop)     ::DestroyWindow(m_hwndTop);
	if (m_hwndLeft)    ::DestroyWindow(m_hwndLeft);

	//if (m_sknActive)
	//{
	//	delete m_sknActive;
	//	m_sknActive = null;
	//}

	//if (m_sknInactive)
	//{
	//	delete m_sknInactive;
	//	m_sknInactive = null;
	//}
}

Object* WindowFrame::Copy()
{
	return null;
}

bool WindowFrame::GetIsSystemFrame()
{
	return this == &WindowFrame::SystemSizable 
		|| this == &WindowFrame::SystemFixed;
}

void WindowFrame::Update(const Point& /*position*/, const Size& /*size*/, bool /*updatePosition*/, bool /*updateSize*/, bool /*active*/)
{
	//Skin* skin = active ? m_sknActive : m_sknInactive;

	//POINT pBottom, pRight, pTop, pLeft;
	//SIZE szBottom, szRight, szTop, szLeft;

	//// Update edge positions

	////pBottom.x = position.x + skin->xo;
	////pBottom.y = position.y + skin->yo + size.Height + skin->ho - skin->b.h;

	////pRight.x = position.x + skin->xo + size.Width + skin->wo - skin->r.w;
	////pRight.y = position.y + skin->yo + skin->tr.h;

	////pTop.x = position.x + skin->xo;
	////pTop.y = position.y + skin->yo;

	////pLeft.x = position.x + skin->xo;
	////pLeft.y = position.y + skin->yo + skin->tl.h;

	//// Draw the edges 

	//if (updateSize || m_active != active)
	//{
	//	// Draw the bottom edge

	//	szBottom.cx = size.Width + skin->wo;
	//	szBottom.cy = skin->b.h;

	//	m_imgBottom->SetSize(szBottom.cx, szBottom.cy);

	//	Graphics gb(m_imgBottom);
	//	
	//	gb.FillRect(0, 0, szBottom.cx, szBottom.cy, Color::Black); // clear the image
	//	
	//	//gb.DrawImage(0, 0, skin->bl.w, skin->bl.h, skin->GetImage(), skin->bl.x, skin->bl.y, skin->bl.w, skin->bl.h);
	//	//gb.DrawImage(skin->bl.w, 0, szBottom.cx - skin->bl.w - skin->br.w, skin->b.h, skin->GetImage(), skin->b.x, skin->b.y, skin->b.w, skin->b.h);
	//	//gb.DrawImage(szBottom.cx - skin->br.w, 0, skin->br.w, skin->br.h, skin->GetImage(), skin->br.x, skin->br.y, skin->br.w, skin->br.h);

	//	// Draw the right edge

	//	szRight.cx = skin->r.w;
	//	szRight.cy = size.Height + skin->ho - skin->tr.h - skin->br.h;

	//	m_imgRight->SetSize(szRight.cx, szRight.cy);

	//	Graphics gr(m_imgRight);
	//	
	//	gr.FillRect(0, 0, szRight.cx, szRight.cy, Color::Black); // clear the image
	//	
	//	//gr.DrawImage(0, 0, szRight.cx, szRight.cy, skin->GetImage(), skin->r.x, skin->r.y, skin->r.w, skin->r.h);

	//	// Draw the top edge

	//	szTop.cx = size.Width + skin->wo;
	//	szTop.cy = skin->t.h;

	//	m_imgTop->SetSize(szTop.cx, szTop.cy);

	//	Graphics gt(m_imgTop);
	//	
	//	gt.FillRect(0, 0, szTop.cx, szTop.cy, Color::Black); // clear the image
	//	
	//	//gt.DrawImage(0, 0, skin->tl.w, skin->tl.h, skin->GetImage(), skin->tl.x, skin->tl.y, skin->tl.w, skin->tl.h);
	//	//gt.DrawImage(skin->tl.w, 0, szTop.cx - skin->tl.w - skin->tr.w, /*skin->t.h*/ skin->tl.h, skin->GetImage(), /*skin->t.x*/ skin->tl.x + skin->tl.w, /*skin->t.y*/ skin->tl.y, skin->t.w, skin->t.h);
	//	//gt.DrawImage(szTop.cx - skin->tr.w, 0, skin->tr.w, skin->tr.h, skin->GetImage(), skin->tr.x, skin->tr.y, skin->tr.w, skin->tr.h);

	//	// Draw the left edge

	//	//szLeft.cx = skin->l.w;
	//	//szLeft.cy = size.Height + skin->ho - skin->tl.h - skin->bl.h;

	//	m_imgLeft->SetSize(szLeft.cx, szLeft.cy);

	//	Graphics gl(m_imgLeft);
	//	
	//	gl.FillRect(0, 0, szLeft.cx, szLeft.cy, Color::Black); // clear the image
	//	
	//	//gl.DrawImage(0, 0, szLeft.cx, szLeft.cy, skin->GetImage(), skin->l.x, skin->l.y, skin->l.w, skin->l.h);
	//}

	//// Update the layered frame windows all at once

	//POINT pSrc = { 0, 0 };
	//BLENDFUNCTION blend = { AC_SRC_OVER, 0, 0xFF, AC_SRC_ALPHA };

	//bool updateContents = updateSize || m_active != active;

	//// The order in which the edges are updated is: bottom, left, right, top.
	//// The joining of the bottom and right edges doesn't have to be as neat,
	//// because statistically the bottom-right corner will be dragged the most.
	//// The possible gap can be evident between the bottom and left edges.

	//::UpdateLayeredWindow(
	//		m_hwndBottom, 
	//		null, 
	//		updatePosition || updateSize ? &pBottom : null, 
	//		updateSize ? &szBottom : null, 
	//		updateContents ? m_imgBottom->hdc : null, 
	//		updateContents ? &pSrc : null, 
	//		null, 
	//		&blend, 
	//		ULW_ALPHA);

	//::UpdateLayeredWindow(
	//		m_hwndLeft, 
	//		null, 
	//		updatePosition || updateSize ? &pLeft : null, 
	//		updateSize ? &szLeft : null, 
	//		updateContents ? m_imgLeft->hdc : null, 
	//		updateContents ? &pSrc : null, 
	//		null, 
	//		&blend, 
	//		ULW_ALPHA);

	//::UpdateLayeredWindow(
	//		m_hwndRight, 
	//		null, 
	//		updatePosition || updateSize ? &pRight : null, 
	//		updateSize ? &szRight : null, 
	//		updateContents ? m_imgRight->hdc : null, 
	//		updateContents ? &pSrc : null, 
	//		null, 
	//		&blend, 
	//		ULW_ALPHA);

	//::UpdateLayeredWindow(
	//		m_hwndTop, 
	//		null, 
	//		updatePosition ? &pTop : null, 
	//		updateSize ? &szTop : null, 
	//		updateContents ? m_imgTop->hdc : null, 
	//		updateContents ? &pSrc : null, 
	//		null, 
	//		&blend, 
	//		ULW_ALPHA);

	//m_active = active;

	////if (position)
	////	::SetWindowPos(m_hwnd, null, p.x, p.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

// Static methods

LRESULT CALLBACK WindowFrame::WindowProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// This is the static window proc which allows the actual window proc to exist within a class

	if (msg == WM_CREATE)
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)(LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);

	return ((WindowFrame*)(LONG_PTR)::GetWindowLongPtr(hwnd, GWLP_USERDATA))->WindowProc(hwnd, msg, wParam, lParam);
}

/****************************************************************************************************************/

// This is the window proc

LRESULT CALLBACK WindowFrame::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hwnd, msg, wParam, lParam); 
}

// Presets

WindowFrame WindowFrame::SystemSizable;
WindowFrame WindowFrame::SystemFixed;
WindowFrame WindowFrame::None;