#ifndef OP_COMPOSITE_H
#define OP_COMPOSITE_H

#include <Operators\Operator.h>
#include <Types\Geometric\Region.h>

#include <Types\Graphics\Image8.h>
#include <Types\Graphics\GdiImage.h>
#include <Types\Graphics\GdiImage8.h>

class OpComposite
	: public Operator
{
	DECLARE_TYPE;

public:
	OpComposite();
	OpComposite(const OpComposite& op);
	~OpComposite();

	String	GetOpName();

	void	Init();
	Object*	Copy();

	bool	UsesSpec(Type specType, Type inType);
	bool	ForwardsSpec(Type specType);

	void	UpdateResult(SpecList& specs);

	bool	TargetBottomLayer;

private:
	Input	m_over;
	Input	m_under;
	Output	m_out;

	void Composite_Image8(
		int* outBuffer,
		int outBufferWidth, int outBufferHeight,
		int outX, int outY,
		//
		int* inBuffer,
		int inBufferWidth, int inBufferHeight,
		int inX, int inY,
		int inWidth, int inHeight,
		//
		int opacity,
		Region& clip);

	void Composite_GdiImage(
		HDC hdcIn,
		int outX, int outY,
		int outWidth, int outHeight,
		//
		HDC hdcOut,
		int inX, int inY,
		int inWidth, int inHeight,
		//
		int opacity,
		Region& clip);
};

#endif