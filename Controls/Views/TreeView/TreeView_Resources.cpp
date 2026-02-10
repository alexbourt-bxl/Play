#include "TreeView.h"

Skin TreeView::s_skinOp(Resource::LoadImage(IDB_OPERATOR), skinOp_Init);
Skin TreeView::s_skinOpActive(Resource::LoadImage(IDB_OPERATOR_ACTIVE), skinOp_Init);
Skin TreeView::s_skinOpShown(Resource::LoadImage(IDB_OPERATOR_SHOWN), skinOp_Init);

void TreeView::skinOp_Init(Skin* skin)
{
	skin->X = 0;
	skin->Y = 0;
	skin->Width = 57;
	skin->Height = 65;

	skin->LW = 28;
	skin->RW = 28;
	skin->TH = 34;
	skin->BH = 30;

	skin->OX = -17;		
	skin->OY = -12;		
	skin->OW = 34;
	skin->OH = 34;
}

GdiImage8 TreeView::s_imgConnectionRing(Resource::LoadBitmap(IDB_OPERATOR_CONNECTION_RING));
GdiImage8 TreeView::s_imgConnectionPlus(Resource::LoadBitmap(IDB_OPERATOR_CONNECTION_PLUS));
GdiImage8 TreeView::s_imgConnectedCircle(Resource::LoadBitmap(IDB_OPERATOR_CONNECTED_CIRCLE));
GdiImage8 TreeView::s_imgConnectedArrow(Resource::LoadBitmap(IDB_CONTROL_TreeView_CONNECTIONARROW));