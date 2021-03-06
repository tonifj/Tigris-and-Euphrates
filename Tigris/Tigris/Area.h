#ifndef _AREA_H_
#define _AREA_H_

#include <vector>

class MapTile;
class Token;
enum MyTokenType;
class Area
{
public:
	Area() { id = -1; }
	Area(int id) : id(id) {}
	Area(MapTile* initial_tile);
	Area(MapTile* initial_tile, int new_id);

	void AddTile(MapTile* tile);

	MapTile* GetTile(int x, int y);
	int GetId() { return id; }
	int GetNumOfLeaders();
	int GetNumSupportersForLeader(Token* leader);
	Area* GetAdjacentArea() { return adjacent_area; }

	bool IsSameAs(Area* area) { if (area != nullptr) return id == area->id; else return new Area(-1); }
	bool IsKingdom();

	std::vector <Token*> GetLeaders();
	std::vector<MapTile*> GetTiles() { return tiles; }
	Token* GetLeader(MyTokenType type);
	void RemoveTile(int x, int y);
	void SetWarAvailable() { war_availabe = true; }
	void SetWarNotAvailable() { war_availabe = false; }
	void SetRevoltAvailable() { revolt_available = true; }
	void SetRevoltNotAvailable() { revolt_available = false; }
	void SetAdjacentKingdom(Area* area) { area->adjacent_area = this; adjacent_area = area; }
	bool CheckValidLeader(MyTokenType type);
	Token* FindToken(MyTokenType type);
	bool IsTokenHere(MyTokenType type);

	bool WarAvailable() { return war_availabe; }
	bool RevoltAvailable() { return revolt_available; }


private:

	int id;
	std::vector<MapTile*> tiles;
	bool war_availabe = false;
	bool revolt_available = false;
	Area* adjacent_area = nullptr;
};

#endif