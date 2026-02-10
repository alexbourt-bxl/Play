#include "OpSkin.h"

#include <Types\Graphics\Image.h>

#include <assert.h>

DEFINE_TYPE(OpSkin, OpGraph);

OpSkin::OpSkin() 
{
	Init();
}

OpSkin::OpSkin(const OpSkin& op) 
{
	Init();
}

OpSkin::~OpSkin()
{

}

String OpSkin::GetOpName()
{
	return L"Skin";
}

void OpSkin::Init()
{
	LeftWidth   = 0;
	RightWidth  = 0;
	TopHeight   = 0;
	BottomHeight = 0;

	OuterLeft   = 0;
	OuterRight  = 0;
	OuterTop    = 0;
	OuterBottom = 0;

	Width.Name = L"Width";
	Width.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&Width);

	Height.Name = L"Height";
	Height.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&Height);

	SourceX.Name = L"Source X";
	SourceX.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&SourceX);

	SourceY.Name = L"Source Y";
	SourceY.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&SourceY);

	SourceWidth.Name = L"Source Width";
	SourceWidth.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&SourceWidth);

	SourceHeight.Name = L"Source Height";
	SourceHeight.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&SourceHeight);

	LeftWidth.Name = L"Left Width";
	LeftWidth.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&LeftWidth);

	RightWidth.Name = L"Right Width";
	RightWidth.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&RightWidth);

	TopHeight.Name = L"Top Height";
	TopHeight.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&TopHeight);

	BottomHeight.Name = L"Bottom Height";
	BottomHeight.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&BottomHeight);

	OuterLeft.Name = L"Outer Left";
	OuterLeft.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&OuterLeft);

	OuterRight.Name = L"Outer Right";
	OuterRight.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&OuterRight);

	OuterTop.Name = L"Outer Top";
	OuterTop.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&OuterTop);

	OuterBottom.Name = L"Outer Bottom";
	OuterBottom.ChangeEvent += EventHandler(this, &Operator::OnPropertyChange);
	Properties.Add(&OuterBottom);

	/*	An op graph is usually responsible for deleting the operators it contains,
	but in this case they are stored locally, so OpGraph should leave them alone. */
	OpGraph::RemoveEvent->Clear();

	Add(&m_opNull);

	m_opColor.Color_ = Color::Transparent;
	Add(&m_opColor);

	Add(&m_opCropTL);	Add(&m_opResizeTL);	Add(&m_opOffsetTL);
	Add(&m_opCropT);	Add(&m_opResizeT);	Add(&m_opOffsetT);
	Add(&m_opCropTR);	Add(&m_opResizeTR);	Add(&m_opOffsetTR);
	Add(&m_opCropL);	Add(&m_opResizeL);	Add(&m_opOffsetL);
	Add(&m_opCropC);	Add(&m_opResizeC);	Add(&m_opOffsetC);
	Add(&m_opCropR);	Add(&m_opResizeR);	Add(&m_opOffsetR);
	Add(&m_opCropBL);	Add(&m_opResizeBL);	Add(&m_opOffsetBL);
	Add(&m_opCropB);	Add(&m_opResizeB);	Add(&m_opOffsetB);
	Add(&m_opCropBR);	Add(&m_opResizeBR);	Add(&m_opOffsetBR);

	Add(&m_opOffset);
	Add(&m_opComp);

	In.Add(m_opNull.In[0]);
	Out.Add(&m_out);

	m_opNull.In[0]->ConnectEvent += EventHandler(this, &OpSkin::Input_OnConnect);

	/*	Set up the operators. */

	m_opCropTL.ChangeOffset = false;	m_opResizeTL.ChangeOffset = false;
	m_opCropT.ChangeOffset  = false;	m_opResizeT.ChangeOffset  = false;
	m_opCropTR.ChangeOffset = false;	m_opResizeTR.ChangeOffset = false;
	m_opCropL.ChangeOffset  = false;	m_opResizeL.ChangeOffset  = false;
	m_opCropC.ChangeOffset  = false;	m_opResizeC.ChangeOffset  = false;
	m_opCropR.ChangeOffset  = false;	m_opResizeR.ChangeOffset  = false;
	m_opCropBL.ChangeOffset = false;	m_opResizeBL.ChangeOffset = false;
	m_opCropB.ChangeOffset  = false;	m_opResizeB.ChangeOffset  = false;
	m_opCropBR.ChangeOffset = false;	m_opResizeBR.ChangeOffset = false;
	
	// Set up the op graph

	m_opColor.Out >> m_opComp.In;

	m_opNull.Out >> m_opCropTL.In;	m_opCropTL.Out >> m_opResizeTL.In;	m_opResizeTL.Out >> m_opOffsetTL.In;	m_opOffsetTL.Out >> m_opComp.In;
	m_opNull.Out >> m_opCropT.In;	m_opCropT.Out  >> m_opResizeT.In;	m_opResizeT.Out  >> m_opOffsetT.In;		m_opOffsetT.Out  >> m_opComp.In;
	m_opNull.Out >> m_opCropTR.In;	m_opCropTR.Out >> m_opResizeTR.In;  m_opResizeTR.Out >> m_opOffsetTR.In;	m_opOffsetTR.Out >> m_opComp.In;	
	m_opNull.Out >> m_opCropL.In;	m_opCropL.Out  >> m_opResizeL.In;	m_opResizeL.Out  >> m_opOffsetL.In;		m_opOffsetL.Out  >> m_opComp.In;
	m_opNull.Out >> m_opCropC.In;	m_opCropC.Out  >> m_opResizeC.In;	m_opResizeC.Out  >> m_opOffsetC.In;		m_opOffsetC.Out  >> m_opComp.In;
	m_opNull.Out >> m_opCropR.In;	m_opCropR.Out  >> m_opResizeR.In;	m_opResizeR.Out  >> m_opOffsetR.In;		m_opOffsetR.Out  >> m_opComp.In;
	m_opNull.Out >> m_opCropBL.In;	m_opCropBL.Out >> m_opResizeBL.In;	m_opResizeBL.Out >> m_opOffsetBL.In;	m_opOffsetBL.Out >> m_opComp.In;
	m_opNull.Out >> m_opCropB.In;	m_opCropB.Out  >> m_opResizeB.In;	m_opResizeB.Out  >> m_opOffsetB.In;		m_opOffsetB.Out  >> m_opComp.In;
	m_opNull.Out >> m_opCropBR.In;	m_opCropBR.Out >> m_opResizeBR.In;	m_opResizeBR.Out >> m_opOffsetBR.In;	m_opOffsetBR.Out >> m_opComp.In;

	m_opComp.Out >> m_opOffset.In;

	/*	TODO:

		remove color from this setup, so that OpComposite has to figure out
		based on the total extent of its layers the size of its result.

		The result will be unnecessary of a TargetSpec is specified. */
}

