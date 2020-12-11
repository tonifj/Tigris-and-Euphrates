#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

enum TokenType
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
	Token(TokenType type);
	Token(TokenType type, PlayerType faction);
	Token(TokenType type, int x, int y);
	Token(std::string type);
	TokenType GetType() { return token_type; };
	MapTile* GetTileParent() { return tile_parent; }
	void SetParent(MapTile* new_parent) { tile_parent = new_parent; }
	void SetFaction(PlayerType new_faction) { faction = new_faction; }
	PlayerType GetFaction() { return faction; }
	MapTile* tile_parent = nullptr;

protected:
	void SetColor(TokenType type);
	TokenType token_type;
	TokenColor token_color;

	//Only for leaders!
	PlayerType faction;

};

#endif