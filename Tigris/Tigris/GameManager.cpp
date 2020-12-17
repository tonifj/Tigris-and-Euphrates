#include <iostream>
#include <climits>
#include <windows.h>
#include <queue>

#include "GameManager.h"
#include "Map.h"
#include "MapTile.h"
#include "Player.h"
#include "Token.h"
#include "Area.h"


#define MAX_DECK_SIZE 6
#define MAX_PLAYERS 4
#define MIN_PLAYERS 2

using namespace std;

GameManager::GameManager()
{
	map = new Map();
	map->PrintMap();

	available_types.push(PlayerType::BOW);
	available_types.push(PlayerType::BULL);
	available_types.push(PlayerType::POT);
	available_types.push(PlayerType::LION);
}

void GameManager::UpdateArea(Area* area, MapTile* new_tile)
{
	area->AddTile(new_tile);
}

void GameManager::ClearInput()
{
	cin.ignore(INT_MAX, '\n');  cin.clear();
}

void GameManager::WaitForPlayers()
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);

	bool waiting_more_players = true;
	int token_counter = 0;

	string token_name;

	vector<Token*> temp_player_tokens;
	temp_player_tokens.reserve(MAX_DECK_SIZE);

	cout << "Waiting new players. To declare yourself as a player, write 6 cards you'd like to have among these 4 types: " << endl;
	cout << "settlement - farm - temple - market" << endl;
	cout << "Once you finish, write '----' and begin the game!" << endl;

	//while we're waiting more players
	while (waiting_more_players && player_count < MAX_PLAYERS && cin >> token_name)
	{

		//detect ---- to tell the game we don't want more players
		if (token_name == "----" && token_counter == 0)
		{
			if (player_count < MIN_PLAYERS)
				cout << "You need one more player :'D" << endl;

			else
			{
				waiting_more_players = false;
				cout << "No more players!" << endl;
			}
		}

		//Detect tiles that the player wants
		else
		{
			if (token_name == "settlement")
			{
				temp_player_tokens.push_back(new Token(MyTokenType::SETTLEMENT));
				++token_counter;
			}

			else if (token_name == "temple")
			{
				temp_player_tokens.push_back(new Token(MyTokenType::TEMPLE));
				++token_counter;
			}

			else if (token_name == "farm")
			{
				temp_player_tokens.push_back(new Token(MyTokenType::FARM));
				++token_counter;
			}

			else if (token_name == "market")
			{
				temp_player_tokens.push_back(new Token(MyTokenType::MARKET));
				++token_counter;
			}

			else
			{
				cout << "enter a valid token!" << endl;
				ClearInput();
			}
		}

		//Once we have 6 tiles, create a player object with those tiles and a random faction
		if (token_counter != 0 && token_counter % (MAX_DECK_SIZE) == 0)
		{
			CreatePlayer(temp_player_tokens);
			cout << "Welcome player!" << endl;
			++player_count;
			token_counter = 0;
			temp_player_tokens.clear();
			temp_player_tokens.shrink_to_fit();
			actual_player_turn_actions = 0;
		}
	}
	ClearInput();
	cout << "The game starts!" << endl;
}

void GameManager::CreatePlayer(vector<Token*>& tokens)
{
	players.push_back(new Player(available_types.front(), tokens, player_count));
	available_types.pop();
}

