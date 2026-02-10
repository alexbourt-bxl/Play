#include "TreeView.h"

#include <Math\Math.h>
#include <Types/Geometric\Vector3.h>
#include <Types/Geometric\Matrix33.h>
#include <Types/Geometric\FRect.h>
#include <Graphics\Graphics.h>
#include <Operators\Operator.h>

DEFINE_TYPE(TreeView, View);

double TreeView::s_disconnectedOpacity = 0.53;
double TreeView::s_connectedOpacity = 1.0;
double TreeView::s_vGap = 7.0;
double TreeView::s_vPadding = 9.0;
double TreeView::s_hGap = 3.0;
double TreeView::s_hPadding = 2.0;
double TreeView::s_hPadding2 = 20.0;

TreeView::TreeView()
{
	RequestViewMessage.Sender = this;

	m_tree = null;

	PaintEvent		  += EventHandler(this, &TreeView::OnPaint);
	LButtonDownEvent  += EventHandler(this, &TreeView::OnLButtonDown);
	LButtonUpEvent    += EventHandler(this, &TreeView::OnLButtonUp);
	MouseMoveEvent    += EventHandler(this, &TreeView::OnMouseMove);
	LDoubleClickEvent += EventHandler(this, &TreeView::OnLDoubleClick);
	LButtonHoldEvent  += EventHandler(this, &TreeView::OnLButtonHold);
	KeyDownEvent      += EventHandler(this, &TreeView::OnKeyDown);

	m_hotInput		= null;
	m_hotOutput		= null;

	m_option		= None;

	m_placementGap	= FPoint(40, 20);

	AllowDoubleClick = true;
}

TreeView::~TreeView()
{

}

Tree* TreeView::GetTree() const
{
	return m_tree;
}

void TreeView::SetTree(_Tree* tree)
{
	if (m_tree)
	{
		*m_tree->AddEvent      -= EventHandler(this, &TreeView::tree_OnAddOperator);
		m_tree->PreUpdateEvent -= EventHandler(this, &TreeView::tree_PreUpdate);
		m_tree->UpdateEvent    -= EventHandler(this, &TreeView::tree_OnUpdate);

		m_tree->Views.Remove(this);
	}

	m_tree = tree;

	if (m_tree)
	{
		*m_tree->AddEvent      += EventHandler(this, &TreeView::tree_OnAddOperator);
		m_tree->PreUpdateEvent += EventHandler(this, &TreeView::tree_PreUpdate);
		m_tree->UpdateEvent    += EventHandler(this, &TreeView::tree_OnUpdate);

		m_tree->Views.Add(this);
	}

	Update();
}