#ifndef PLAY_WINDOW_H
#define PLAY_WINDOW_H

#include <Controls\Window\Window.h>

#include <Controls\Layout\Layout.h>
#include <Controls\Views\TreeView\TreeView.h>
#include <Controls\Views\ImageView\ImageView.h>
#include <Controls\Views\ShapeView\ShapeView.h>
#include <Controls\Views\PropertyView\PropertyView.h>
#include <Controls\Views\FileView\FileView.h>
#include <Controls\OpList\OpList.h>
#include <Interface\Sidebar\Sidebar.h>
#include <Controls\HoverButton\HoverButton.h>
#include <Controls\Layout\Spacer\Spacer.h>

#include <Controls\Views\CurveView\CurveView.h>

class PlayWindow
	: public Window
{
	DECLARE_TYPE;

public:
	PlayWindow();
	~PlayWindow();

	void controls_OnRemove(Container::Message& msg);
	void treeView_OnRequestView(View::Message& msg);

	Sidebar m_sidebar;

	HoverButton	m_btnFiles;
	static GdiImage8 s_imgFiles;
	void btnFiles_OnPress(Control::Message& msg);
	void btnFiles_OnLButtonHold(Mouse::Message& msg);

	HoverButton	m_btnView;
	static GdiImage8 s_imgView;
	void btnView_OnPress(Control::Message& msg);

	HoverButton	m_btnTree;
	static GdiImage8 s_imgTree;
	void btnTree_OnPress(Control::Message& msg);

	HoverButton	m_btnRedo;
	static GdiImage8 s_imgRedo;
	void btnRedo_OnPress(Control::Message& msg);

	HoverButton	m_btnUndo;
	static GdiImage8 s_imgUndo;
	void btnUndo_OnPress(Control::Message& msg);

	View*		RequestView(Operator* op);

	void		ShowOp(Operator* op);
	void		ShowOpView();

	Spacer		m_spacer1;
	Spacer		m_spacer2;

	FileView	m_fileView;
	
	TreeView	m_treeView;
	TreeView	m_treeView2;
	TreeView	m_treeView3;
	TreeView	m_treeView4;
	OpList		m_opList;
	
	ImageView	m_imageView;
	//ShapeView	m_shapeView;
	PropertyView m_propView;
};

#endif