#ifndef OP_SHAPE_SHAPE_TOOL_H
#define OP_SHAPE_SHAPE_TOOL_H

#include <Tools\Tool.h>
#include <Controls\ListButton\ListButton.h>
#include <Controls\Valuebox\Valuebox.h>
#include <Types/Graphics\GdiImage8.h>
#include <Operators\Image\OpShape\OpShape.h>

class OpShape::ShapeTool
	: public Tool
{
	friend class OpShape;

	DECLARE_TYPE;

public:
	ShapeTool();

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

	ListButton m_button;
	static GdiImage8 s_imgEllipse;
	static GdiImage8 s_imgRect;
	static GdiImage8 s_imgPolygon;

	Valuebox m_valEdges;

	void button_OnSelectItem(::Message& msg);

	int m_tool;

	static const int EllipseTool = 1;
	static const int RectTool    = 2;
	static const int PolygonTool = 3;

	void UpdateCreatedCurve(ImageView* view, FPoint& p);

	Curve m_unitCurve; // the curve which is transformed by the shape tool

	Point m_dragStart;

	bool  m_proportional;
	bool  m_centerPivot;
};

#endif