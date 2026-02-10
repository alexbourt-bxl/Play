#ifndef OP_SHAPE_CONTROL_TOOL_H
#define OP_SHAPE_CONTROL_TOOL_H

#include <Tools\Tool.h>
#include <Controls\Button\Button.h>
#include <Controls\ListButton\ListButton.h>
#include <Types/Graphics\GdiImage8.h>
#include <Operators\Image\OpShape\OpShape.h>

class OpShape::ControlTool
	: public Tool
{
	friend class OpShape;

	DECLARE_TYPE;

public:
	ControlTool();

	void Select();
	void Deselect();

	void ConnectViewProtected(View* view);
	void DisconnectViewProtected(View* view);

	//

	void OnPaint(Graphics::Message& msg);
	void OnLButtonDown(Mouse::Message& msg);
	void OnLButtonUp(Mouse::Message& msg);
	void OnMouseMove(Mouse::Message& msg);
	void OnKeyDown(Key::Message& msg);
	//void OnKeyUp(Key::Message& msg);

private:
	OpShape* m_opShape;

	Button m_button;
	static GdiImage8 s_imgControl;

	void button_OnPress(Control::Message& msg);

	ListButton	m_btnVertex;
	static GdiImage8 s_imgVertexFixed;
	static GdiImage8 s_imgVertexSliding;
	static GdiImage8 s_imgVertexBroken;
	static GdiImage8 s_imgVertexMixed;

	void btnVertex_OnSelectItem(::Message& msg);

	bool m_selecting;
	Rect m_selectRect;
	Point m_dragStart;

	void UpdateSelectionControls();

	bool store_allowKeyRepeat;
};

#endif