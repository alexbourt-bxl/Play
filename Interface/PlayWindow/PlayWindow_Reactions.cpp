#include "PlayWindow.h"

#include <Animation\Animation.h>
#include <Actions\Action.h>
#include <Types/Geometric\Shape.h>
#include <Types/Geometric\Poly.h>
#include <stdio.h>

void PlayWindow::controls_OnRemove(Container::Message& /*msg*/)
{

}

void PlayWindow::treeView_OnRequestView(View::Message& msg)
{
	if (msg.Type == TYPEOF(Image))
		msg.View = &m_imageView;
	//else if (msg.Type == TYPEOF(Shape))
	//	msg.View = &m_shapeView;
}

void PlayWindow::btnFiles_OnPress(Control::Message& )
{
	SuspendUpdates();

	m_fileView.Show();

	m_opList.Hide();
	m_treeView.Hide();

	m_imageView.Hide();
	//m_shapeView.Hide();
	m_propView.Hide();

	ResumeUpdates(true);

	m_fileView.HideSettings();
}

void PlayWindow::btnFiles_OnLButtonHold(Mouse::Message& )
{
	SuspendUpdates();

	m_fileView.Show();

	m_opList.Hide();
	m_treeView.Hide();

	m_imageView.Hide();
	//m_shapeView.Hide();
	m_propView.Hide();

	ResumeUpdates(true);

	m_fileView.ShowSettings();
}

void PlayWindow::btnView_OnPress(Control::Message& )
{
	SuspendUpdates();

	m_fileView.Hide();

	m_opList.Hide();
	m_treeView.Hide();

	m_imageView.Show();
	//m_shapeView.Show();
	m_propView.Show();

	ResumeUpdates(true);
}

void PlayWindow::btnTree_OnPress(Control::Message& )
{
	SuspendUpdates();

	m_fileView.Hide();

	m_opList.Show();
	m_treeView.Show();

	m_imageView.Hide();
	//m_shapeView.Hide();
	m_propView.Hide();

	ResumeUpdates(true);
}

void PlayWindow::btnRedo_OnPress(Control::Message& )
{
	Action::GetManager().Redo();
}

void PlayWindow::btnUndo_OnPress(Control::Message& )
{
	Action::GetManager().Undo();
}