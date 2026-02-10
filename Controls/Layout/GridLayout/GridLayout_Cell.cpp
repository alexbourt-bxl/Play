#include "GridLayout.h"

#include <assert.h>

DEFINE_TYPE(GridLayout::Cell, Layout);

GridLayout::Cell::Cell()
{
	AutoSize = true;

	m_stretchColumn = true;
	m_stretchRow    = true;

	SizingEvent.Clear();
	SizeEvent.Clear();

	PaintEvent += EventHandler(this, &GridLayout::Cell::OnPaint);
	SizeEvent  += EventHandler(this, &GridLayout::Cell::OnSize);

	*Controls.AddEvent += EventHandler(this, &GridLayout::Cell::controls_OnAdd);
}

GridLayout::Cell::Cell(const Cell& cell)
{
	AutoSize = ((Cell&)cell).AutoSize;

	m_stretchColumn = cell.m_stretchColumn;
	m_stretchRow    = cell.m_stretchRow;

	SizingEvent.Clear();

	*Controls.AddEvent += EventHandler(this, &GridLayout::Cell::controls_OnAdd);
}

Size GridLayout::Cell::GetContentSize()
{
	_Rect extents;

	for (int i = 0; i < Controls.Count; i++)
	{
		_Rect rect = Controls[i]->Rect;

		/*	The extents need to be shrunk
			as much as possible. */

		if (m_align == Align::Left 
		 || m_align == Align::Center 
		 || m_align == Align::Right)
			rect.X = 0;

		if (m_align == Align::Top 
		 || m_align == Align::Middle 
		 || m_align == Align::Bottom 
		 || m_align == Align::Baseline)
			rect.Y = 0;

		extents.Expand(rect);
	}

	return extents.Size;
}

GridLayout::Column& GridLayout::Cell::GetColumn()
{
	assert(Parent && typeof(Parent) == TYPEOF(GridLayout));

	GridLayout* grid = (GridLayout*)Parent;
	return *grid->Columns[grid->Controls.GetIndexOf(this) % grid->Columns.Count];
}

GridLayout::Row& GridLayout::Cell::GetRow()
{
	assert(Parent && typeof(Parent) == TYPEOF(GridLayout));

	GridLayout* grid = (GridLayout*)Parent;
	return *grid->Rows[grid->Controls.GetIndexOf(this) / grid->Columns.Count];
}

bool GridLayout::Cell::GetStretchColumn()
{
	return m_stretchColumn;
}

void GridLayout::Cell::SetStretchColumn(bool stretchColumn)
{
	m_stretchColumn = stretchColumn;

	if (Parent && typeof(Parent) == TYPEOF(GridLayout))
	{
		GridLayout* grid = (GridLayout*)Parent;
		grid->MarkForLayout();
		grid->Update();
	}
}

bool GridLayout::Cell::GetStretchRow()
{
	return m_stretchRow;
}

void GridLayout::Cell::SetStretchRow(bool stretchRow)
{
	m_stretchRow = stretchRow;

	if (Parent && typeof(Parent) == TYPEOF(GridLayout))
	{
		GridLayout* grid = (GridLayout*)Parent;
		grid->MarkForLayout();
		grid->Update();
	}
}