bool GameManager::ReadCommand(Player* player, string& command, int turn_actions)
{
	if (turn_actions < 2)
	{
		if (command == "tile")
			return CommandTile(player);

		else if (command == "refresh")
			return CommandRefresh(player);

		else if (command == "deck")
		{
			cout << "You got: " << endl;
			current_player->ShowPlayer();
		}

		else if (command == "leader")
		{
			return CommandLeader(player);
		}

		else if (command == "treasure")
		{
			return CommandTreasure(player);
		}

		else if (command == "catastrophe")
		{
			//do catastrophe stuff
		}

		else if (command == "revolt")
		{
			return CommandRevolt();
		}

		else if (command == "war")
		{
			//become USA
		}

		else if (command == "monument")
		{
			return CommandMonument();
		}

		else if (command == "help")
		{
			cout << "Type the folowing commands: " << endl;
			cout << "tile: to place a tile you have in your deck" << endl;
			cout << "refresh: draw tiles from the bag into your hand!" << endl;
			cout << "leader: to place a leader" << endl;
			cout << "treasure: to pick a sure" << endl;
			cout << "catastrophe: to place a catastrophe tile" << endl;
			cout << "revolt: to begin a revolt in a kingdom" << endl;
			cout << "war: to begin a war between two kingdoms" << endl;
			cout << "monument: to build a monument" << endl;
			cout << "deck: to check your deck" << endl;

			cout << "- - - - - - - - - - - - - - - -" << endl;
			cout << "Map info:" << endl;
			cout << "- - - - - - - - - - - - - - - -" << endl;
			cout << "*: river" << endl;
			cout << "-: land" << endl;
			cout << "F: farmer / f: farm" << endl;
			cout << "M: merchant / f: market" << endl;
			cout << "T: temple / p: priest" << endl;
			cout << "K: king / s: settlement" << endl;

			return false;
		}
	}

	else
	{
		if (command == "----")
		{
			player->EndTurn();
			actual_player_turn_actions = 0;
			return true;
		}

		else if (command == "help")
			cout << "Now, you have to pass your turn by typing '----'";

		else
			cout << "exception: too many turn actions" << endl;
	}

	return false;
}

bool GameManager::CheckTokenAvailability(Player* player, MyTokenType type)
{
	//Check if the player has the token
	for (int i = 0; i < int(player->GetDeck().size()); ++i)
	{
		if (player->GetDeck()[i]->GetType() == type)
			return true;
	}

	return false;
}

bool GameManager::CommandTile(Player* player)
{
	string type;
	cin >> type;

	if (type == "settlement")
		return ProcessTile(player, MyTokenType::SETTLEMENT);

	else if (type == "farm")
		return ProcessTile(player, MyTokenType::FARM);

	else if (type == "market")
		return ProcessTile(player, MyTokenType::MARKET);

	else if (type == "temple")
		return ProcessTile(player, MyTokenType::TEMPLE);

	cout << "exception: could not parse tile type" << endl;
	return false;

}

bool GameManager::CheckTileAvailability(int x, int y)
{
	//Check if it's empty
	return map->GetTile(x, y)->IsEmpty();
}

bool GameManager::CheckValidTile(int x, int y)
{
	//returns if the x and y belong to the board
	return map->IsValidTile(x, y);
}

Player* GameManager::GetPlayerByDinasty(PlayerType type)
{
	for (int i = 0; players.size(); ++i)
	{
		if (players[i]->GetFaction() == type)
			return players[i];
	}
}

bool GameManager::CommandLeader(Player* player)
{
	string leader_type;
	cin >> leader_type;

	if (CheckValidLeader(leader_type)) //is a leader introduced correctly?
	{
		int x, y;
		cin >> x >> y;
		if (CheckValidTile(x, y)) // is a valid tile?
		{
			if (!map->GetTile(x, y)->IsRiver())
			{
				if (CheckTileAvailability(x, y)) // is the tile empty?
				{
					//proceed
					return ProcessLeader(player->GetLeader(TranslateStringToToken(leader_type)), x, y);
				}

				else
				{
					cout << "exception: board space is already occupied" << endl;
					return false;
				}
			}

			else
			{
				cout << "exception: cannot place a leader in a river tile" << endl;
				return false;
			}
		}

		else
		{
			cout << "exception: invalid space position" << endl;
			return false;
		}
	}
	cout << "exception: could not parse leader type" << endl;
	return false;
}

MyTokenType GameManager::TranslateStringToToken(string name)
{
	if (name == "king")
		return MyTokenType::KING;
	else if (name == "merchant")
		return MyTokenType::MERCHANT;
	else if (name == "farmer")
		return MyTokenType::FARMER;
	else if (name == "priest")
		return MyTokenType::PRIEST;
	else if (name == "settlement")
		return MyTokenType::SETTLEMENT;
	else if (name == "farm")
		return MyTokenType::FARM;
	else if (name == "temple")
		return MyTokenType::TEMPLE;
	else if (name == "market")
		return MyTokenType::MARKET;
}

