#include "GridLayout.h"

#include <assert.h>

DEFINE_TYPE(GridLayout::Row, Layout);

GridLayout::Row::Row()
{
	AutoSize = true;
	m_gridLayout = null;
}

GridLayout::Row::Row(const Row& row)
{
	AutoSize = ((Row&)row).AutoSize;
	m_gridLayout = null;
}

GridLayout::Cell& GridLayout::Row::operator [] (int col)
{
	assert(m_gridLayout);
	int cellIndex = m_gridLayout->Rows.GetIndexOf(this) * m_gridLayout->Columns.Count + col;
	return *((Cell*)m_gridLayout->Controls[cellIndex]);
}