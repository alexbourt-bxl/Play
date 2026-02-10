#include "OpCrop.h"

GdiImage8 OpCrop::s_imgTL(Resource::LoadBitmap(IDB_OP_CROP_TL));
GdiImage8 OpCrop::s_imgTR(Resource::LoadBitmap(IDB_OP_CROP_TR));
GdiImage8 OpCrop::s_imgBL(Resource::LoadBitmap(IDB_OP_CROP_BL));
GdiImage8 OpCrop::s_imgBR(Resource::LoadBitmap(IDB_OP_CROP_BR));

//Skin OpCrop::s_skinShadow(Resource::LoadImage(IDB_OP_CROP_SHADOW), skinShadow_Init);
//
//void OpCrop::skinShadow_Init(Skin* skin)
//{
//	skin->X = 0;
//	skin->Y = 0;
//	skin->Width = 39;
//	skin->Height = 39;
//
//	skin->LW = 19;
//	skin->RW = 19;
//	skin->TH = 24;
//	skin->BH = 14;
//}