#include "OpShape.h"

#include <Controls\Valuebox\Valuebox.h>
#include <Controls\Views\OpView\OpView.h>

#include "Tools\CreateTool.h"
#include "Tools\ControlTool.h"
#include "Tools\TransformTool.h"
#include "Tools\ShapeTool.h"

DEFINE_TYPE(OpShape, Operator);
SET_TYPE_DISPLAY_NAME(OpShape, L"Shape");

OpShape::OpShape() 
{
	Init();
}

OpShape::OpShape(const OpShape& op)
{
	_Shape = op._Shape;

	Init();
}

OpShape::~OpShape()
{
	if (m_outImage)
		delete m_outImage;

	if (m_createTool)
		delete m_createTool;

	if (m_controlTool)
		delete m_controlTool;

	if (m_transformTool)
		delete m_transformTool;

	if (m_shapeTool)
		delete m_shapeTool;
}

Object* OpShape::Copy()
{
	return new OpShape(*this);
}

void OpShape::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outImage = null;

	m_createTool = new CreateTool();
	m_controlTool = new ControlTool();
	m_transformTool = new TransformTool();
	m_shapeTool = new ShapeTool();

	m_createTool->m_opShape = this;
	m_controlTool->m_opShape = this;
	m_transformTool->m_opShape = this;
	m_shapeTool->m_opShape = this;

	m_tools.Add(m_createTool);
	m_tools.Add(m_controlTool);
	m_tools.Add(m_transformTool);
	m_tools.Add(m_shapeTool);
	m_tools.PreChangeActiveToolEvent += EventHandler(this, &OpShape::tools_PreChangeActiveTool);
	m_tools.ChangeActiveToolEvent += EventHandler(this, &OpShape::tools_OnChangeActiveTool);

	m_tools.SetActiveTool(m_createTool);

	m_shapeColor   = Color::White;

	m_curveColor   = Color::PlayYellow;
	m_vertexColor  = Color::Back;
	m_selectedColor = Color::PlayYellow;
	m_tangentColor = Color(0.8, 0.6, 0.2, 0.75);

	m_vertexSize   = 7;
	m_hitSize	   = 12;

	m_down = false;
	m_moved = false;
	m_moving = null;
	m_collapseHandles = false;
}

void OpShape::LayoutControls(LineLayout* layout)
{
	layout->Controls.Add(&m_tools.Layout);
}

void OpShape::CopyWorkingValues()
{
	m_shape = _Shape;
}

void OpShape::Process()
{
	Image* in = m_in.Data.Get<Image>(Tag::Image);

	if (!in)
	{
		if (m_outImage)
		{
			delete m_outImage;
			m_outImage = null;
		}

		m_out.Data.Clear();
		return; 
	}

	/*	Create the output image. */

	Size outSize = in->Size;

	Image* outImage = m_out.Data.Get<Image>(Tag::Image);

	m_outImage = 
		outImage && typeof(outImage) == TYPEOF(Image8) && outImage->Size == outSize
		? (Image8*)outImage
		: new Image8(outSize);

	/*	Draw the shapes. */

	Graphics g(m_outImage);
	
	g.Antialias = Graphics::Highest;

	g.ClearRect(
		0, 0,
		outSize.Width,
		outSize.Height);

	/*	If the curve is not closed, then it should be drawn as 
		incomplete, so there is a straight line between the last 
		and first points. */

	for (int i = 0; i < m_shape.Curves.Count; i++)
	{
		Curve& curve = m_shape.Curves[i];

		if (!curve.Closed)
		{
			curve.Points[0].rIn = FPoint(0, 0);
			curve.Points[curve.Points.Count - 1].rOut = FPoint(0, 0);
		}
	}

	//

	g.FillShape(
		m_shape,
		m_shapeColor);

	//

	m_out.Data.Set(Tag::Image, m_outImage);
}

void OpShape::ConnectView(OpView* view)
{
	view->PaintEvent += EventHandler(this, &OpShape::OnPaint);

	if (m_tools.ActiveTool)
		m_tools.ActiveTool->ConnectView(view);

	// TODO: update view?
}

void OpShape::DisconnectView(OpView* view)
{
	view->PaintEvent -= EventHandler(this, &OpShape::OnPaint);

	if (m_tools.ActiveTool)
		m_tools.ActiveTool->DisconnectViews();
}