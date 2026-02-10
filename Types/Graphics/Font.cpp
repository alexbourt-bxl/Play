#include "Font.h"

#include <Graphics\Graphics.h>
#include <Native\Native.h>

DEFINE_TYPE(Font, Object);

Font::Font()
{
	Construct(L"Segoe UI", 16, Font::Normal);
}

Font::Font(const Font& font)
{
	Construct(font.m_name, font.m_size, font.m_weight);
}

Font::Font(const String& name, double size, _Weight weight)
{
	Construct(name, size, weight);
}

Font::~Font()
{
	Destruct();
}

Object* Font::Copy()
{
	return new Font(*this);
}

void Font::Construct(const String& name, double size, _Weight weight)
{
	m_mapMode = MM_TEXT;

	m_name   = name;
	m_size   = size;
	m_weight = weight;

	LOGFONT lf;

	lf.lfHeight		 = (LONG)m_size; // = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72); for Unit::Point
	lf.lfWidth		 = 0;
	lf.lfEscapement	 = 0;
	lf.lfOrientation = 0;

	switch (m_weight)
	{
	case Font::Normal:
		lf.lfWeight	= FW_NORMAL;
		break;

	case Font::SemiBold:
		lf.lfWeight	= FW_SEMIBOLD;
		break;

	case Font::Bold:
		lf.lfWeight	= FW_BOLD;
		break;
	}
	
	lf.lfItalic			= FALSE;
	lf.lfUnderline		= FALSE;
	lf.lfStrikeOut		= FALSE;
	lf.lfCharSet		= DEFAULT_CHARSET;
	lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lf.lfQuality		= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
	
	wcsncpy_s(
		lf.lfFaceName, sizeof(lf.lfFaceName) / sizeof(lf.lfFaceName[0]), 
		(wchar_t*)m_name, min(m_name.Length + 1, sizeof(lf.lfFaceName) / sizeof(lf.lfFaceName[0])));

	m_hfont = ::CreateFontIndirect(&lf);
}

void Font::Destruct()
{
	::DeleteObject(m_hfont);
}

Font& Font::operator = (const Font& font)
{
	Destruct();
	Construct(font.m_name, font.m_size, font.m_weight);

	return *this;
}

String Font::GetName()
{
	return m_name;
}

double Font::GetSize()
{
	return m_size;
}

Font::_Weight Font::GetWeight()
{
	return m_weight;
}

int Font::GetAverageCharWidth()
{
	TEXTMETRIC tm;

	HDC hdc = ::GetDC(null);
	HFONT hfOld = (HFONT)::SelectObject(hdc, m_hfont);

	::GetTextMetrics(hdc, &tm);

	::SelectObject(hdc, hfOld);
	::ReleaseDC(null, hdc);

	return tm.tmAveCharWidth;
}

// Presets

Font Font::Default(L"Segoe UI", 16, Font::Normal);
Font Font::Label(L"Segoe UI", 20, Font::Bold);
Font Font::Button(L"Segoe UI", 18, Font::Bold);