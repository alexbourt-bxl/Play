#include "Space.h"

#include <Types/Graphics/Image8.h>
#include <Graphics/Graphics.h>

#include <assert.h>

DEFINE_TYPE(Space, Object);

// TODO: Space should derive from Image

Space::Space()
	: m_imageType(TYPEOF(Image8)), m_fpath(8)
{
	m_tileSize = Size(128, 128),
	m_depth = 16;

	m_tl = new Tile(0);
	m_tr = new Tile(0);
	m_bl = new Tile(0);
	m_br = new Tile(0);
}

Space::Space(const Space& space)
	: m_imageType(space.m_imageType), m_fpath(8)
{
	m_tileSize = space.m_tileSize;
	m_depth = space.m_depth;

	m_tl = space.m_tl->Copy();
	m_tr = space.m_tr->Copy();
	m_bl = space.m_bl->Copy();
	m_br = space.m_br->Copy();
}

Space::Space(Type tileType)
	: m_imageType(tileType), m_fpath(8)
{
	m_tileSize = Size(128, 128),
	m_depth = 16;

	m_tl = new Tile(0);
	m_tr = new Tile(0);
	m_bl = new Tile(0);
	m_br = new Tile(0);
}
	
Space::Space(int tileWidth, int tileHeight, Type tileType)
	: m_imageType(tileType), m_fpath(8)
{
	m_tileSize = Size(
		tileWidth, 
		tileHeight),
	m_depth = 16;

	m_tl = new Tile(0);
	m_tr = new Tile(0);
	m_bl = new Tile(0);
	m_br = new Tile(0);
}

Space::~Space()
{
	Clear();
}

void Space::Clear()
{
	delete m_tl; m_tl = null;
	delete m_tr; m_tr = null;
	delete m_bl; m_bl = null;
	delete m_br; m_br = null;
}

Space& Space::operator = (const Space& space)
{
	Clear();

	m_tileSize = space.m_tileSize;
	m_depth = space.m_depth;

	m_tl = space.m_tl->Copy();
	m_tr = space.m_tr->Copy();
	m_bl = space.m_bl->Copy();
	m_br = space.m_br->Copy();

	return *this;
}

Object* Space::Copy()
{
	return new Space(*this);
}

Type Space::GetImageType()
{
	return m_imageType;
}

Size Space::GetTileSize()
{
	return m_tileSize;
}

//Color Space::GetPixel(int x, int y)
//{
//	if (Tile* tile = GetTile(x, y, false))
//	{
//		return tile->Image->GetPixel(
//			x - tile->Rect.X, 
//			y - tile->Rect.Y);
//	}
//
//	return Color::Transparent;
//}

Rect Space::GetContentRect()
{
	Rect contentRect;

	contentRect.Expand(GetContentRect(m_tl));
	contentRect.Expand(GetContentRect(m_tr));
	contentRect.Expand(GetContentRect(m_bl));
	contentRect.Expand(GetContentRect(m_br));

	return contentRect;
}

Rect Space::GetContentRect(Space::Tile* tile)
{
	Rect rect;

	if (tile->Level < 0)
	{
		rect = Graphics::GetContentRect(tile->Image);
		rect.Offset(tile->Rect.Position);
	}
	else
	{
		for (int i = 0; i < (signed)SQR(m_depth); i++)
		{
			if (tile->Tiles[i])
				rect.Expand(GetContentRect(tile->Tiles[i]));
		}
	}

	return rect;
}

Space::Tile* Space::GetTile(const Point& p, bool create)
{
	return GetTile(p.X, p.Y, create);
}