Object* OpSkin::Copy()
{
	return OpSkin(*this);
}

void OpSkin::UpdateResult(SpecList& _specs)
{
	SpecList specs = _specs;

	Image* in = (Image*)m_opNull.In[0]->GetInputObject(specs);

	if (!in)
		return;

	/*	TODO: When squeezing, cut up the skin so that the outer edge
	is scaled separately from the corner images, because the corners
	have to be scaled evenly. Right now if the bottom shadow is thicker
	than the top shadow, then the bottom edge is squeezed less visually
	than the top edge. */

	m_opColor.Width  = Width + OuterLeft + (int)OuterRight;
	m_opColor.Height = Height + OuterTop + (int)OuterBottom;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int _topBottom    = SourceY + TopHeight;
	int _leftRight    = SourceX + LeftWidth;
	int _centerLeft   = SourceX + LeftWidth;
	int _centerRight  = SourceX + SourceWidth - RightWidth;
	int _centerTop    = SourceY + TopHeight;
	int _centerBottom = SourceY + SourceHeight - BottomHeight;
	int _rightLeft    = SourceX + SourceWidth - RightWidth;
	int _rightRight   = SourceX + SourceWidth;
	int _bottomTop    = SourceY + SourceHeight - BottomHeight;
	int _bottomBottom = SourceY + SourceHeight;

	m_opCropTL.Left   = SourceX;			m_opCropT.Left   = _centerLeft;			m_opCropTR.Left   = _rightLeft;
	m_opCropTL.Top    = SourceY;			m_opCropT.Top    = SourceY;				m_opCropTR.Top    = SourceY;
	m_opCropTL.Right  = _leftRight;			m_opCropT.Right  = _centerRight;		m_opCropTR.Right  = _rightRight;
	m_opCropTL.Bottom = _topBottom;			m_opCropT.Bottom = _topBottom;			m_opCropTR.Bottom = _topBottom;

	m_opCropL.Left    = SourceX;			m_opCropC.Left   = _centerLeft;			m_opCropR.Left    = _rightLeft;
	m_opCropL.Top     = _centerTop;			m_opCropC.Top    = _centerTop;			m_opCropR.Top     = _centerTop;
	m_opCropL.Right   = _leftRight;			m_opCropC.Right  = _centerRight;		m_opCropR.Right   = _rightRight;
	m_opCropL.Bottom  = _centerBottom;		m_opCropC.Bottom = _centerBottom;		m_opCropR.Bottom  = _centerBottom;

	m_opCropBL.Left   = SourceX;			m_opCropB.Left   = _centerLeft;			m_opCropBR.Left   = _rightLeft;
	m_opCropBL.Top    = _bottomTop;			m_opCropB.Top    = _bottomTop;			m_opCropBR.Top    = _bottomTop;
	m_opCropBL.Right  = _leftRight;			m_opCropB.Right  = _centerRight;		m_opCropBR.Right  = _rightRight;
	m_opCropBL.Bottom = _bottomBottom;		m_opCropB.Bottom = _bottomBottom;		m_opCropBR.Bottom = _bottomBottom;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float _leftSqueeze   = (float)(Width - RightWidth) / (float)LeftWidth;
	float _rightSqueeze  = (float)(Width - LeftWidth) / (float)RightWidth;
	float _topSqueeze    = (float)(Height - BottomHeight) / (float)TopHeight;
	float _bottomSqueeze = (float)(Height - TopHeight) / (float)BottomHeight;

	int _leftWidth    = min(LeftWidth, Math::Round((float)LeftWidth * _leftSqueeze));
	int _rightWidth   = min(RightWidth, (int)((float)RightWidth * _rightSqueeze));
	int _topHeight    = min(TopHeight, Math::Round((float)TopHeight * _topSqueeze));
	int _bottomHeight = min(BottomHeight, (int)((float)BottomHeight * _bottomSqueeze));

	int _widthC  = max(0, Width - LeftWidth - RightWidth);
	int _heightC = max(0, Height - TopHeight - BottomHeight);

	m_opResizeTL.Width  = _leftWidth;		m_opResizeT.Width  = _widthC;		m_opResizeTR.Width  = _rightWidth;
	m_opResizeTL.Height = _topHeight;		m_opResizeT.Height = _topHeight;	m_opResizeTR.Height = _topHeight;

	m_opResizeL.Width   = _leftWidth;		m_opResizeC.Width  = _widthC;		m_opResizeR.Width   = _rightWidth;
	m_opResizeL.Height  = _heightC;			m_opResizeC.Height = _heightC;		m_opResizeR.Height  = _heightC;

	m_opResizeBL.Width  = _leftWidth;		m_opResizeB.Width  = _widthC;		m_opResizeBR.Width  = _rightWidth;
	m_opResizeBL.Height = _bottomHeight;	m_opResizeB.Height = _bottomHeight;	m_opResizeBR.Height = _bottomHeight;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int _rightOffset = Width - RightWidth;
	int _bottomOffset = Height - BottomHeight;

	m_opOffsetTL.X = 0;						m_opOffsetT.X = _leftWidth;			m_opOffsetTR.X = _rightOffset;
	m_opOffsetTL.Y = 0;						m_opOffsetT.Y = 0;					m_opOffsetTR.Y = 0;

	m_opOffsetL.X  = 0;						m_opOffsetC.X = _leftWidth;			m_opOffsetR.X  = _rightOffset;
	m_opOffsetL.Y  = _topHeight;			m_opOffsetC.Y = _topHeight;			m_opOffsetR.Y  = _topHeight;

	m_opOffsetBL.X = 0;						m_opOffsetB.X = _leftWidth;			m_opOffsetBR.X = _rightOffset;
	m_opOffsetBL.Y = _bottomOffset;			m_opOffsetB.Y = _bottomOffset;		m_opOffsetBR.Y = _bottomOffset;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_opOffset.X = -OuterLeft;
	m_opOffset.Y = -OuterTop;

	m_opComp.Update(specs);

	m_out.SetResultTransitive(
		m_opComp.Out[0]->Result, 
		m_opComp.Out[0]->Specs);
}