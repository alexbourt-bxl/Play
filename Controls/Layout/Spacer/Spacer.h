#ifndef SPACER_H
#define SPACER_H

#include <Controls\Control.h>
#include <Graphics\Graphics.h>

class Spacer 
	: public Control
{
	DECLARE_TYPE;

	friend class LineLayout;

public:
	Spacer();

	void	SetSize(int width, int height);

	__declspec(property(get = GetCanShrink, put = SetCanShrink))
	bool	CanShrink;
	bool	GetCanShrink();
	void	SetCanShrink(bool canShrink);

private:
	bool	m_canShrink;

	double	m_weight;

	int		m_setWidth;
	int		m_setHeight;

	//

	void	OnPaint(Graphics::Message& msg);

	void	Width_OnChange(Property::Message& msg);
	void	Height_OnChange(Property::Message& msg);
};

#endif