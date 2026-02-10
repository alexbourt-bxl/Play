#include "Play.h"

#define TEST

#include <Native/Native.h>
#include <Clipboard/Clipboard.h>

#include <Controls/Window/Window.h>
#include <Controls/Button/Button.h>
#include <Controls/Views/ImageView/ImageView.h>
#include <Controls/Views/TreeView/TreeView.h>
#include <Controls/OpList/OpList.h>
#include <Controls/OpList/OpButton.h>

#include <Operators/OpFile/OpFile.h>
//#include <Operators/Image/OpComposite/OpComposite.h>
#include <Operators/Image/OpResize/OpResize.h>
#include <Operators/Image/OpCrop/OpCrop.h>
//#include <Operators/Image/OpMove/OpMove.h>
#include <Operators/Image/OpOpacity/OpOpacity.h>
#include <Operators/Image/OpNoise/OpNoise.h>
#include <Operators/Image/OpColor/OpColor.h>
//#include <Operators/Image/OpGradient/OpGradient.h>
//#include <Operators/Image/OpSkin/OpSkin.h>

#include <Operators/Actions/ActCreateOp.h>

#include <stdio.h>

int Play()
{
	/*	This is the main function. It is not the entry point, 
		but it is the main one of interest. */

	PlayWindow& wnd = GetMainWindow();
	
	Tree& tree = GetMainTree();
	wnd.m_treeView.Tree = &tree;
	//wnd.m_treeView2.Tree = &tree;
	//wnd.m_treeView3.Tree = &tree;
	//wnd.m_treeView4.Tree = &tree;

	//*tree.AddEvent += EventHandler(&OnAddOperator);

	/*	TODO: pressing and holding a sidebar button shows its settings. For example,
		for the file view, it can show settings for the thumbnail size and other stuff.
		For the image view, it can allow the layout of screens (a la Combustion), etc. */

	//wnd.m_fileView.Directory.Path = L"C:\\Users\\Alex\\Documents\\XD";

	//

	for (int i = 0; i < wnd.m_opList.Controls.Count; i++)
	{
		Control* control = wnd.m_opList.Controls[i];

		if (typeof(control) != TYPEOF(OpButton))
			continue;

		((OpButton*)control)->PressEvent += EventHandler(opButton_OnPress);
	}

	wnd.Show();

	// Update the clipboard

	Clipboard::Update();

	// Let the user play :)

	int result = ProcessMessages();

	// Clean up and exit

	return result;
}

PlayWindow& GetMainWindow()
{
	static PlayWindow wnd;
	return wnd;
}

void opButton_OnPress(Control::Message& msg)
{
	OpButton* btnOp = (OpButton*)msg.Control;

	ActCreateOp* actCreateOp = new ActCreateOp(&GetMainTree(), btnOp->Type);

	/*	If a single op is selected, 
		attempt an automatic connection. */

	if (GetMainTree().Selected.Count == 1)
		actCreateOp->ConnectAfter = GetMainTree().Selected[0];

	Action::Perform(actCreateOp);
}

Tree& GetMainTree()
{
	static Tree tree;
	return tree;
}
//
//void OnAddOperator(Container::Message& msg)
//{
//	Operator* op = *((Operator**)msg.Item);
//
//	PlayWindow& wnd = GetMainWindow();
//	wnd.ShowOp(op);
//}