#ifndef OP_SHAPE_H
#define OP_SHAPE_H

#include <Operators\Operator.h>
#include <Properties\IntProperty\IntProperty.h>
#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Controls\Button\Button.h>
#include <Controls\ListButton\ListButton.h>
#include <Controls\Valuebox\Valuebox.h>
#include <Types/Geometric\Shape.h>
#include <Types/Geometric\Rect.h>
#include <Types/Graphics\Image8.h>

#include <Tools\ToolArray.h>
//#include "Tools\CreateTool.h"
//#include "Tools\ControlTool.h"
//#include "Tools\TransformTool.h"
//#include "Tools\ShapeTool.h"

class ImageView;

class OpShape
	: public Operator
{
	DECLARE_TYPE;

public:
	OpShape();
	OpShape(const OpShape& op);
	~OpShape();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	Shape	_Shape;

	void	ConnectView(OpView* view);
	void	DisconnectView(OpView* view);

	//

	class	CreateTool;
	class	ControlTool;
	class	TransformTool;
	class	ShapeTool;

private:
	Input	m_in;

	Output	m_out;
	Image*	m_outImage;

	/* Working values. */
	Shape   m_shape;

	//

	Color	m_shapeColor;

	Color	m_curveColor;
	Color	m_vertexColor;
	Color	m_selectedColor;
	Color	m_tangentColor;

	double	m_vertexSize;
	double	m_hitSize;

	struct HitResult
	{
		int CurveId;
		int PointId;
		int Handle; // -1 = In, 0 = neither, 1 = Out

		HitResult()
			: CurveId(-1), PointId(-1), Handle(0) {}

		HitResult(int curveId, int pointId, int handle = 0)
			: CurveId(curveId), PointId(pointId), Handle(handle) {}

		HitResult& operator = (int possibleNull) { assert(!possibleNull); CurveId = -1; PointId = -1; Handle = 0; return *this; }

		/*	If a hit result is compared with another whose handle is < -1, 
			the comparison will return true if the curve and point IDs match. 
			This way handles can be matched with their points. */

		bool operator == (const HitResult& hit)	{ return CurveId == hit.CurveId && PointId == hit.PointId && (hit.Handle < -1 || Handle == hit.Handle); }
		bool operator != (const HitResult& hit) { return !(*this == hit); }

		/*	To determine if the hit result is valid, only the curve ID has
			to be checked, because a selected point or handle implies a curve. */

		operator bool() { return CurveId > -1; }
	};

	//

	ToolArray	   m_tools;

	CreateTool*	   m_createTool;
	ControlTool*   m_controlTool;
	TransformTool* m_transformTool;
	ShapeTool*	   m_shapeTool;

	//

	Array<HitResult> m_selected;
	Array<FPoint>	 m_offsets;

	bool	m_down;
	bool	m_moved;

	HitResult m_moving;

	bool	m_collapseHandles; // one-time flag
	double	m_handleLength;

	//

	void OnPaint(Graphics::Message& msg);
	//void OnLButtonDown(Mouse::Message& msg);
	//void OnLButtonUp(Mouse::Message& msg);
	//void OnMouseMove(Mouse::Message& msg);
	//void OnKeyDown(Key::Message& msg);
	//void OnKeyUp(Key::Message& msg);

	void tools_PreChangeActiveTool(::Message& msg);
	void tools_OnChangeActiveTool(::Message& msg);

	void MoveSelection(ImageView* view, HitResult& moving, FPoint& p, FPoint& pp);
	HitResult HitTest(ImageView* view, const Point& p);
	Rect GetViewRect(ImageView* view, const Rect& rect, int padding);
};

#endif