#ifndef OP_RESIZE_H
#define OP_RESIZE_H

#include <Operators\Operator.h>
#include <Properties\IntProperty\IntProperty.h>
#include <Properties\ListProperty\ListProperty.h>
#include <Properties\BoolProperty\BoolProperty.h>
#include <Types/Geometric\Region.h>

#include <Types/Graphics\Image8.h>
#include <Types/Geometric\FSize.h>
#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Controls\Button\Button.h>

class OpResize
	: public Operator
{
	DECLARE_TYPE;

public:
	enum _Interpolation { Nearest, Bilinear, Bicubic };

public:
	OpResize();
	OpResize(const OpResize& op);
	~OpResize();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	Object*	Copy();

	void	CopyWorkingValues();
	void	Process();

	IntProperty	Width;
	IntProperty	Height;

	ListProperty Interpolation;

	BoolProperty  Subpixel;

	void	in_OnConnect(Input::Message& msg);

private:
	Input	m_in;
	Output	m_out;

	FSize	m_refSize;
	bool	m_init; // Decides whether to set the initial dimensions

	Button m_btnProportional;
	void btnProportional_OnPress(Control::Message& msg);

	static GdiImage8 s_imgProportional;

	LineLayout m_layoutDimensions;
	LineLayout m_layoutDimensions2;

	void Width_Valuebox_PreValueChange(ValueMessage& msg);
	void Width_Valuebox_OnValueChange(ValueMessage& msg);

	void Height_Valuebox_PreValueChange(ValueMessage& msg);
	void Height_Valuebox_OnValueChange(ValueMessage& msg);

	void Interpolation_OnSelectItem(::Message& msg);
	Graphics::_Interpolation _interpolation;

	/*	Working values. */
	int m_width;
	int m_height;
	Graphics::_Interpolation m_interpolation;
	bool m_subpixel;

	//

	void Resize_NoResize(
		int* outBuffer,
		int outWidth, int outHeight,
		int* inBuffer,
		int inBufferWidth, int inBufferHeight,
		int inX, int inY,
		int inWidth, int inHeight,
		Region& clip);
	
	void Resize_Nearest(
		int* outBuffer,
		int outWidth, int outHeight,
		int* inBuffer,
		int inBufferWidth, int inBufferHeight,
		int inX, int inY,
		int inWidth, int inHeight,
		Region& clip);
	
	void Resize_Bilinear(
		int* outBuffer,
		int outWidth, int outHeight,
		int* inBuffer,
		int inBufferWidth, int inBufferHeight,
		int inX, int inY,
		int inWidth, int inHeight,
		Region& clip);
};

#endif