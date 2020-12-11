#include "Area.h"
#include "MapTile.h"
#include "Map.h"

MapTile* Area::GetTile(int x, int y)
{
	for (int i = 0; i < int(tiles.size()); ++i)
	{
		if (tiles[i]->position_x == x && tiles[i]->position_y == y)
			return tiles[i];
	}

	return nullptr;
}

Area* Map::GetAreaByTile(int x, int y)
{
	Area* area = areas[GetTile(x, y)->GetAreaParent()->GetId()];
	return area;
}