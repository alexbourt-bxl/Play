#include "Skin.h"

#include <Types/Graphics\Image.h>

#include <assert.h>

DEFINE_TYPE(Skin, Object);

Skin::Skin()
{
	m_image = null;

	X      = 0;	LW = 0;	OX = 0; CX = 0;
	Y      = 0;	RW = 0;	OY = 0; CY = 0;
	Width  = 0;	TH = 0;	OW = 0; CW = 0;
	Height = 0;	BH = 0;	OH = 0; CH = 0;

	m_initProc = null;
}

Skin::Skin(const Skin& skin)
{
	m_image = skin.m_image;

	X      = skin.X;		LW = skin.LW;	OX = skin.OX;	CX = skin.OX;
	Y      = skin.Y;		RW = skin.RW;	OY = skin.OY;	CY = skin.OY;
	Width  = skin.Width;	TH = skin.TH;	OW = skin.OW;	CW = skin.CW;
	Height = skin.Height;	BH = skin.BH;	OH = skin.OH;	CH = skin.CH;

	m_initProc = skin.m_initProc;

	if (m_initProc)
		m_initProc(this);
}

Skin::Skin(_Image* image, SkinInitProc initProc)
{
	m_image	= image;

	OX = 0;	CX = 0;
	OY = 0;	CY = 0;
	OW = 0;	CW = 0;
	OH = 0;	CH = 0;

	m_initProc = initProc;

	if (m_initProc)
		m_initProc(this);
}

Skin::~Skin()
{
	if (m_image)
		delete m_image;
}

Object* Skin::Copy()
{
	return new Skin(*this);
}

Image* Skin::GetImage() const
{
	return m_image;
}
