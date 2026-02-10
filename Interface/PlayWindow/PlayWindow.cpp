#include "PlayWindow.h"

DEFINE_TYPE(PlayWindow, Window);

/*	TODO: need a system of viewers for different situations. */

PlayWindow::PlayWindow()
{
	//m_treeView.OpView = &m_imageView;

	SuspendUpdates();
	
	m_btnFiles.Icon = &s_imgFiles;
	m_btnFiles.PressEvent += EventHandler(this, &PlayWindow::btnFiles_OnPress);
	m_btnFiles.LButtonHoldEvent += EventHandler(this, &PlayWindow::btnFiles_OnLButtonHold);

	m_btnView.Icon = &s_imgView;
	m_btnView.PressEvent += EventHandler(this, &PlayWindow::btnView_OnPress);

	m_btnTree.Icon = &s_imgTree;
	m_btnTree.PressEvent += EventHandler(this, &PlayWindow::btnTree_OnPress);

	m_btnRedo.Icon = &s_imgRedo;
	m_btnRedo.PressEvent += EventHandler(this, &PlayWindow::btnRedo_OnPress);

	m_btnUndo.Icon = &s_imgUndo;
	m_btnUndo.PressEvent += EventHandler(this, &PlayWindow::btnUndo_OnPress);

	m_spacer1.Height = 10000;
	m_spacer1.CanShrink = true;

	m_spacer2.Height = 10000;
	m_spacer2.CanShrink = true;

	m_sidebar.Align = Align::Center | Align::Top;
	m_sidebar.Padding = 5;
	m_sidebar.Dock = DockLeft;
	m_sidebar.Controls.Add(&m_btnFiles);
	m_sidebar.Controls.Add(&m_spacer1);
	m_sidebar.Controls.Add(&m_btnView);
	m_sidebar.Controls.Add(&m_btnTree);
	m_sidebar.Controls.Add(&m_spacer2);
	m_sidebar.Controls.Add(&m_btnRedo);
	m_sidebar.Controls.Add(&m_btnUndo);

	m_fileView.Dock = DockFill;
	m_fileView.Hide();

	m_imageView.Dock = DockFill;
	m_imageView.Hide();

	//m_shapeView.Dock = DockFill;
	//m_shapeView.Hide();

	m_opList.AutoSize = true;
	m_opList.Height = 50;
	m_opList.Dock = DockTop;

	m_treeView.Dock = DockFill;
	m_treeView.Width = 300;
	m_treeView.RequestViewMessage += EventHandler(this, &PlayWindow::treeView_OnRequestView);

	//m_treeView2.Dock = DockLeft;
	//m_treeView2.Width = 300;
	//m_treeView2.RequestViewMessage += EventHandler(this, &PlayWindow::treeView_OnRequestView);

	//m_treeView3.Dock = DockLeft;
	//m_treeView3.Width = 300;
	//m_treeView3.RequestViewMessage += EventHandler(this, &PlayWindow::treeView_OnRequestView);

	//m_treeView4.Dock = DockLeft;
	//m_treeView4.Width = 300;
	//m_treeView4.RequestViewMessage += EventHandler(this, &PlayWindow::treeView_OnRequestView);

	m_propView.BackColor = Color::Panel;
	m_propView.Dock = DockBottom;
	m_propView.AutoSize = true;
	m_propView.Hide();

	Controls.Add(&m_sidebar);
	Controls.Add(&m_fileView);
	Controls.Add(&m_imageView);
	//Controls.Add(&m_shapeView);
	Controls.Add(&m_opList);
	Controls.Add(&m_treeView);
	//Controls.Add(&m_treeView2);
	//Controls.Add(&m_treeView3);
	//Controls.Add(&m_treeView4);
	Controls.Add(&m_propView);

	ResumeUpdates();
}

PlayWindow::~PlayWindow()
{

}

View* PlayWindow::RequestView(Operator* op)
{
	/*	Update the operator to have a valid result. */

	op->Update();

	Object* obj = op->Out[0]->Data[Tag::Image];

	if (!obj)
		return null;

	if (typeof(obj) == TYPEOF(Image))
		return &m_imageView;
	//else if (typeof(obj) == TYPEOF(Shape))
	//	return &m_shapeView;

	return null;
}

void PlayWindow::ShowOp(Operator* op)
{
	View* view = RequestView(op);

	if (!view || typeof(view) != TYPEOF(OpView))
		return;

	OpView* opView = (OpView*)view;

	opView->ViewedOperator = op;
	opView->ActiveOperator = op;

	m_propView.SetPropertyContainer(op, typeof(op).DisplayName);

	ShowOpView();
}

void PlayWindow::ShowOpView()
{
	SuspendUpdates();

	m_fileView.Hide();

	m_opList.Hide();
	m_treeView.Hide();

	m_imageView.Show();
	m_propView.Show();

	ResumeUpdates(true);
}