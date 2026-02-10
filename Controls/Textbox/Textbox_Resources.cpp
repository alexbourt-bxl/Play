#include "Textbox.h"

#include <Resources\Resource.h>

Cursor Textbox::s_curText(IDC_TEXT);

Skin Textbox::s_skinBorder(Resource::LoadImage(IDB_CONTROL_TEXTBOX_BORDER), skinBorder_Init);

void Textbox::skinBorder_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 13;
	skin->Height = 11;

	skin->LW = 6;
	skin->RW = 6;
	skin->TH = 5;
	skin->BH = 5;

	skin->OX = -1;		
	skin->OY = -1;		
	skin->OW = 2;
	skin->OH = 2;
}

Skin Textbox::s_skinSelShadow(Resource::LoadImage(IDB_CONTROL_TEXTBOX_SELECTION_SHADOW), skinSelShadow_Init);

void Textbox::skinSelShadow_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 29;
	skin->Height = 27;

	skin->LW = 10;
	skin->RW = 10;
	skin->TH = 10;
	skin->BH = 10;

	skin->OX = -5;		
	skin->OY = -3;		
	skin->OW = 10;
	skin->OH = 10;
}