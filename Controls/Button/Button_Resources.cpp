#include "Button.h"

Skin Button::s_skinButton(Resource::LoadImage(IDB_CONTROL_BUTTON), skinButton_Init);
Skin Button::s_skinButtonPressed(Resource::LoadImage(IDB_CONTROL_BUTTON_PRESSED), skinButton_Init);
Skin Button::s_skinButtonHighlight(Resource::LoadImage(IDB_CONTROL_BUTTON_HIGHLIGHT), skinButton_Init);

void Button::skinButton_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width	= 39;
	skin->Height = 40;

	skin->LW = 7;
	skin->RW = 7;
	skin->TH = 6;
	skin->BH = 9;

	skin->OX = -2;		
	skin->OY = -1;		
	skin->OW = 4;
	skin->OH = 5;
}

Skin Button::s_skinSelected(Resource::LoadImage(IDB_CONTROL_BUTTON_SELECTED), skinSelected_Init);

void Button::skinSelected_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 41;
	skin->Height = 41;

	skin->LW = 8;
	skin->RW = 8;
	skin->TH = 8;
	skin->BH = 8;

	skin->OX = -3;		
	skin->OY = -3;		
	skin->OW = 6;
	skin->OH = 6;
}