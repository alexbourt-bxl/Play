#include "Strip.h"

#include <Types/Graphics\Skin.h>

DEFINE_TYPE(Strip, LineLayout);

Strip::Strip()
{
	m_skinNeck = null;

	PaintEvent += EventHandler(this, &Strip::OnPaint);

	SuspendUpdates();

	Padding = 3;
	AutoSize = true;
	Orientation = LineLayout::Vertical;
	Wrap = false;

	ResumeUpdates();
}

Strip::~Strip()
{

}

bool Strip::GetIgnoreParentClip()
{
	return true;
}


Skin* Strip::GetNeckSkin() const
{
	return m_skinNeck;
}

void Strip::SetNeckSkin(Skin* skin)
{
	m_skinNeck = skin;
}