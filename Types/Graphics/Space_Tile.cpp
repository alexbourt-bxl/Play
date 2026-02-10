#include "Space.h"

#include <stdlib.h>

Space::Tile::Tile(int level)
{
	m_parent = null;
	m_level  = level;

	m_image = null;
	
	m_tiles = 
		m_level < 0
		? null
		: (Tile**)calloc(256, sizeof(Tile*));
}

Space::Tile::Tile(_Image* image)
{
	m_parent = null;
	m_level  = -1; // This is only for the wrapper tiles that contain actual images

	m_image  = image;
	m_tiles  = null;
}

Space::Tile::~Tile()
{
	Clear();
}

Space::Tile* Space::Tile::GetParent() const
{
	return m_parent;
}

int Space::Tile::GetLevel()
{
	return m_level;
}

Image* Space::Tile::GetImage() const
{
	return m_image;
}

Rect Space::Tile::GetRect()
{
	return m_rect;
}

Space::Tile** Space::Tile::GetTiles() const
{
	return m_tiles;
}

Space::Tile* Space::Tile::Copy()
{
	if (m_image)
	{
		Tile* tile = new Tile((_Image*)m_image->Copy());
		tile->m_rect = m_rect;
		return tile;
	}
	else
	{
		Tile* tile = new Tile(Level);

		for (int i = 0; i < 256; i++)
		{
			if (m_tiles[i])
			{
				tile->m_tiles[i] = m_tiles[i]->Copy();
				tile->m_tiles[i]->m_parent = tile;
			}
		}

		return tile;
	}
}

void Space::Tile::Clear()
{
	if (m_image)
	{
		delete m_image;
		m_image = null;
	}

	if (m_tiles)
	{
		for (int i = 0; i < 256; i++)
		{
			if (m_tiles[i])
			{
				delete m_tiles[i];
				m_tiles[i] = null;
			}
		}

		free(m_tiles);
		m_tiles = null;
	}
}

void Space::Tile::Clear(Tile* tile)
{
	for (int i = 0; i < 256; i++)
	{
		if (m_tiles[i] == tile)
		{
			delete m_tiles[i];
			m_tiles[i] = null;
			break;
		}
	}

	// Check if any child tiles remain

	bool present = false;

	for (int i = 0; i < 256; i++)
	{
		if (m_tiles[i])
		{
			present = true;
			break;
		}
	}

	// If not, remove this tile completely

	if (!present && m_parent)
		m_parent->Clear(this);
}

byte Space::Tile::IndexOf(int x, int y)
{
	// This function returns the array index of the tile
	return (byte)(((y & 0xF) << 4) | (x & 0xF));
}