#ifndef _MAPTILE_H_
#define _MAPTILE_H_

enum TileType
{
	GROUND = 0,
	RIVER
};

class Token;
class Area;
class MapTile
{
public:
	MapTile() {}
	MapTile(MapTile* tile);
	MapTile(TileType type) : tile_type(type) {}
	MapTile(TileType type, int x, int y) : tile_type(type), position_x(x), position_y(y) {}
	const bool IsEmpty() const { return token == nullptr; };
	const bool IsRiver() const { return tile_type == TileType::RIVER; };
	void SetToken(Token* new_token);
	Token* GetToken() const { return token; }
	void RemoveToken();
	void SetAreaParent(Area* area) { area_parent = area; }
	Area* GetAreaParent() { return area_parent; }

	const bool BelongsToKingdom();
	void JoinToKingdom() { belongs_to_kingdom = true; }
	void LeaveKingdom() { belongs_to_kingdom = false; }

	int position_x;
	int position_y;
private:
	Area* area_parent = nullptr;
	bool belongs_to_kingdom = false;
	Token* token = nullptr; //the map tile could have a token placed by a player

	TileType tile_type;
};

#endif //_MAPTILES_H_