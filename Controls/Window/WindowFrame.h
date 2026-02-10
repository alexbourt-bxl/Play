#ifndef WINDOWFRAME_H
#define WINDOWFRAME_H

// ---------------------------------------
// |                top                  |
// |-------------------------------------|
// |        |                  |         |
// |        |                  |         |
// |  left  |     (center)     |  right  |
// |        |                  |         |
// |        |                  |         |
// |-------------------------------------|
// |               bottom                |
// ---------------------------------------

#include <Types/Object.h>
#include <Types/Geometric\Rect.h>

//#include "Graphics.h"
#include <windows.h>

class Window;
class GdiImage8;

class WindowFrame
	: public Object
{
	DECLARE_TYPE;

	friend class Window;

public:
	WindowFrame(/*Skin* sknActive = null, Skin* sknInactive = null*/);
	~WindowFrame();

	Object*		Copy();

	__declspec(property(get = GetIsSystemFrame))
	bool		IsSystemFrame;
	bool		GetIsSystemFrame();

	void		Update(const Point& position, const Size& size, bool updatePosition, bool updateSize, bool active);

private:
	//Skin*		m_sknActive;
	//Skin*		m_sknInactive;

	HWND		m_hwndTop;
	HWND		m_hwndBottom;
	HWND		m_hwndLeft;
	HWND		m_hwndRight;

	GdiImage8*	m_imgTop;
	GdiImage8*	m_imgBottom;
	GdiImage8*	m_imgLeft;
	GdiImage8*	m_imgRight;

	bool		m_active;

public:
	static LRESULT CALLBACK WindowProcStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public: // Presets

	static WindowFrame SystemSizable;
	static WindowFrame SystemFixed;
	static WindowFrame None;
};

#endif