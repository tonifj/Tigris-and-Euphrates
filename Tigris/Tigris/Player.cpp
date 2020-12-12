#include <iostream>
#include <vector>

#include "Player.h"
#include "Token.h"
#include "Map.h"
#include "MapTile.h"
#include "Area.h"

using namespace std;

Token* Player::GetLeader(MyTokenType type)
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
	leaders.push_back(new Token(MyTokenType::FARMER, faction));
	leaders.push_back(new Token(MyTokenType::KING, faction));
	leaders.push_back(new Token(MyTokenType::MERCHANT, faction));
	leaders.push_back(new Token(MyTokenType::PRIEST, faction));

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

void Player::ShowPlayer()
{

	for (int i = 0; i < int(deck.size()); ++i)
	{
		switch (deck[i]->GetType())
		{
		case MyTokenType::FARM:
			cout << " farm ";
			break;


		case MyTokenType::TEMPLE:
			cout << " temple ";
			break;


		case MyTokenType::MARKET:
			cout << " market ";
			break;

		case MyTokenType::SETTLEMENT:
			cout << " settlement ";
			break;
		}
	}
}




void Player::AddPoint(TokenColor c)
{

}

bool Player::PlaceToken(Map* map, MyTokenType type, int x, int y)
{
	if (map->GetTile(x, y)->IsRiver())
	{

		if (type == MyTokenType::FARM)
		{
			map->UpdateMap(new Token(type, map->GetTile(x, y)), x, y);
			map->PrintMap();
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
		if (type != MyTokenType::FARM)
		{
			if (type == type == MyTokenType::KING || type == MyTokenType::MERCHANT || type == MyTokenType::PRIEST || type == MyTokenType::FARMER)
			{
				map->UpdateMap(new Token(type, map->GetTile(x, y)), x, y);
				map->PrintMap();
			}

			else
			{
				map->UpdateMap(new Token(type, map->GetTile(x, y)), x, y);
				map->PrintMap();
				DiscardUsedToken(type);
			}

			return true;
		}

		else
		{
			cout << "exception: only farms can be placed in a river tile" << endl;
			return false;
		}
	}
}

void Player::DiscardUsedToken(MyTokenType type)
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
}

//Returns false if there is no need to dissolve a kingdom
bool Player::MoveLeader(Map* map, Area* area, MyTokenType type, int x, int y)
{
	//check if the leader is placed somewhere
	if (GetLeader(type)->GetTileParent() != nullptr)
	{
		//check if the tile where the leader was placed was a kingdom and if it had more that 2 or more leaders
		if (map->GetAreaByTile(GetLeader(type)->GetTileParent()->position_x, GetLeader(type)->GetTileParent()->position_y)->IsKingdom() &&
			map->GetAreaByTile(GetLeader(type)->GetTileParent()->position_x, GetLeader(type)->GetTileParent()->position_y)->GetNumOfLeaders() >= 2)
		{
			//Remove token from old tile
			GetLeader(type)->GetTileParent()->RemoveToken();

			//Set new parent
			GetLeader(type)->SetParent(map->GetTile(x, y));

			//Add token to new tile
			map->GetTile(x, y)->SetToken(GetLeader(type));

			//Add tile to area
			area->AddTile(map->GetTile(x, y));

			map->UpdateMap(map->GetTile(x, y)->GetToken(), x, y);
			map->PrintMap();

			//this indicates if it's needed to dissolve a kingdom
			return false;
		}

		else 
		{
			//remove area from the container
			map->GetAreas().erase(GetLeader(type)->GetTileParent()->GetAreaParent()->GetId());

			//Remove token from old tile
			GetLeader(type)->GetTileParent()->RemoveToken();

			//Set new parent
			GetLeader(type)->SetParent(map->GetTile(x, y));

			//Add token to new tile
			map->GetTile(x, y)->SetToken(GetLeader(type));

			//Add new tile to the area
			area->AddTile(map->GetTile(x, y));

			map->UpdateMap(map->GetTile(x, y)->GetToken(), x, y);
			map->PrintMap();

			return true;
		}
	}
	
	else
	{
		//Set new parent
		GetLeader(type)->SetParent(map->GetTile(x, y));

		//Add token to new tile
		map->GetTile(x, y)->SetToken(GetLeader(type));

		//create new area
		Area* new_area = new Area(map->GetTile(x, y));
		map->AddArea(new_area);

		map->UpdateMap(map->GetTile(x, y)->GetToken(), x, y);
		map->PrintMap();

		return false;
	}


}