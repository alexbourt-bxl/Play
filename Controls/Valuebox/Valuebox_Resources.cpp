#include "Valuebox.h"

Skin Valuebox::s_skinBorder(Resource::LoadImage(IDB_CONTROL_TEXTBOX_BORDER), skinBorder_Init);

void Valuebox::skinBorder_Init(Skin* skin)
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

GdiImage8 Valuebox::s_imgShadowL(Resource::LoadBitmap(IDB_CONTROL_VALUEBOX_SHADOW_LEFT));
GdiImage8 Valuebox::s_imgShadowR(Resource::LoadBitmap(IDB_CONTROL_VALUEBOX_SHADOW_RIGHT));
GdiImage8 Valuebox::s_imgAbsolute(Resource::LoadBitmap(IDB_CONTROL_VALUEBOX_ABSOLUTE));

GdiImage8 Valuebox::s_imgArrowLeft(Resource::LoadBitmap(IDB_CONTROL_VALUEBOX_ARROW_LEFT));
GdiImage8 Valuebox::s_imgArrowRight(Resource::LoadBitmap(IDB_CONTROL_VALUEBOX_ARROW_RIGHT));
GdiImage8 Valuebox::s_imgArrowUp(Resource::LoadBitmap(IDB_CONTROL_VALUEBOX_ARROW_UP));
GdiImage8 Valuebox::s_imgArrowDown(Resource::LoadBitmap(IDB_CONTROL_VALUEBOX_ARROW_DOWN));