#include "ColorButton.h"

Skin ColorButton::s_skinButton(Resource::LoadImage(IDB_CONTROL_COLORBUTTON), skinButton_Init);
Skin ColorButton::s_skinButtonPressed(Resource::LoadImage(IDB_CONTROL_COLORBUTTON_PRESSED), skinButton_Init);

void ColorButton::skinButton_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 39;
	skin->Height = 40;

	skin->LW = 19;
	skin->RW = 19;
	skin->TH = 18;
	skin->BH = 21;

	skin->OX = -2;		
	skin->OY = -1;		
	skin->OW = 4;
	skin->OH = 5;
}

Skin ColorButton::s_skinHint(Resource::LoadImage(IDB_CONTROL_COLORBUTTON_EXPANDS), skinHint_Init);

void ColorButton::skinHint_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 35;
	skin->Height = 35;

	skin->LW = 17;
	skin->RW = 17;
	skin->TH = 17;
	skin->BH = 17;
}

Skin ColorButton::s_skinStripNeck(Resource::LoadImage(IDB_CONTROL_COLORBUTTON_NECK), skinStripNeck_Init);

void ColorButton::skinStripNeck_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 41;
	skin->Height = 28;

	skin->LW = 20;
	skin->RW = 20;
	skin->TH = 6;
	skin->BH = 21;
}