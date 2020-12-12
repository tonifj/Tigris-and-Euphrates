#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

enum MyTokenType
{
	SETTLEMENT = 0,
	TEMPLE,
	MARKET,
	FARM,
	CATASTROPHE,
	KING,
	PRIEST,
	MERCHANT,
	FARMER,
	INVALID
};

enum TokenColor
{
	RED,
	BLUE,
	GREEN,
	BLACK
};

enum PlayerType;
class MapTile;
class Token
{
public:
	Token(MyTokenType type);
	Token(MyTokenType type, PlayerType faction);
	Token(MyTokenType type, MapTile* parent);
	Token(std::string type);
	MyTokenType GetType() { return token_type; };
	MapTile* GetTileParent() { return tile_parent; }
	void SetParent(MapTile* new_parent) { tile_parent = new_parent; }
	void SetFaction(PlayerType new_faction) { faction = new_faction; }
	PlayerType GetFaction() { return faction; }
	MapTile* tile_parent = nullptr;

protected:
	void SetColor(MyTokenType type);
	MyTokenType token_type;
	TokenColor token_color;

	//Only for leaders!
	PlayerType faction;

};

#endif