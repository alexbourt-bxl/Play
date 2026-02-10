#ifndef SPACE_TILE_H
#define SPACE_TILE_H

#include "Space.h"

class Space::Tile
{
	friend class Space;

	typedef Image _Image;
	typedef Rect _Rect;

public:
	Tile*	Parent;
	int		Level;

	Tile**	Tiles;

public:
	Tile(int level = -1);
	Tile(Image* image);
	~Tile();

	__declspec(property(get = GetParent))
	Tile*	Parent;
	Tile*	GetParent() const;

	__declspec(property(get = GetLevel))
	int		Level;
	int		GetLevel();

	__declspec(property(get = GetImage))
	_Image*	Image;
	_Image*	GetImage() const;

	__declspec(property(get = GetRect))
	_Rect	Rect;
	_Rect	GetRect();

	__declspec(property(get = GetTiles))
	Tile**	Tiles;
	Tile**	GetTiles() const;

	Tile*	Copy();

	void	Clear();
	void	Clear(Tile* tile);

	static byte	IndexOf(int x, int y);

private:
	Tile*	m_parent;
	int		m_level;

	_Image*	m_image;
	_Rect	m_rect;

	Tile**	m_tiles;
};

#endif