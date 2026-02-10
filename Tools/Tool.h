#ifndef TOOL_H
#define TOOL_H

/*	Tools provide a nice way to organize interaction in operators
	in a clean way. See OpShape for a good example. 
	
	When a tool is selected it's assigned to a view. Only one tool
	can be active in a view at any time. */

#include <Types/Object.h>
#include <Controls/Layout/LineLayout/LineLayout.h>
#include <Containers/Array.h>

class ToolArray;
class View;

class Tool
	: public Object
{
	friend class ToolArray;

	DECLARE_TYPE;

public:
	Tool();
	Tool(const Tool& tool);
	~Tool();

	Object* Copy();

	void Init();

	__declspec(property(get = GetControlLayout))
	LineLayout& ControlLayout;
	LineLayout& GetControlLayout();

	__declspec(property(get = GetParamLayout))
	LineLayout& ParamLayout;
	LineLayout& GetParamLayout();

	__declspec(property(get = GetConnectedViews))
	Array<View*>& ConnectedViews;
	Array<View*>& GetConnectedViews();

	/*	Having the derived tools create their own Select/Deselect
		functions allows them to do whatever they need individually
		to indicate their status. */

	virtual void Select();
	virtual void Deselect();

	//

	__declspec(property(get = GetContainer))
	ToolArray&	Container;
	ToolArray&	GetContainer();

	//

	void ConnectView(View* view);
	void DisconnectViews();

protected:
	virtual void ConnectViewProtected(View* view);
	virtual void DisconnectViewProtected(View* view);

	LineLayout	m_controlLayout;
	LineLayout	m_paramLayout;

private:
	ToolArray*	m_container;
	Array<View*> m_connectedViews;
};

#endif