bool GameManager::ProcessLeader(Token* leader, int x, int y)
{
	vector <MapTile*> adjacent_tokens = map->GetAdjacentsToTile(x, y);

	Token* old_region_token = nullptr;
	Token* old_kingdom_token = nullptr;

	int adjacent_kingdoms = 0;
	bool at_least_one_temple = false;
	bool too_many_kingdoms = false;
	bool leaders_of_same_color = false;
	bool activate_revolt = false;

	for (int i = 0; i < int(adjacent_tokens.size()); ++i)
	{
		//Check if the leader will be placed near a temple
		if (adjacent_tokens[i]->GetToken() != nullptr &&
			adjacent_tokens[i]->GetToken()->GetType() == MyTokenType::TEMPLE)
		{
			at_least_one_temple = true;

			//While the tile is not adjacent to 2 or more kingdoms
			if (adjacent_kingdoms <= 1)
			{
				old_region_token = adjacent_tokens[i]->GetToken();

				//update adjacent kingdom counter
				if (IsTokenPartOfKingdom(old_region_token))
				{
					old_kingdom_token = old_region_token;
					++adjacent_kingdoms;
				}
			}

			//in case we place the leader adjacent to two kingdoms
			else
			{
				too_many_kingdoms = true;
				at_least_one_temple = false;
				leaders_of_same_color = false;
				activate_revolt = false;
			}
		}
	}

	if (at_least_one_temple)
	{
		if (!too_many_kingdoms)
		{
			if (adjacent_kingdoms == 0)
			{
				current_player->MoveLeader(map, old_region_token->GetTileParent()->GetAreaParent(), leader->GetType(), x, y);
				return true;
			}

			else
			{
				current_player->MoveLeader(map, old_region_token->GetTileParent()->GetAreaParent(), leader->GetType(), x, y);

				if (AreThereLeadersOfTheSameColor(leader, old_region_token->GetTileParent()->GetAreaParent()))
				{
					cout << "A revolt is available now!" << endl;
					old_region_token->GetTileParent()->GetAreaParent()->SetRevoltAvailable();
				}
				return true;
			}
		}

		else
		{
			cout << "exception: cannot place a leader that would unite two kingdoms" << endl;
			return false;
		}
	}

	else
	{
		cout << "exception: cannot place a leader in a space without adjacent temple" << endl;
		return false;
	}
}

bool GameManager::AreThereLeadersOfTheSameColor(Token* incoming_leader, Area* area)
{

	for (int i = 0; i < int(area->GetTiles().size()); ++i)
	{
		if (area->GetTiles()[i]->GetToken() != nullptr && area->CheckValidLeader(area->GetTiles()[i]->GetToken()->GetType()))
		{
			if (incoming_leader->GetType() == area->GetTiles()[i]->GetToken()->GetType() &&
				incoming_leader->GetFaction() != area->GetTiles()[i]->GetToken()->GetFaction())
				return true;
		}

	}
	return false;
}

bool GameManager::IsTokenPartOfKingdom(Token* token)
{
	return 	token->GetTileParent()->GetAreaParent()->IsKingdom();
}

