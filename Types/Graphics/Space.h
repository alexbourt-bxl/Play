#ifndef SPACE_H
#define SPACE_H

#include <Types/Graphics/Image.h>
#include <Containers/Array.h>

/*	A Space is composed of four tile grids that extend from the origin, one for each quadrant, 
	and grows exponentially as new tiles are created.

	The tile system allows for a fractal division of space in a way that uses very little housekeeping 
	memory. Tiles can also be quickly found from coordinates by fractal recursion.

	Each tile can hold 256 slots (16x16). The memory footprint per tile is 1 KB (256 x 4 B) on 32-bit
	systems, and 2 KB (256 x 8 B) on 64-bit systems.
	
	Tile positions are encoded in the array index. The index byte is divided in the following way:

		 1111  1111
		  Y     X

	Each slot can either hold another tile grid or an image.

	If a tile needs to be created outside of the area currently covered by the grid, then the grid
	grows in size logarithmically until there is a slot to hold a tile at those coordinates.

	Levels start at 0 and go up.

	A tile of level -1 holds the actual pixels in an Image.
*/

class Space 
	: public Object
{
	DECLARE_TYPE;

public:
	class Tile;

public:
	Space();
	Space(const Space& space);
	Space(Type tileType);
	Space(int tileWidth, int tileHeight, Type imageType);
	~Space();

	void	Clear();

	Space&	operator = (const Space& space);

	Object*	Copy();

	__declspec(property(get = GetImageType))
	Type	ImageType;
	Type	GetImageType();

	__declspec(property(get = GetTileSize))
	Size	TileSize;
	Size	GetTileSize();

	//Color	GetPixel(int x, int y);

	__declspec(property(get = GetContentRect))
	Rect	ContentRect;
	Rect	GetContentRect();
	Rect	GetContentRect(Tile* tile);

	Tile*	GetTile(const Point& p, bool create = false);
	Tile*	GetTile(int x, int y, bool create = false);

	Array<Tile*>& GetTiles(const Rect& rect, bool create = false);

private:
	Type	m_imageType; // this is used to create tiles

	Size	m_tileSize;
	uint	m_depth; // 2 | 4 | 8 | 16

	Tile*	m_tl;	//
	Tile*	m_tr;	// quadrants
	Tile*	m_bl;	//
	Tile*	m_br;	//

	Array<byte>  m_fpath;
	Array<Tile*> m_returnArray;
};

#include "Space_Tile.h"

#endif