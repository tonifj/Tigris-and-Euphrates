#include <iostream>
#include <climits>
#include <windows.h>

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

			else cout << "enter a valid token!" << endl;

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
			//pick treasure
		}

		else if (command == "catastrophe")
		{
			//do catastrophe stuff
		}

		else if (command == "revolt")
		{
			//become marx
		}

		else if (command == "war")
		{
			//become USA
		}

		else if (command == "monument")
		{
			//build me a monument pls
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
	bool only_one_kingdom = true;

	Area* prev_kingdom = nullptr;
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
				}

				if (num_adjacent_tiles_belonging_to_kingdom == 0)
				{
					map->GetTile(x, y)->SetToken(new Token(type));
					map->AddArea(new Area(map->GetTile(x, y)));
					return player->PlaceToken(map, type, x, y);
				}

				else
				{
					if (only_one_kingdom)
					{
						map->GetTile(x, y)->SetToken(new Token(type));
						prev_kingdom->AddTile(map->GetTile(x, y));

						//add points
						AddPointsToPlayer(prev_kingdom, map->GetTile(x, y)->GetToken());

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

	cin >> x >> y;

	if (CheckValidTile(x, y))
	{
		if (map->GetTile(x, y)->GetToken()->GetType() == MyTokenType::TEMPLE)
		{
			if (map->GetTile(x, y)->GetToken()->HasTreasure())
			{
				map->GetTile(x, y)->GetToken()->RemoveTreasure();
				player->AddTreasure();
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

void GameManager::AddPointsToPlayer(Area* area, Token* token)
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
