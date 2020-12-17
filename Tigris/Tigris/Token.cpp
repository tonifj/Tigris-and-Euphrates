#include "Token.h"
#include "MapTile.h"

void Token::SetColor(MyTokenType type)
{
	if (type == MyTokenType::KING || type == MyTokenType::SETTLEMENT)
		token_color = TokenColor::BLACK;

	else if (type == MyTokenType::PRIEST || type == MyTokenType::TEMPLE)
		token_color = TokenColor::RED;

	else if (type == MyTokenType::FARMER || type == MyTokenType::FARM)
		token_color = TokenColor::BLUE;

	else if (type == MyTokenType::MERCHANT || type == MyTokenType::MARKET)
		token_color = TokenColor::GREEN;
}

Token::Token(MyTokenType type)
{
	token_type = type;
	SetColor(type);
}

Token::Token(std::string type)
{
	if (type == "settlement")
		token_type = MyTokenType::SETTLEMENT;

	else if (type == "farm")
		token_type = MyTokenType::FARM;

	else if (type == "market")
		token_type = MyTokenType::MARKET;

	else if (type == "temple")
		token_type = MyTokenType::TEMPLE;

	SetColor(token_type);
}

Token::Token(MyTokenType type, PlayerType faction) : token_type(type), faction(faction) { SetColor(type); }

Token::Token(MyTokenType type, MapTile* parent) : token_type(type)
{
	//adjacents = vector <Token*>();
	//adjacents.reserve(4);

	SetColor(type);

	if (type == MyTokenType::FARM)
		tile_parent = parent;

	else
		tile_parent = parent;
}

bool Token::operator==(Token& token)
{
	return	token.GetType() == token_type && 
			token.GetTileParent()->position_x == tile_parent->position_x && 
			token.GetTileParent()->position_y == tile_parent->position_y;
}