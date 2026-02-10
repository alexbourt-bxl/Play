#include "GridLayout.h"

#include <System\System.h>
#include <Math\Math.h>

DEFINE_TYPE(GridLayout, Layout);

GridLayout::GridLayout()
{
	AutoSize = true;
	Align = Align::Center | Align::Middle;

	*m_cols.ResizeAddEvent	+= EventHandler(this, &GridLayout::col_OnResizeAdd);
	*m_cols.AddEvent		+= EventHandler(this, &GridLayout::col_OnAdd);
	*m_cols.RemoveEvent		+= EventHandler(this, &GridLayout::col_OnRemove);
	*m_rows.ResizeAddEvent	+= EventHandler(this, &GridLayout::row_OnResizeAdd);
	*m_rows.AddEvent		+= EventHandler(this, &GridLayout::row_OnAdd);
	*m_rows.RemoveEvent		+= EventHandler(this, &GridLayout::row_OnRemove);
	
	*Controls.ResizeAddEvent += EventHandler(this, &GridLayout::cell_OnResizeAdd);
	*Controls.RemoveEvent   += EventHandler(this, &GridLayout::cell_OnRemove);

	SizingEvent += EventHandler(this, &GridLayout::OnSizing);
	SizeEvent   += EventHandler(this, &GridLayout::OnSize);

	m_cols.Resize(1);
	m_rows.Resize(1);
	Controls.Resize(1);

	//BackColor = Color8(
	//	Math::Random(0, 0xFF),
	//	Math::Random(0, 0xFF),
	//	Math::Random(0, 0xFF),
	//	0x40);
}

GridLayout::~GridLayout()
{

}

Size GridLayout::GetContentSize(const _Size& /*refSize*/)
{
	_Size size(0, 0);

	for (int c = 0; c < m_cols.Count; c++)
	{
		Column& col = *m_cols[c];
		size.Width += col.Width;
	}

	for (int r = 0; r < m_rows.Count; r++)
	{
		Row& row = *m_rows[r];
		size.Height += row.Height;
	}

	return size;
}

GridLayout::Column& GridLayout::operator [] (int col)
{
	return *m_cols[col];
}

Array<GridLayout::Column*>& GridLayout::GetColumns()
{
	return m_cols;
}

Array<GridLayout::Row*>& GridLayout::GetRows()
{
	return m_rows;
}

void GridLayout::SetColumnCount(int nCols)
{
	if (nCols == m_cols.Count)
		return;

	/*	To change the column count, the cells have to be rearranged. */

	/*	If the column count is shrinking, then the cells are moved 
		to their new positions from front to back. */

	if (nCols < m_cols.Count)
	{
		for (int row = 0; row < m_rows.Count; row++)
		{
			for (int col = 0; col < m_cols.Count; col++)
			{
				Controls.SetIndexAt(
					row * m_cols.Count + col, 
					row * nCols + col);
			}
		}
	}

	/*	Resize the cell array. */

	Controls.Resize(m_rows.Count * nCols);

	/*	If the column count is growing, then the cells are moved 
		to their new positions from back to front. */

	if (nCols > m_cols.Count)
	{
		for (int row = m_rows.Count - 1; row >= 0; row--)
		{
			for (int col = m_cols.Count - 1; col >= 0; col--)
			{
				Controls.SetIndexAt(
					row * m_cols.Count + col, 
					row * nCols + col);
			}
		}
	}

	m_cols.Resize(nCols);

	UpdateLayout();
}

void GridLayout::SetRowCount(int nRows)
{
	if (nRows == m_rows.Count)
		return;

	Controls.Resize(nRows * m_cols.Count);
	m_rows.Resize(nRows);

	UpdateLayout();
}

void GridLayout::Clear()
{
	SetRowCount(1);
	SetColumnCount(1);
	Controls[0]->Controls.Clear();
}