bool GameManager::ProcessTile(Player* player, MyTokenType type)
{
	int num_adjacent_tiles_belonging_to_kingdom = 0;
	int num_adj_tiles_belonging_to_region = 0;
	bool only_one_kingdom = true;
	bool only_one_region = true;

	Area* prev_kingdom = nullptr;
	Area* prev_region = nullptr;
	int x, y;
	if (CheckTokenAvailability(player, type))
	{
		cin >> x >> y;

		if (CheckValidTile(x, y))
		{
			if (CheckTileAvailability(x, y))
			{
				vector <MapTile*> adjacents = map->GetAdjacentsToTile(x, y);
				prev_kingdom = adjacents[0]->GetAreaParent();

				//check every adjacent tile where we want to put our tile
				for (int i = 0; i < adjacents.size() && only_one_kingdom; ++i)
				{
					if (adjacents[i]->BelongsToKingdom())
					{
						++num_adjacent_tiles_belonging_to_kingdom;
						//if the adj tile belongs to a kingdom and is not the same as the tile before
						if (!adjacents[i]->GetAreaParent()->IsSameAs(prev_kingdom))
							only_one_kingdom = false;

						else
							prev_kingdom = adjacents[i]->GetAreaParent();

						if (!only_one_kingdom)
						{
							cout << "Tile placed between two kingdoms" << endl;
							prev_kingdom->SetWarAvailable();
							adjacents[i]->GetAreaParent()->SetWarAvailable();

							adjacents[i]->GetAreaParent()->AddTile(map->GetTile(x, y));

							return player->PlaceToken(map, type, x, y);
						}
					}

					else if (!adjacents[i]->BelongsToKingdom() && adjacents[i]->GetToken() != nullptr)
					{
						++num_adj_tiles_belonging_to_region;

						if (!adjacents[i]->GetAreaParent()->IsSameAs(prev_region) && prev_region != nullptr)
							only_one_region = false;

						else
							prev_region = adjacents[i]->GetAreaParent();
					}
				}

				if (num_adjacent_tiles_belonging_to_kingdom == 0 && num_adj_tiles_belonging_to_region == 0)
				{
					map->GetTile(x, y)->SetToken(new Token(type));
					map->AddArea(new Area(map->GetTile(x, y)));
					return player->PlaceToken(map, type, x, y);
				}

				else if(num_adjacent_tiles_belonging_to_kingdom == 0 && num_adj_tiles_belonging_to_region != 0)
				{
					prev_region->AddTile(map->GetTile(x, y));
					return player->PlaceToken(map, type, x, y);
				}

				else
				{
					if (only_one_kingdom)
					{
						map->GetTile(x, y)->SetToken(new Token(type));
						prev_kingdom->AddTile(map->GetTile(x, y));

						//add points
						AddPointsTokenPlaced(prev_kingdom, map->GetTile(x, y)->GetToken());

						return player->PlaceToken(map, type, x, y);
					}

					

				}
			}

			cout << "exception: board space is already occupied" << endl;
			return false;
		}

		cout << "exception: invalid board space position" << endl;
		return false;
	}

	cout << "exception: could not find tile of specified type" << endl;
	return false;
}

bool GameManager::CommandTreasure(Player* player)
{
	int x, y;
	bool is_merchant_here = false;


	cin >> x >> y;

	if (CheckValidTile(x, y))
	{

		if (map->GetTile(x, y)->GetToken() != nullptr && map->GetTile(x, y)->GetToken()->GetType() == MyTokenType::TEMPLE)
		{
			if (map->GetTile(x, y)->GetToken()->HasTreasure())
			{
				for (int i = 0; i < map->GetAreaByTile(x, y)->GetLeaders().size() && !is_merchant_here; ++i)
				{
					if (map->GetAreaByTile(x, y)->GetLeaders()[i]->GetType() == MyTokenType::MERCHANT &&
						map->GetAreaByTile(x, y)->GetLeaders()[i]->GetFaction() == current_player->GetFaction())
					{
						is_merchant_here = true;
					}
				}

				if (is_merchant_here)
				{
					map->GetTile(x, y)->GetToken()->RemoveTreasure();
					player->AddTreasure();
				}

				else
				{
					cout << "exception: there is no merchant of your faction in the kingdom" << endl;
					return false;
				}
			}

			else
			{
				cout << "exception: this temple hasn't any treasure" << endl;
				return false;
			}
		}

		else
		{
			cout << "exception: treasures can only be found at temples" << endl;
			return false;
		}

	}

	else
	{
		cout << "exception: invalid board space position" << endl;
		return false;
	}
}

bool GameManager::CheckValidLeader(MyTokenType type)
{
	return type == MyTokenType::KING || type == MyTokenType::MERCHANT || type == MyTokenType::PRIEST || type == MyTokenType::FARMER;
}

bool GameManager::CommandCatastrophe()
{
	int x, y;
	cin >> x >> y;

	if (CheckValidTile(x, y))
	{
		if (!CheckValidLeader(map->GetTile(x, y)->GetToken()->GetType()))
		{
			if (current_player->GetNumCatastropheTokens() > 0)
			{
				current_player->SubtractCatastropheTile();
				ProcessCatastrophe(x, y);
			}

			else
			{
				cout << "exception: you have no more catastrophe tokens available" << endl;
				return false;
			}
		}

		else
		{
			cout << "exception: catastrophe tokens cannot be placed over a leader" << endl;
			return false;
		}

	}

	else
	{
		cout << "exception: invalid board space position" << endl;
		return false;
	}
}

