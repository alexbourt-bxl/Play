#include "Cursor.h"

Cursor::Cursor()
{
	m_hcursor = null;
}

Cursor::Cursor(WORD id, bool animated)
{
	m_hcursor = animated
		? Resource::LoadAnimatedCursor(id)
		: Resource::LoadCursor(id);
}

Cursor::~Cursor()
{
	if (m_hcursor)
		::DestroyCursor(m_hcursor);
}

HCURSOR Cursor::ToHCURSOR()
{
	return m_hcursor;
}

// Presets

Cursor Cursor::Arrow		= Cursor(IDC_NORMAL);
Cursor Cursor::Ghost		= Cursor(IDC_GHOST);
Cursor Cursor::Aim			= Cursor(IDC_AIM);
Cursor Cursor::Hand			= Cursor(IDC_HAND_NORMAL);
Cursor Cursor::HandPressed	= Cursor(IDC_HAND_PRESSED);
Cursor Cursor::Magnify		= Cursor(IDC_MAGNIFY);
Cursor Cursor::Move			= Cursor(IDC_MOVE);
Cursor Cursor::ScaleLR		= Cursor(IDC_SCALE_LR);
Cursor Cursor::ScaleTB		= Cursor(IDC_SCALE_TB);
Cursor Cursor::ScaleTLBR	= Cursor(IDC_SCALE_TLBR);
Cursor Cursor::ScaleBLTR	= Cursor(IDC_SCALE_BLTR);
Cursor Cursor::RotateTL		= Cursor(IDC_ROTATE_TL);
Cursor Cursor::RotateT		= Cursor(IDC_ROTATE_T);
Cursor Cursor::RotateTR		= Cursor(IDC_ROTATE_TR);
Cursor Cursor::RotateR		= Cursor(IDC_ROTATE_R);
Cursor Cursor::RotateBR		= Cursor(IDC_ROTATE_BR);
Cursor Cursor::RotateB		= Cursor(IDC_ROTATE_B);
Cursor Cursor::RotateBL		= Cursor(IDC_ROTATE_BL);
Cursor Cursor::RotateL		= Cursor(IDC_ROTATE_L);
Cursor Cursor::Dot			= Cursor(IDC_DOT);
Cursor Cursor::LargeDot		= Cursor(IDC_DOT_LARGE);
Cursor Cursor::Add          = Cursor(IDC_ADD);
Cursor Cursor::Remove       = Cursor(IDC_REMOVE);
Cursor Cursor::Complete		= Cursor(IDC_COMPLETE);
Cursor Cursor::Adjust		= Cursor(IDC_ADJUST);