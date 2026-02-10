#include "HoverButton.h"

Skin HoverButton::s_skinButtonHover(Resource::LoadImage(IDB_CONTROL_HOVERBUTTON_HOVER), skinButtonHover_Init);
Skin HoverButton::s_skinButtonHighlight(Resource::LoadImage(IDB_CONTROL_HOVERBUTTON_HIGHLIGHT), skinButtonHover_Init);

void HoverButton::skinButtonHover_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 46;
	skin->Height = 46;

	skin->LW = 6;
	skin->RW = 6;
	skin->TH = 6;
	skin->BH = 6;

	skin->OX = -1;	
	skin->OY = -1;	
	skin->OW = 2;
	skin->OH = 2;
}

Skin HoverButton::s_skinButtonPressed(Resource::LoadImage(IDB_CONTROL_HOVERBUTTON_PRESSED), skinButtonPressed_Init);

void HoverButton::skinButtonPressed_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 44;
	skin->Height = 44;

	skin->LW = 5;
	skin->RW = 5;
	skin->TH = 5;
	skin->BH = 5;
}

Skin HoverButton::s_skinButtonBack(Resource::LoadImage(IDB_CONTROL_HOVERBUTTON_BACK), skinButtonBack_Init);

void HoverButton::skinButtonBack_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 9;
	skin->Height = 9;

	skin->LW = 4;
	skin->RW = 4;
	skin->TH = 4;
	skin->BH = 4;
}