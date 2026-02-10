//TODO: create OpImage which contains
//		  size controls and allows the image
//		  to be resized by view controls.
//
//		  OpColor, OpGradient and OpNoise et al.
//		  will derive from this.

#ifndef OP_IMAGE_H
#define OP_IMAGE_H

#include <Operators\Operator.h>
#include <Properties\IntProperty\IntProperty.h>
#include <Controls\Layout\LineLayout\LineLayout.h>

#include <Types/Graphics\Image8.h>

class ImageView;

class OpImage
	: public Operator
{
	DECLARE_TYPE;

public:
	OpImage();
	OpImage(const OpImage& op);
	~OpImage();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	IntProperty Width;
	IntProperty Height;

	void	ConnectView(OpView* view);
	void	DisconnectView(OpView* view);

protected:

	/* Working values. */
	int m_width;
	int m_height;

private:
	/*	Keep the output and its contents
		together. The op is responsible
		for cleaning up, the output won't
		delete anything. */
	Output	m_out;
	Image*	m_outImage;

	LineLayout m_layoutVertical;

	// View controls

	static const int GrabNone   = 0;
	static const int GrabLeft   = 1;
	static const int GrabRight  = 2;
	static const int GrabTop    = 4;
	static const int GrabBottom = 8;

	int		m_grab;
	int		m_grabSize;

	Point	m_dragOffsetTL;
	Point	m_dragOffsetBR;

	int GetGrabAspect(ImageView* view, const Point& p);

	//

	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);
};

#endif