#ifndef OP_BUTTON_H
#define OP_BUTTON_H

#include <Controls\Button\Button.h>
#include <Graphics\Graphics.h>
#include <Operators\Operator.h>

class OpButton
	: public Button
{
	DECLARE_TYPE;

	typedef Type _Type;

public:
	OpButton();

	_Rect GetFullPaintRect();

	__declspec(property(get = GetOpType, put = SetOpType))
	__Type* Type;
	__Type* GetOpType();
	void SetOpType(__Type* type);

private:
	__Type* m_type;

	void OnPaint(Graphics::Message& msg);
};

#endif