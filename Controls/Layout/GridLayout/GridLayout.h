#ifndef GRID_LAYOUT_H
#define GRID_LAYOUT_H

#include <Controls/Layout/Layout.h>
#include <Containers/Array.h>
#include <Types/Geometric/Size.h>

class GridLayout
	: public Layout
{
	DECLARE_TYPE;

public:
	class Column;
	class Row;
	class Cell;

public:
	GridLayout();
	~GridLayout();

	_Size GetContentSize(const _Size& refSize);

	Column& operator [] (int col);

	__declspec(property(get = GetColumns))
	Array<Column*>& Columns;
	Array<Column*>& GetColumns();

	__declspec(property(get = GetRows))
	Array<Row*>& Rows;
	Array<Row*>& GetRows();

	void SetColumnCount(int nRows);
	void SetRowCount(int nRows);

	void Clear();

	//

	void OnSizing(Control::_SizingMessage& msg);
	void OnSize(Control::Message& msg);

	void col_OnResizeAdd(Container::Message& msg);
	void col_OnAdd(Container::Message& msg);
	void col_OnRemove(Container::Message& msg);
	void row_OnResizeAdd(Container::Message& msg);
	void row_OnAdd(Container::Message& msg);
	void row_OnRemove(Container::Message& msg);
	
	void cell_OnResizeAdd(Container::Message& msg);
	void cell_OnRemove(Container::Message& msg);

private:
	Array<Column*> m_cols;
	Array<Row*> m_rows;
};

//

class GridLayout::Cell
	: public Layout
{
	DECLARE_TYPE;

	friend class GridLayout;

public:
	Cell();
	Cell(const Cell& cell);

	_Size GetContentSize();

	__declspec(property(get = GetColumn))
	GridLayout::Column& Column;
	GridLayout::Column& GetColumn();
	
	__declspec(property(get = GetRow))
	GridLayout::Row& Row;
	GridLayout::Row& GetRow();

	__declspec(property(get = GetStretchColumn, put = SetStretchColumn))
	bool StretchColumn;
	bool GetStretchColumn();
	void SetStretchColumn(bool stretchColumn);

	__declspec(property(get = GetStretchRow, put = SetStretchRow))
	bool StretchRow;
	bool GetStretchRow();
	void SetStretchRow(bool stretchRow);

	void OnSize(Control::Message& msg);

	void controls_OnAdd(Container::Message& msg);

private:
	bool m_stretchColumn;
	bool m_stretchRow;

	/*	TODO: padding */
};

//

class GridLayout::Column
	: public Layout
{
	DECLARE_TYPE;

	friend class GridLayout;

public:
	Column();
	Column(const Column& col);
	
	Cell& operator [] (int row);

private:
	GridLayout* m_gridLayout;

	/*	TODO: margins */
};

//

class GridLayout::Row
	: public Layout
{
	DECLARE_TYPE;

	friend class GridLayout;

public:
	Row();
	Row(const Row& row);
	
	Cell& operator [] (int col);

private:
	GridLayout* m_gridLayout;

	/*	TODO: margins */
};

#endif