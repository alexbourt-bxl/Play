#include "ListButton.h"

Skin ListButton::s_skinHint(Resource::LoadImage(IDB_CONTROL_LISTBUTTON_EXPANDS), skinHint_Init);

void ListButton::skinHint_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 35;
	skin->Height = 35;

	skin->LW = 5;
	skin->RW = 5;
	skin->TH = 5;
	skin->BH = 5;
}

Skin ListButton::s_skinStripNeck(Resource::LoadImage(IDB_CONTROL_LISTBUTTON_STRIP_NECK), skinStripNeck_Init);

void ListButton::skinStripNeck_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 13;
	skin->Height = 13;

	skin->LW = 6;
	skin->RW = 6;
	skin->TH = 6;
	skin->BH = 6;
}