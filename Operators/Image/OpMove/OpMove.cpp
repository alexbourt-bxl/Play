#include "OpMove.h"

#include <Controls\Valuebox\Valuebox.h>
#include <Controls\Views\OpView\OpView.h>

DEFINE_TYPE(OpMove, Operator);
SET_TYPE_DISPLAY_NAME(OpMove, L"Move");

OpMove::OpMove() 
{
	X = 0;
	Y = 0;

	Init();
}

OpMove::OpMove(const OpMove& op) 
{
	X = op.X;
	Y = op.Y;

	Init();
}

OpMove::~OpMove()
{

}

Object* OpMove::Copy()
{
	return new OpMove(*this);
}

void OpMove::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image));
	In.Add(&m_in);

	Out.Add(&m_out);

	X.Name = L"X";
	Properties.Add(&X);
	
	Y.Name = L"Y";
	Properties.Add(&Y);

	m_moving = false;
}

void OpMove::LayoutControls(LineLayout* layout)
{
	X.Valuebox->AdjustSpeed = 0.5;
	X.Valuebox->Suffix = L"px";

	Y.Valuebox->AdjustSpeed = 0.5;
	Y.Valuebox->Suffix = L"px";

	m_layoutVertical.Orientation = LineLayout::Vertical;

	m_layoutVertical.Gap = 5;
	m_layoutVertical.AutoSize = true;
	m_layoutVertical.Controls.Add(X.Valuebox);
	m_layoutVertical.Controls.Add(Y.Valuebox);

	layout->Controls.Add(&m_layoutVertical);
}

void OpMove::CopyWorkingValues()
{
	m_x = X;
	m_y = Y;
}

void OpMove::Process()
{
	m_outOffset.X = m_x;
	m_outOffset.Y = m_y;

	m_out.Data = m_in.Data;
	m_out.Data.Remove(Tag::ImagePreview);

	Point* offset = m_out.Data.Get<Point>(Tag::ImageOffset);

	if (offset) 
		m_outOffset += *offset;

	m_out.Data.Set(Tag::ImageOffset, &m_outOffset);
}

void OpMove::ConnectView(OpView* view)
{
	store_allowKeyRepeat = view->AllowKeyRepeat;
	view->AllowKeyRepeat = true;

	view->LButtonDownEvent += EventHandler(this, &OpMove::OnLButtonDown);
	view->LButtonUpEvent   += EventHandler(this, &OpMove::OnLButtonUp);
	view->MouseMoveEvent   += EventHandler(this, &OpMove::OnMouseMove);
	view->KeyDownEvent     += EventHandler(this, &OpMove::OnKeyDown);
}

void OpMove::DisconnectView(OpView* view)
{
	view->LButtonDownEvent -= EventHandler(this, &OpMove::OnLButtonDown);
	view->LButtonUpEvent   -= EventHandler(this, &OpMove::OnLButtonUp);
	view->MouseMoveEvent   -= EventHandler(this, &OpMove::OnMouseMove);
	view->KeyDownEvent     -= EventHandler(this, &OpMove::OnKeyDown);

	view->AllowKeyRepeat = store_allowKeyRepeat;
}