bool GameManager::ProcessCatastrophe(int x, int y)
{
	map->GetTile(x, y)->SetToken(new Token(MyTokenType::CATASTROPHE));
	return true;
}

bool GameManager::CommandRevolt()
{
	int x, y;
	cin >> x >> y;

	if (CheckValidTile(x, y))
	{
		if (CheckValidLeader(map->GetTile(x, y)->GetToken()->GetType()))
		{
			if (map->GetAreaByTile(x, y)->RevoltAvailable())
				return ProcessRevolt(x, y);

			else
			{
				cout << "exception: revolt is not available" << endl;
				return false;
			}
		}

		else
		{
			cout << "exception: you must select a leader" << endl;
			return false;
		}
	}

	else
	{
		cout << "exception: invalid board space position" << endl;
		return false;
	}
}

bool GameManager::ProcessRevolt(int x, int y)
{
	vector <Token*> leaders_in_area = map->GetTile(x, y)->GetAreaParent()->GetLeaders();

	Token *prev_leader = nullptr, *attack_leader = nullptr, *defend_leader = nullptr;
	bool proceed = false;
	vector <MapTile*> defender_adj_temples, attacker_adj_temples;
	Player* defender = nullptr;
	int defender_temples;
	int attacker_temples;

	for (int i = 0; i < leaders_in_area.size() && !proceed; ++i)
	{
		prev_leader = leaders_in_area[i];

		for (int j = 0; j < leaders_in_area.size() && !proceed; ++j)
		{
			if (j != i)
			{
				if (prev_leader->GetColor() == leaders_in_area[j]->GetColor() && prev_leader->GetFaction() != leaders_in_area[j]->GetFaction())
				{
					if (prev_leader->GetFaction() == current_player->GetFaction())
					{
						attacker_adj_temples = map->GetAdjacentsToTile(prev_leader->GetTileParent()->position_x, prev_leader->GetTileParent()->position_y);
						defender_adj_temples = map->GetAdjacentsToTile(leaders_in_area[j]->GetTileParent()->position_x, leaders_in_area[j]->GetTileParent()->position_y);
						defender = GetPlayerByDinasty(leaders_in_area[j]->GetFaction());
						attack_leader = prev_leader;
						defend_leader = leaders_in_area[j];
					}

					else
					{
						defender_adj_temples = map->GetAdjacentsToTile(prev_leader->GetTileParent()->position_x, prev_leader->GetTileParent()->position_y);
						attacker_adj_temples = map->GetAdjacentsToTile(leaders_in_area[j]->GetTileParent()->position_x, leaders_in_area[j]->GetTileParent()->position_y);
						defender = GetPlayerByDinasty(prev_leader->GetFaction());
						attack_leader = leaders_in_area[j];
						defend_leader = prev_leader;
					}
					proceed = true;
				}
			}
		}
	}

	if (proceed)
	{
		defender_temples = defender->GetTemplesInDeck();
		attacker_temples = current_player->GetTemplesInDeck();

		for (int i = 0; i < defender_adj_temples.size(); ++i)
		{
			if (defender_adj_temples[i]->GetToken() != nullptr && defender_adj_temples[i]->GetToken()->GetType() == MyTokenType::TEMPLE)
				++defender_temples;
		}

		for (int i = 0; i < attacker_adj_temples.size(); ++i)
		{
			if (attacker_adj_temples[i]->GetToken() != nullptr && attacker_adj_temples[i]->GetToken()->GetType() == MyTokenType::TEMPLE)
				++attacker_temples;
		}
	}

	if (defender_temples >= attacker_temples)
	{
		AddPointsRevoltWon(defender, current_player, attack_leader);
	}

	else
	{
		AddPointsRevoltWon(current_player, defender, defend_leader);
	}

	return true;
}

