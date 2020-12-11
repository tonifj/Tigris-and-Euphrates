#include <iostream>
#include <vector>

#include "Player.h"
#include "Token.h"
#include "Map.h"
#include "MapTile.h"

using namespace std;

Token* Player::GetLeader(TokenType type)
{
	for (int i = 0; i < int(leaders.size()); ++i)
	{
		if (leaders[i]->GetType() == type)
			return leaders[i];
	}
}

Player::Player(PlayerType faction, vector<Token*>& tokens, int new_turn) : faction(faction)
{
	points.amulets = 0;
	points.bricks = 0;
	points.crops = 0;
	points.goods = 0;

	deck = tokens;

	turn = new_turn;

	leaders.reserve(4);
	leaders.push_back(new Token(TokenType::KING, faction));
	leaders.push_back(new Token(TokenType::FARMER, faction));
	leaders.push_back(new Token(TokenType::MERCHANT, faction));
	leaders.push_back(new Token(TokenType::PRIEST, faction));

	cout << "Your faction is: ";

	switch (faction)
	{
	case PlayerType::BOW:
			std::cout << " BOW" << endl;
			break;

	case PlayerType::BULL:
		std::cout << " BULL" << endl;
		break;

	case PlayerType::LION:
		std::cout << " LION" << endl;
		break;

	case PlayerType::POT:
		std::cout << " POT" << endl;
		break;
	}
}

void Player::Refresh()
{

}

void Player::ShowPlayer()
{
	//cout << "Faction: ";

	//switch (faction)
	//{
	//case PlayerType::BULL:
	//	cout << "bull" << endl;
	//	break;

	//case PlayerType::BOW:
	//	cout << "bow" << endl;
	//	break;

	//case PlayerType::LION:
	//	cout << "lion" << endl;
	//	break;

	//case PlayerType::POT:
	//	cout << "pot" << endl;
	//	break;
	//}

	for (int i = 0; i < int(deck.size()); ++i)
		cout << "Card " << i << ": " << deck[i]->GetType() << endl;


}

void Player::AddPoint(TokenColor c)
{

}

bool Player::PlaceToken(Map* map, TokenType type, int x, int y)
{
	if (map->GetTile(x, y)->IsRiver())
	{
		if (type == TokenType::FARM)
		{
			map->UpdateMap(new Token(type, x, y), x, y);

			DiscardUsedToken(type);
			return true;
		}

		else
		{
			cout << "exception: only farms can be placed in a river tile" << endl;
			return false;
		}
	}

	else
	{
		if (type != TokenType::FARM)
		{
			map->UpdateMap(new Token(type, x, y), x, y);

			DiscardUsedToken(type);
			return true;
		}

		else
		{
			cout << "exception: only farms can be placed in a river tile" << endl;
			return false;
		}
	}
}

void Player::DiscardUsedToken(TokenType type)
{
	int position_to_remove_token = 0;

	bool found = false;
	for (int i = 0; i < int(deck.size()) && !found; ++i)
	{
		if (deck[i]->GetType() == type)
		{
			position_to_remove_token = i;
			found = true;
		}
	}

	deck.erase(deck.begin() + position_to_remove_token);
	deck.shrink_to_fit();
	//ShowPlayer(); // ho faig per veure que s'ha borrat bé
}
