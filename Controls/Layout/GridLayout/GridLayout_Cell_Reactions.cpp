#include "GridLayout.h"

void GridLayout::Cell::controls_OnAdd(Container::Message& msg)
{
	assert(Parent && typeof(Parent) == TYPEOF(GridLayout));
	GridLayout& grid = *(GridLayout*)Parent;

	int rowHeight = Row.AutoSize ? 0 : Row.Height;
	int columnWidth = Column.AutoSize ? 0 : Column.Width;

	if (m_stretchRow || AutoSize)
	{
		for (int c = 0; c < grid.Columns.Count; c++)
		{
			_Size contentSize = Row[c].GetContentSize();
			rowHeight = max(rowHeight, contentSize.Height);
		}
	}

	if (m_stretchColumn || AutoSize)
	{
		for (int r = 0; r < grid.Rows.Count; r++)
		{
			_Size contentSize = Column[r].GetContentSize();
			columnWidth = max(columnWidth, contentSize.Width);
		}
	}

	Row.Height = rowHeight;
	Column.Width = columnWidth;

	grid.UpdateLayout();
}

void GridLayout::Cell::OnSize(Control::Message& msg)
{
	for (int i = 0; i < Controls.Count; i++)
	{
		if (Column.Align == Align::Left)
			Controls[i]->X = 0;
		else if (Column.Align == Align::Center)
			Controls[i]->X = (Width - Controls[i]->Width) / 2;
		else if (Column.Align == Align::Right)
			Controls[i]->X = Width - Controls[i]->Width;

		if (Row.Align == Align::Top)
			Controls[i]->Y = 0;
		else if (Row.Align == Align::Middle)
			Controls[i]->Y = (Height - Controls[i]->Height) / 2;
		else if (Row.Align == Align::Bottom || m_align == Align::Baseline)
			Controls[i]->Y = Height - Controls[i]->Height;
	}
}