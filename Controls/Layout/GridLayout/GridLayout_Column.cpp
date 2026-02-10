#include "GridLayout.h"

#include <assert.h>

DEFINE_TYPE(GridLayout::Column, Layout);

GridLayout::Column::Column()
{
	AutoSize = true;
	m_gridLayout = null;
}

GridLayout::Column::Column(const Column& column)
{
	AutoSize = ((Column&)column).AutoSize;
	m_gridLayout = null;
}

GridLayout::Cell& GridLayout::Column::operator [] (int row)
{
	assert(m_gridLayout);
	int cellIndex = row * m_gridLayout->Columns.Count + m_gridLayout->Columns.GetIndexOf(this);
	return *((Cell*)m_gridLayout->Controls[cellIndex]);
}