#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include <Controls\Views\View.h>
#include <Properties\FloatProperty\FloatProperty.h>
#include <Types/Graphics\GdiImage8.h>
#include <Controls\Views\NavControl\NavControl.h>
#include <Controls\Layout\GridLayout\GridLayout.h>
#include <Operators\Tree\Tree.h>
#include <Types/Graphics\Skin.h>
#include <Types/Graphics\GdiImage8.h>
#include <Types/Geometric\FPoint.h>
#include <Types/Geometric\FSize.h>

class Operator;
class Input;
class Output;
class Connection;
class FRect;
class FPoint;
class OpView;

class TreeView 
	: public View
{
	DECLARE_TYPE;

	friend class OpButton;
	
	friend class ActCreateOp;
	friend class ActDeleteOps;
	friend class ActMoveOps;

	typedef Tree _Tree;

public:
	FloatProperty PanX;
	FloatProperty PanY;
	FloatProperty Zoom;
	FloatProperty AspectRatio;

	enum Option
	{
		None = 0,
		Moving,
		Connecting
	};

public:
	TreeView();
	~TreeView();

	__declspec(property(get = GetGraph, put = SetTree))
	_Tree*	Tree;
	_Tree*	GetTree() const;
	void	SetTree(_Tree* tree);

	//__declspec(property(get = GetOpView, put = SetOpView))
	//View* OpView;
	//View* GetOpView() const;
	//void SetOpView(View* opView);

	FSize	GetOpSize(Operator* op);
	FRect	GetInputRect(Input* in);
	FRect	GetOutputRect(Output* out);

	_Rect	GetOpPaintRect(Operator* op);
	_Rect	GetOpFullPaintRect(Operator* op);

	Region	GetOpRegion(Operator* op);

	void	GetConnectionPoints(Output* out, Input* in, FPoint* p0, FPoint* p1, FPoint* p2, FPoint* p3);
	Region	GetConnectionRegion(Connection* conn);

	_Rect	GetHotInputRect(const FPoint& p, Input* in);
	_Rect	GetHotOutputRect(Output* out, const FPoint& p);

	FPoint	GetHotPoint(const Point& p);

	void	PaintOp(Graphics::Message& msg, Operator* op);
	void	PaintOpConnections(Graphics::Message& msg, Operator* op);

	void	PaintConnection(Graphics& g, FPoint p0, FPoint p1, FPoint p2, FPoint p3, bool hotOutput);

	Object*	HitTestOp(const Point& p);

	void	SelectAll();
	void	DeselectAll();
	void	DeleteSelected();

	FRect	GetOpBounds(Array<Operator*>& ops);

	//

	Event<TreeView::Message&> RequestViewMessage;

	void	OnPaint(Graphics::Message& msg);
	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);
	void	OnLDoubleClick(Mouse::Message& msg);
	void	OnLButtonHold(Mouse::Message& msg);
	void	OnKeyDown(Key::Message& msg);

private:
	_Tree*  m_tree;

	void	tree_OnAddOperator(Container::Message& msg);

	Region  m_rgnUpdate;
	void    tree_PreUpdate(Tree::Message& msg);
	void    tree_OnUpdate(Tree::Message& msg);

	Option  m_option;
	Point   m_moveStart;
	bool    m_moved;

	Input*  m_hotInput;
	Output* m_hotOutput;

	FPoint	m_hotPoint;
	FPoint	m_prevPoint;

	Array<FPoint> m_offsetStart;

	FPoint	m_placementGap;

	//

	static Skin	s_skinOp;
	static Skin s_skinOpActive;
	static Skin	s_skinOpShown;
	static void	skinOp_Init(Skin* skin);

	static GdiImage8 s_imgConnectionRing;
	static GdiImage8 s_imgConnectionPlus;
	static GdiImage8 s_imgConnectedCircle;
	static GdiImage8 s_imgConnectedArrow;

	static double s_disconnectedOpacity;
	static double s_connectedOpacity;
	static double s_vGap;
	static double s_vPadding;
	static double s_hGap;
	static double s_hPadding;
	static double s_hPadding2;
};

#endif