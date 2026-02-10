#ifndef LAYOUT_H
#define LAYOUT_H

#include <Controls\Control.h>
#include <Types/Geometric\Align.h>

class Layout 
	: public Control
{
	DECLARE_TYPE;

	typedef Align _Align;

public:
	Layout();
	~Layout();

	//_Rect	GetClientRect();

	void PerformLayout();
	bool AdjustSize(_Size& size);

	/*	GetContentSize() must be provided with a reference size 
		because sometimes it is useful to know the desired size. */
	virtual _Size GetContentSize(_Size* refSize);

	__declspec(property(get = GetAlign, put = SetAlign))
	_Align	Align; // ugly, but must be so
	_Align	GetAlign();
	void	SetAlign(const _Align& align);

	__declspec(property(get = GetAutoSize, put = SetAutoSize))
	bool	AutoSize;
	bool	GetAutoSize();
	void	SetAutoSize(bool autoSize);

	__declspec(property(get = GetPadding, put = SetPadding))
	int		Padding;
	int		GetPadding();
	void	SetPadding(int padding);

	void	OnPaint(Graphics::Message& msg);
protected:
	_Align	m_align;
	bool	m_autoSize;

	int		m_padding;

	void	OnAddControl(Container::Message& msg);
	void	OnRemoveControl(Container::Message& msg);
	//void	control_OnAdjustRect(Control::SizingEvent& msg);
};

#endif