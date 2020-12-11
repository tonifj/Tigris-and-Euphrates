#include "Token.h"
#include "MapTile.h"

void Token::SetColor(TokenType type)
{
	if (type == TokenType::KING || type == TokenType::SETTLEMENT)
		token_color = TokenColor::BLACK;

	else if (type == TokenType::PRIEST || type == TokenType::TEMPLE)
		token_color = TokenColor::RED;

	else if (type == TokenType::FARMER || type == TokenType::FARM)
		token_color = TokenColor::BLUE;

	else if (type == TokenType::MERCHANT || type == TokenType::MARKET)
		token_color = TokenColor::GREEN;
}

Token::Token(TokenType type) : token_type(type)
{
	SetColor(type);
}

Token::Token(std::string type)
{
	if (type == "settlement")
		token_type = TokenType::SETTLEMENT;

	else if (type == "farm")
		token_type = TokenType::FARM;

	else if (type == "market")
		token_type = TokenType::MARKET;

	else if (type == "temple")
		token_type = TokenType::TEMPLE;

	SetColor(token_type);
}

Token::Token(TokenType type, PlayerType faction) : token_type(type), faction(faction) { SetColor(type); }

Token::Token(TokenType type, int x, int y) : token_type(type)
{
	//adjacents = vector <Token*>();
	//adjacents.reserve(4);

	SetColor(type);

	if (type == TokenType::FARM)
		tile_parent = new MapTile(TileType::RIVER, x, y);

	else
		tile_parent = new MapTile(TileType::GROUND, x, y);
}
