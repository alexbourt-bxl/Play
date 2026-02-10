#ifndef OP_SHAPE_TRANSFORM_TOOL_H
#define OP_SHAPE_TRANSFORM_TOOL_H

#include <Tools\Tool.h>
#include <Controls\Button\Button.h>
#include <Types/Graphics\GdiImage8.h>
#include <Operators\Image\OpShape\OpShape.h>

class OpShape::TransformTool
	: public Tool
{
	friend class OpShape;

	DECLARE_TYPE;

public:
	TransformTool();

	void Select();
	void Deselect();

	void ConnectViewProtected(View* view);
	void DisconnectViewProtected(View* view);

	//

	//void OnPaint(Graphics::Message& msg);
	void OnLButtonDown(Mouse::Message& msg);
	void OnLButtonUp(Mouse::Message& msg);
	void OnMouseMove(Mouse::Message& msg);
	void OnKeyDown(Key::Message& msg);
	void OnKeyUp(Key::Message& msg);

private:
	OpShape* m_opShape;

	Button m_button;
	static GdiImage8 s_imgTransform;

	void button_OnPress(Control::Message& msg);
};

#endif