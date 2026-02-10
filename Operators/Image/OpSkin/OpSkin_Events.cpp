#include "OpSkin.h"

#include <Types\Graphics\Image.h>

void OpSkin::Input_OnConnect(Input::EventArgs& e)
{
	Image* image = (Image*)e.In->GetInputObject(SpecList::Empty);

	Width = image->Width;
	Height = image->Height;

	SourceX = 0;
	SourceY = 0;
	SourceWidth = image->Width;
	SourceHeight = image->Height;

	m_opColor.OutType = typeof(image);
}