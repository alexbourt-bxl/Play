#ifndef OP_SKIN_H
#define OP_SKIN_H

#include <Operators\OpGraph\OpGraph.h>
#include <Operators\Image\OpCrop\OpCrop.h>
#include <Operators\Image\OpResize\OpResize.h>
#include <Operators\Image\OpOffset\OpOffset.h>
#include <Operators\Image\OpColor\OpColor.h>
#include <Operators\Image\OpComposite\OpComposite.h>
#include <Operators\OpNull\OpNull.h>
#include <Types\Properties\IntProperty\IntProperty.h>

class OpSkin
	: public OpGraph
{
	DECLARE_TYPE;

public:
	OpSkin();
	OpSkin(const OpSkin& op);
	~OpSkin();

	String	GetOpName();

	void	Init();
	Object*	Copy();

	void	UpdateResult(SpecList& specs);

	IntProperty		Width;
	IntProperty		Height;

	IntProperty		SourceX;
	IntProperty		SourceY;
	IntProperty		SourceWidth;
	IntProperty		SourceHeight;

	IntProperty		LeftWidth;
	IntProperty		RightWidth;
	IntProperty		TopHeight;
	IntProperty		BottomHeight;

	/*	Outer dimensions are used for 
	offset and clip region. */

	IntProperty		OuterLeft;
	IntProperty		OuterRight;
	IntProperty		OuterTop;
	IntProperty		OuterBottom;

	void Input_OnConnect(Input::EventArgs& e);

private:
	Output	m_out;

	OpNull	m_opNull; // used to route the input to the crops

	OpColor	m_opColor;

	OpCrop	m_opCropTL;	OpResize m_opResizeTL;	OpOffset m_opOffsetTL;	
	OpCrop	m_opCropT;	OpResize m_opResizeT;	OpOffset m_opOffsetT;	
	OpCrop	m_opCropTR;	OpResize m_opResizeTR;	OpOffset m_opOffsetTR;	
	OpCrop	m_opCropL;	OpResize m_opResizeL;	OpOffset m_opOffsetL;	
	OpCrop	m_opCropC;	OpResize m_opResizeC;	OpOffset m_opOffsetC;	
	OpCrop	m_opCropR;	OpResize m_opResizeR;	OpOffset m_opOffsetR;	
	OpCrop	m_opCropBL;	OpResize m_opResizeBL;	OpOffset m_opOffsetBL;	
	OpCrop	m_opCropB;	OpResize m_opResizeB;	OpOffset m_opOffsetB;	
	OpCrop	m_opCropBR;	OpResize m_opResizeBR;	OpOffset m_opOffsetBR;
	// TODO: replace all the Overs with a Composite

	OpComposite m_opComp;
	OpOffset m_opOffset;
};

#endif