#ifndef LABEL_H
#define LABEL_H

#include <Controls\TextControl\TextControl.h>

class Label 
	: public TextControl
{
	DECLARE_TYPE;

public:
	Label();

	_Rect	GetFullPaintRect();
	bool	AdjustSize(_Size& size);

	__declspec(property(get = GetAutoSize, put = SetAutoSize))
	bool	AutoSize;
	bool	GetAutoSize();
	void	SetAutoSize(bool autoSize);

	__declspec(property(get = GetWrap, put = SetWrap))
	bool	Wrap;
	bool	GetWrap();
	void	SetWrap(bool wrap);

private:
	bool	m_autoSize;
	bool	m_wrap;

	void	OnPaint(Graphics::Message& msg);
};

#endif