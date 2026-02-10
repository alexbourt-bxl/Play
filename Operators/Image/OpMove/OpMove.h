#ifndef OP_MOVE_H
#define OP_MOVE_H

#include <Operators/Operator.h>
#include <Properties/IntProperty/IntProperty.h>
#include <Controls/Layout/LineLayout/LineLayout.h>

#include <Types/Graphics/Image8.h>

class OpMove
	: public Operator
{
	DECLARE_TYPE;

public:
	OpMove();
	OpMove(const OpMove& op);
	~OpMove();

	Object*	Copy();
	
	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	IntProperty X;
	IntProperty Y;

	void	ConnectView(OpView* view);
	void	DisconnectView(OpView* view);

private:
	Input	m_in;

	Output	m_out;
	Point	m_outOffset;

	LineLayout m_layoutVertical;

	bool	m_moving;
	Point	m_moveOffset;

	bool	store_allowKeyRepeat;

	// workinig values
	int		m_x;
	int		m_y;

	//

	void	OnLButtonDown(Mouse::Message& msg);
	void	OnLButtonUp(Mouse::Message& msg);
	void	OnMouseMove(Mouse::Message& msg);
	void	OnKeyDown(Key::Message& msg);
};

#endif