Space::Tile* Space::GetTile(int x, int y, bool create)
{
	// Get the tile-coordinates

	int tx = x / m_tileSize.Width;
	int ty = y / m_tileSize.Height;

	// Get the top-left coordinates of the tile

	int left = tx * m_tileSize.Width;  
	int top  = ty * m_tileSize.Height;

	/*	NOTE: Although the above looks stupid at first, 
		it is not a pointless exercise in division and 
		multiplication, because the values are correctly 
		rounded in the process. */

	// Select the quadrant
	
	Tile** qo = null; // quadrant from the origin
	
	if (x < 0 && y < 0) // top-left
	{
		qo = &m_tl;
		left -= m_tileSize.Width; 
		top -= m_tileSize.Height;
	}
	else if (x >= 0 && y < 0) // top-right
	{
		qo = &m_tr;
		top -= m_tileSize.Height;
	}
	else if (x < 0 && y >= 0) // bottom-left
	{
		qo = &m_bl;
		left -= m_tileSize.Width;
	}
	else if (x >= 0 && y >= 0) // bottom-right
	{ 
		qo = &m_br; 
	}

	assert(qo);

	// Get the fractal coordinates of the tile

	int lvl = 0;

	tx = abs(tx);	
	ty = abs(ty);

	while (tx >= 0 || ty >= 0)
	{
		m_fpath[lvl] = Tile::IndexOf(tx % m_depth, ty % m_depth);
		
		tx /= m_depth; 
		ty /= m_depth;

		if (tx == 0 && ty == 0)
			break;

		lvl++;
	}

	/*	If there are more levels to go, this means
		the coordinates are close to the origin. */

	while ((*qo)->Level > lvl)
		m_fpath[++lvl] = 0;

	// Come up to the top, creating levels as neeeded

	if ((*qo)->Level < lvl && !create)
		return null;

	while ((*qo)->Level < lvl)
	{
		Tile* up = new Tile((*qo)->Level + 1);

		up->Tiles[0] = (*qo);
		(*qo)->m_parent = up;

		(*qo) = up;
	}

	Tile* tile = (*qo);

	// Now go back down into the space towards the tile
	
	while (lvl > 0)
	{
		if (!tile->Tiles[m_fpath[lvl]])
		{
			if (create)	
			{
				tile->Tiles[m_fpath[lvl]] = new Tile(lvl - 1);
				tile->Tiles[m_fpath[lvl]]->m_parent = tile;
			}
			else return null;
		}

		tile = tile->Tiles[m_fpath[lvl--]];
	}

	assert(lvl == 0);

	// Get or create the -1 level tile

	if (tile->Tiles[m_fpath[lvl]])
	{
		return tile->Tiles[m_fpath[lvl]];
	}
	else if (create)
	{
		tile->Tiles[m_fpath[lvl]] = new Tile(Image::Create(
			m_tileSize.Width, 
			m_tileSize.Height, 
			m_imageType));

		tile->Tiles[m_fpath[lvl]]->m_parent = tile;

		tile = tile->Tiles[m_fpath[lvl]];
		
		tile->m_rect = Rect(
			left, 
			top, 
			m_tileSize.Width, 
			m_tileSize.Height);
		
		return tile;
	}

	return null;
}

Array<Space::Tile*>& Space::GetTiles(const Rect& rect, bool create)
{
	int left = (rect.X / m_tileSize.Width) * m_tileSize.Width; 
	if (rect.X < 0) left -= m_tileSize.Width;
	
	int right = ((rect.X + rect.Width - 1) / m_tileSize.Width) * m_tileSize.Width; 
	if (rect.X + rect.Width < 0) right -= m_tileSize.Width;

	int top = (rect.Y / m_tileSize.Height) * m_tileSize.Height;
	if (rect.Y < 0) top -= m_tileSize.Height;

	int bottom = ((rect.Y + rect.Height - 1) / m_tileSize.Height) * m_tileSize.Height;
	if (rect.Y + rect.Height < 0) bottom -= m_tileSize.Height;

	m_returnArray.Clear();

	Tile* tile = null;

	for (int y = top; y <= bottom; y += m_tileSize.Height)
	{
		for (int x = left; x <= right; x += m_tileSize.Width)
		{
			if (Rect(x, y, m_tileSize.Width, m_tileSize.Height).Intersects(rect) 
				&& (tile = GetTile(Point(x + m_tileSize.Width / 2, y + m_tileSize.Height / 2), create)) != null)
			{
				m_returnArray.Add(tile);
			}
		}
	}

	return m_returnArray;
}