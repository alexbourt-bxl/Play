#ifndef STRIP_H
#define STRIP_H

#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Graphics\Graphics.h>
#include <Types/Graphics\Skin.h>

class Strip
	: public LineLayout
{
	DECLARE_TYPE;

public:
	Strip();
	~Strip();

	bool	GetIgnoreParentClip();

	__declspec(property(get = GetNeckSkin, put = SetNeckSkin))
	Skin*	NeckSkin;
	Skin*	GetNeckSkin() const;
	void	SetNeckSkin(Skin* skin);

private:
	Skin*	m_skinNeck;

	void	OnPaint(Graphics::Message& msg);
};

#endif