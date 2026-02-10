#include "OpList.h"

#include <Operators\Operator.h>
#include <Controls\OpList\OpButton.h>
#include <Controls\Views\TreeView\TreeView.h>

DEFINE_TYPE(OpList, LineLayout);

OpList::OpList()
{
	*Controls.RemoveEvent += EventHandler(this, &OpList::OnRemoveControl);
	
	SuspendUpdates();

	Wrap = true;
	Padding = 10;
	Gap = 5;
	BackColor = Color(Color::White, 0.1);

	ResumeUpdates();
	UpdateList();
}

OpList::~OpList()
{

}

void OpList::UpdateList()
{
	Controls.Clear();

	//Array<OpType>& opTypes = Operator::GetRegistered();
	Array<__Type*> ops = __Type::GetByType(TYPEOF(Operator));
	
	// TODO: array sort (comparison function)

	for (int i = 0; i < ops.Count; i++)
	{
		if (ops[i]->IsAbstract)
			continue;

		OpButton* btnOp = new OpButton();
		btnOp->Type = ops[i];
		Controls.Add(btnOp);
	}

	UpdateLayout();
}