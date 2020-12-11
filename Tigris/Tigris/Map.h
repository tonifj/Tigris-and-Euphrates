#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include <unordered_map>

class Token;
class MapTile;
class Area;
class Map
{
	MapTile* tiles[16][11];

public:
	Map();
	void PrintMap() const;
	void UpdateMap(Token* token, int x, int y);
	MapTile* GetTile(int x, int y) { return tiles[x][y]; }
	bool IsValidTile(int x, int y);
	std::vector <MapTile*> GetAdjacentsToTile(int x, int y);
	std::unordered_map<int, Area*> GetAreas() { return areas; }
	void AddArea(Area* area);
	void AddArea(int id, Area* area) { areas[id] = area; }

	Area* GetAreaByTile(int x, int y);
	Area* GetAreaById(int id) { return areas[id]; }


private:
	std::unordered_map<int, Area*> areas;
};

#endif; //_MAP_H_