void GameManager::AddPointsTokenPlaced(Area* area, Token* token)
{
	vector <Token*> area_leaders = area->GetLeaders();

	//if there is only a king, add points to the owner of the king
	if (area_leaders.size() == 1 && area_leaders[0]->GetType() == MyTokenType::KING)
	{
		GetPlayerByDinasty(area_leaders[0]->GetFaction())->UpdatePoints(token->GetColor());
	}

	//if there are more leaders, add points depending on the correspondant leader!
	else
	{
		for (int i = 0; i < area_leaders.size(); ++i)
		{
			if (area_leaders[i]->GetColor() == token->GetColor())
			{
				switch (token->GetType())
				{
				case MyTokenType::FARM:
					GetPlayerByDinasty(area_leaders[i]->GetFaction())->UpdatePoints(TokenColor::BLUE);
					break;

				case MyTokenType::MARKET:
					GetPlayerByDinasty(area_leaders[i]->GetFaction())->UpdatePoints(TokenColor::GREEN);
					break;

				case MyTokenType::SETTLEMENT:
					GetPlayerByDinasty(area_leaders[i]->GetFaction())->UpdatePoints(TokenColor::BLACK);
					break;

				case MyTokenType::TEMPLE:
					GetPlayerByDinasty(area_leaders[i]->GetFaction())->UpdatePoints(TokenColor::RED);
					break;
				}
			}

		}
	}



}

void GameManager::AddPointsRevoltWon(Player* winner, Player* loser, Token* loser_leader)
{
	bool no_more_temples_found = false;

	winner->UpdatePoints(TokenColor::RED);

	queue <int> positions_to_remove;
	for (int i = 0; i < loser->GetDeck().size(); ++i)
	{
		if (loser->GetDeck()[i]->GetType() == MyTokenType::TEMPLE)
			positions_to_remove.push(i);
	}

	//remove temples from loser's deck
	while (!positions_to_remove.empty())
	{
		loser->GetDeck().erase(loser->GetDeck().begin() + positions_to_remove.front());
		positions_to_remove.pop();
	}

	//remove leader from board and adjacent temples (remove tile from area)
	vector<MapTile*> adjacents_to_loser_leader = map->GetAdjacentsToTile(loser_leader->GetTileParent()->position_x, loser_leader->GetTileParent()->position_y);
	map->GetTile(loser_leader->GetTileParent()->position_x, loser_leader->GetTileParent()->position_y)->RemoveToken();
	map->PrintMap();

	/*int i = 0;
	while (!no_more_temples_found)
	{
		if (adjacents_to_loser_leader[i]->GetToken() != nullptr && adjacents_to_loser_leader[i]->GetToken()->GetType() == MyTokenType::TEMPLE)
		{
			map->GetTile(adjacents_to_loser_leader[i]->position_x, adjacents_to_loser_leader[i]->position_y)->RemoveToken();
			adjacents_to_loser_leader.erase(adjacents_to_loser_leader.begin() + i);
		}

		int temples_found = 0;
		for (int j = 0; i < adjacents_to_loser_leader.size(); ++i)
		{
			if (adjacents_to_loser_leader[i]->GetToken() != nullptr && adjacents_to_loser_leader[i]->GetToken()->GetType() == MyTokenType::TEMPLE)
				++temples_found;
		}

		if (temples_found == 0)
			no_more_temples_found = true;
	}*/
}

