#include "GridLayout.h"

#include <Graphics\Graphics.h>

void GridLayout::OnSizing(Control::_SizingMessage& msg)
{
	if (AutoSize)
		msg.Rect.Size = GetContentSize(msg.Rect.Size);
}

void GridLayout::OnSize(Control::Message& /*msg*/)
{
	SuspendUpdates();

	_Size contentSize = GetContentSize(Size);

	int xStart = 0;
	int y = 0;

	if		(m_align == Align::Left)   xStart = 0;
	else if (m_align == Align::Center) xStart = (Width - contentSize.Width) / 2;
	else if (m_align == Align::Right)  xStart = Width - contentSize.Width;

	if		(m_align == Align::Top)    y = 0;
	else if (m_align == Align::Middle) y = (Height - contentSize.Height) / 2;
	else if (m_align == Align::Bottom) y = Height - contentSize.Height;

	for (int r = 0; r < m_rows.Count; r++)
	{
		int x = xStart;

		for (int c = 0; c < m_cols.Count; c++)
		{
			Cell& cell = *(Cell*)Controls[r * m_cols.Count + c];
			
			cell.SetRect(
				x, 
				y,
				cell.Column.Width,
				cell.Row.Height);

			x += m_cols[c]->Width;
		}

		y += m_rows[r]->Height;
	}

	ResumeUpdates(true);
}

void GridLayout::col_OnResizeAdd(Container::Message& msg)
{
	Column** col = (Column**)msg.Item;
	*col = new Column();
}

void GridLayout::col_OnAdd(Container::Message& msg)
{
	Column* col = *((Column**)msg.Item);
	col->m_gridLayout = this;
}

void GridLayout::col_OnRemove(Container::Message& msg)
{
	Column* col = *((Column**)msg.Item);
	col->m_gridLayout = null;
}

void GridLayout::row_OnResizeAdd(Container::Message& msg)
{
	Row** row = (Row**)msg.Item;
	*row = new Row();
}

void GridLayout::row_OnAdd(Container::Message& msg)
{
	Row* row = *((Row**)msg.Item);
	row->m_gridLayout = this;
}

void GridLayout::row_OnRemove(Container::Message& msg)
{
	Row* row = *((Row**)msg.Item);
	row->m_gridLayout = null;
}

void GridLayout::cell_OnResizeAdd(Container::Message& msg)
{
	Control** control = (Control**)msg.Item;
	*control = new Cell();

	MarkForLayout();
}

void GridLayout::cell_OnRemove(Container::Message& msg)
{
	Cell* cell = *((Cell**)msg.Item);
	delete cell;
}