bool GameManager::CommandRefresh(Player* player)
{
	int player_num;
	cin >> player_num;

	if (sizeof(player_num) != sizeof(int))
	{
		cout << "Write the player number you want to refresh cards!" << endl;
		return false;
	}

	vector<string> tokens_to_refresh;
	tokens_to_refresh.reserve(MAX_DECK_SIZE);

	if (player_num == player->GetTurn())
	{
		int deck_size = int(player->GetDeck().size());
		string temp_string;

		//check that we have less than 6 cards
		if (deck_size < MAX_DECK_SIZE)
		{
			//prepare cards that we want to refill

			while (1)
			{
				if (int(tokens_to_refresh.size()) < MAX_DECK_SIZE - deck_size && cin >> temp_string)
				{
					if (temp_string == "help")
					{
						cout << "Write the cards you want to refresh: " << endl;
						cout << "1) settlement" << endl;
						cout << "2) farm" << endl;
						cout << "3) temple" << endl;
						cout << "4) market" << endl;
						cout << "As you can only have 6 cards in your deck, you can refresh as many as you want until reaching 6 cards!" << endl;
						return false;
					}

					if (CheckValidTileName(temp_string))
						tokens_to_refresh.push_back(temp_string);

					else if (temp_string == "----")
					{
						return true;
					}

					else
					{
						cout << "exception: invalid number or player tiles" << endl;
						return false;
					}
				}

				else if (int(tokens_to_refresh.size()) + deck_size == MAX_DECK_SIZE)
				{
					Token* new_token;
					for (int i = 0; i < int(tokens_to_refresh.size()); ++i)
					{
						new_token = new Token(tokens_to_refresh[i]);
						player->AcquireNewToken(new_token);

					}
					player->ShowPlayer();
					return true;
				}

				else
				{
					cout << "exception: invalid number or player tiles" << endl;
					return false;
				}
			}
		}

		else
		{
			cout << "exception: invalid number or player tiles" << endl;
			return false;
		}
	}

	else
	{
		cout << "exception: invalid number or player tiles" << endl;
		return false;
	}

}

bool GameManager::CommandMonument()
{
	string color1, color2;
	cin >> color1 >> color2;

	TokenColor c1, c2;

	c1 = TranslateStringToColor(color1);
	c2 = TranslateStringToColor(color2);

	int x, y;
	cin >> x >> y;

	if (CheckValidTile(x, y))
	{
		if (!map->GetTile(x, y)->GetAreaParent()->WarAvailable())
		{
			return Check2x2Tokens(x, y, c1, c2);
		}

		else
		{
			cout << "exception: there are unresolved conflicts" << endl;
			return false;
		}
	}

	else
	{
		cout << "exception: invalid board space position" << endl;
		return false;
	}

}

//This function checks for the adjacent token near the player input that has the same color, and then, depending
//on if the adjacent has a greater or lesser x or y coordinate, checks the immediate adjacents to the player input
bool GameManager::Check2x2Tokens(int x, int y, TokenColor c1, TokenColor c2)
{
	vector<MapTile*> adjacents_to_player_input = map->GetAdjacentsToTile(x, y);
	MapTile* temp_tile = nullptr;
	vector<MapTile*> tiles_to_remove;
	tiles_to_remove.reserve(4);

	tiles_to_remove.push_back(map->GetTile(x, y));

	for (int i = 0; i < adjacents_to_player_input.size(); ++i)
	{
		temp_tile = adjacents_to_player_input[i];

		//check vertically if adjacents_to_player_input[i] and temp_tile are adjacent to temples, vertically
		if (temp_tile->position_x >= x && temp_tile->position_y == y ||
			temp_tile->position_x < x && temp_tile->position_y == y)
		{
			if (CheckValidTile(temp_tile->position_x, temp_tile->position_y - 1) && CheckValidTile(x, y - 1))
			{
				if (map->GetTile(temp_tile->position_x, temp_tile->position_y - 1)->GetToken() != nullptr &&
					map->GetTile(x, y - 1)->GetToken() != nullptr)
				{
					//if the two inferior adjacents to adjacents_to_player_input[i] and temp_tile are the same color...
					if (map->GetTile(temp_tile->position_x, temp_tile->position_y - 1)->GetToken()->GetColor() == map->GetTile(x, y - 1)->GetToken()->GetColor())
					{
						tiles_to_remove.push_back(temp_tile);
						tiles_to_remove.push_back(map->GetTile(temp_tile->position_x, temp_tile->position_y - 1));
						tiles_to_remove.push_back(map->GetTile(x, y - 1));
						break;
					}
				}

			}

			//checking the two superior tokens
			if (CheckValidTile(temp_tile->position_x, temp_tile->position_y + 1) && CheckValidTile(x, y + 1))
			{
				if (map->GetTile(temp_tile->position_x, temp_tile->position_y + 1)->GetToken() != nullptr &&
					map->GetTile(x, y + 1)->GetToken() != nullptr)
				{
					if (map->GetTile(temp_tile->position_x, temp_tile->position_y + 1)->GetToken()->GetColor() == map->GetTile(x, y + 1)->GetToken()->GetColor())
					{
						tiles_to_remove.push_back(temp_tile);
						tiles_to_remove.push_back(map->GetTile(temp_tile->position_x, temp_tile->position_y + 1));
						tiles_to_remove.push_back(map->GetTile(x, y + 1));
						break;
					}
				}
			}
		}

		else if (temp_tile->position_x == x && temp_tile->position_y >= y ||
			temp_tile->position_x == x && temp_tile->position_y < y)
		{
			//check horizontally
			if (CheckValidTile(temp_tile->position_x - 1, temp_tile->position_y) && CheckValidTile(x - 1, y))
			{
				if (map->GetTile(temp_tile->position_x - 1, temp_tile->position_y)->GetToken() != nullptr &&
					map->GetTile(x - 1, y)->GetToken() != nullptr)
				{
					if (map->GetTile(temp_tile->position_x - 1, temp_tile->position_y)->GetToken()->GetColor() == map->GetTile(x - 1, y)->GetToken()->GetColor())
					{
						tiles_to_remove.push_back(temp_tile);
						tiles_to_remove.push_back(map->GetTile(temp_tile->position_x - 1, temp_tile->position_y));
						tiles_to_remove.push_back(map->GetTile(x - 1, y));
						break;
					}
				}

				if (map->GetTile(temp_tile->position_x + 1, temp_tile->position_y)->GetToken() != nullptr &&
					map->GetTile(x + 1, y)->GetToken() != nullptr)
				{
					if (map->GetTile(temp_tile->position_x + 1, temp_tile->position_y)->GetToken()->GetColor() == map->GetTile(x + 1, y)->GetToken()->GetColor())
					{
						tiles_to_remove.push_back(temp_tile);
						tiles_to_remove.push_back(map->GetTile(temp_tile->position_x + 1, temp_tile->position_y));
						tiles_to_remove.push_back(map->GetTile(x + 1, y));
						break;
					}
				}

			}
		}
	}

	if (tiles_to_remove.size() == 4)
	{
		if (c1 == tiles_to_remove[0]->GetToken()->GetColor() || c2 == tiles_to_remove[0]->GetToken()->GetColor())
		{
			for (int i = 0; i < tiles_to_remove.size(); ++i)
			{
				map->GetTile(tiles_to_remove[i]->position_x, tiles_to_remove[i]->position_y)->SetToken(new Token(MyTokenType::MONUMENT));
			}
		}

		else
		{
			cout << "exception: temple color isn't the same as the tiles" << endl;
			return false;
		}
			
		map->PrintMap();
		return true;
	}

	else
	{
		cout << "exception: there are not enough tiles of the same color together" << endl;
		return false;
	}

}

TokenColor GameManager::TranslateStringToColor(std::string s)
{
	if (s == "red")
		return TokenColor::RED;

	else if (s == "green")
		return TokenColor::GREEN;

	else if (s == "black")
		return TokenColor::BLACK;

	else if (s == "blue")
		return TokenColor::BLUE;
	else
		return TokenColor::WRONG;
}

void GameManager::PrintPlayersVP()
{
	for (int i = 0; i < int(players.size()); ++i)
		cout << "victory " << players[i]->GetNumTreasures() << ' ' << '(' <<
		players[i]->GetVP().bricks << ' ' << players[i]->GetVP().amulets << ' ' <<
		players[i]->GetVP().crops << ' ' << players[i]->GetVP().goods << ')' << endl;

	cout << "----" << endl;
}

void GameManager::GameLoop()
{
	bool end_game = false;
	string command;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTextAttribute(hConsole, 15);

	while (!end_game)
	{
		for (int i = 0; i < int(players.size()); ++i)
		{
			players[i]->NowPlaying();
			SetCurrentPlayer(players[i]);
			SetConsoleTextAttribute(hConsole, i + 2);
			cout << "Player " << i << "'s turn" << endl;
			while (players[i]->IsPlaying())
			{
				cout << "- - - - - - - - - - - - - - - -" << endl;


				cin >> command;

				if (ReadCommand(players[i], command, actual_player_turn_actions))
					++actual_player_turn_actions;
				ClearInput();

			}
			PrintPlayersVP();
			actual_player_turn_actions = 0;

		}
		//end_game = true;
	}
}


