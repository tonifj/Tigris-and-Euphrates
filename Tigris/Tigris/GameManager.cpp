#include <iostream>
#include <climits>

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
				cout << "The game starts!" << endl;
			}
		}

		//Detect tiles that the player wants
		else
		{
			if (token_name == "settlement")
			{
				temp_player_tokens.push_back(new Token(TokenType::SETTLEMENT));
				++token_counter;
			}

			else if (token_name == "temple")
			{
				temp_player_tokens.push_back(new Token(TokenType::TEMPLE));
				++token_counter;
			}

			else if (token_name == "farm")
			{
				temp_player_tokens.push_back(new Token(TokenType::FARM));
				++token_counter;
			}

			else if (token_name == "market")
			{
				temp_player_tokens.push_back(new Token(TokenType::MARKET));
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
			cout << "leader: to place a leader" << endl;
			cout << "treasure: to pick a sure" << endl;
			cout << "catastrophe: to place a catastrophe tile" << endl;
			cout << "revolt: to begin a revolt in a kingdom" << endl;
			cout << "war: to begin a war between two kingdoms" << endl;
			cout << "monument: to build a monument" << endl;
			return false;
		}
	}

	if (command == "refresh")
	{
		CommandRefresh(player);
		player->EndTurn();
		actual_player_turn_actions = 0;
		return true;
	}

	else if (command == "help")
	{
		cout << "As you don't have more turns, you can refresh your deck or pass turn by ";
		cout << "typing ´refresh` or `----`" << endl;
	}


	else
	{
		if (command == "----")
		{
			player->EndTurn();
			actual_player_turn_actions = 0;
		}

		else
			cout << "exception: too many turn actions" << endl;
	}

	return false;
}

bool GameManager::CheckTokenAvailability(Player* player, TokenType type)
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
		return ProcessTile(player, TokenType::SETTLEMENT);

	else if (type == "farm")
		return ProcessTile(player, TokenType::FARM);

	else if (type == "market")
		return ProcessTile(player, TokenType::MARKET);

	else if (type == "temple")
		return ProcessTile(player, TokenType::TEMPLE);

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

vector <Token*> GameManager::GetAreaLeaders(int area_id)
{
	return map->GetAreas()[area_id]->GetLeaders();
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

TokenType GameManager::TranslateStringToToken(string name)
{
	if (name == "king")
		return TokenType::KING;
	else if (name == "merchant")
		return TokenType::MERCHANT;
	else if (name == "farmer")
		return TokenType::FARMER;
	else if (name == "priest")
		return TokenType::PRIEST;
	else if (name == "settlement")
		return TokenType::SETTLEMENT;
	else if (name == "farm")
		return TokenType::FARM;
	else if (name == "temple")
		return TokenType::TEMPLE;
	else if (name == "market")
		return TokenType::MARKET;
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
			adjacent_tokens[i]->GetToken()->GetType() == TokenType::TEMPLE)
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
				//if the leader is placed somewhere, remove it
				if (leader->GetTileParent() != nullptr)
					RemoveLeader(leader);

				map->GetTile(x, y)->SetAreaParent(old_region_token->GetTileParent()->GetAreaParent()); // add parent to the tile
				MoveLeader(leader, nullptr, map->GetTile(x, y));
				old_region_token->GetTileParent()->GetAreaParent()->AddLeader(leader);

			}

			else
			{
				if (leader->GetTileParent() != nullptr)
					RemoveLeader(leader);

				//add to the kingdom the new leader
				map->GetTile(x, y)->SetAreaParent(old_kingdom_token->GetTileParent()->GetAreaParent()); // add parent to the tile where we're placing the leader
				MoveLeader(leader, leader->GetTileParent(), map->GetTile(x, y));
				old_kingdom_token->GetTileParent()->GetAreaParent()->AddLeader(leader); // add leader to the area

				if (AreThereLeadersOfTheSameColor(leader, old_kingdom_token));
				activate_revolt = true;

				//This means  that a leader of the same was sitting here before the new one
				if (activate_revolt)
				{
					if (leader->GetFaction() != old_region_token->GetFaction())
					{
						old_kingdom_token->GetTileParent()->GetAreaParent()->SetRevoltAvailable();
						cout << "This will be a blast!" << endl;
					}

				}
			}

			//place leader
			return current_player->PlaceToken(map, leader->GetType(), x, y);
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

void GameManager::MoveLeader(Token* leader, MapTile* old_tile, MapTile* new_tile)
{
	int x = new_tile->position_x;
	int y = new_tile->position_y;
	map->GetTile(x, y)->SetToken(leader);
	leader->SetParent(map->GetTile(x, y));
	leader->GetTileParent()->SetToken(leader);
	map->GetTile(x, y)->SetToken(leader);
	map->UpdateMap(leader, new_tile->position_x, new_tile->position_y);
}

void GameManager::RemoveLeader(Token* leader)
{
	leader->GetTileParent()->RemoveToken();
}

bool GameManager::AreThereLeadersOfTheSameColor(Token* incoming_leader, Token* old_leader)
{
	Area* temp_area = old_leader->GetTileParent()->GetAreaParent();
	for (int i = 0; i < int(temp_area->GetLeaders().size()); ++i)
	{
		if (incoming_leader->GetType() == temp_area->GetLeaders()[i]->GetType() && incoming_leader->GetFaction() != temp_area->GetLeaders()[i]->GetFaction())
			return true;
	}
	return false;
}

bool GameManager::IsTokenPartOfKingdom(Token* token)
{
	return 	token->GetTileParent()->GetAreaParent()->IsKingdom();
}

bool GameManager::ProcessTile(Player* player, TokenType type)
{
	int x, y;
	if (CheckTokenAvailability(player, type))
	{
		cin >> x >> y;

		if (CheckValidTile(x, y))
		{
			if (CheckTileAvailability(x, y))
			{
				return player->PlaceToken(map, type, x, y);
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

bool GameManager::CommandRefresh(Player* player)
{
	int player_num;
	cin >> player_num;

	if (sizeof(player_num) != sizeof(int))
	{
		cout << "Write the player numebr you want to refresh cards!" << endl;
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
					for (int i = 0; i < int(tokens_to_refresh.size()); ++i)
						player->AcquireNewToken(new Token(tokens_to_refresh[i]));

					//cout << "Tokens refreshed!" << endl;
					//player->ShowPlayer();

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

	while (!end_game)
	{
		for (int i = 0; i < int(players.size()); ++i)
		{
			players[i]->NowPlaying();
			SetCurrentPlayer(players[i]);
			//cout << "Player " << i << endl;
			while (players[i]->IsPlaying())
			{
				cin >> command;

				if (ReadCommand(players[i], command, actual_player_turn_actions))
					++actual_player_turn_actions;
				ClearInput();

			}
			PrintPlayersVP();
			actual_player_turn_actions = 0;

		}
		end_game = true;
	}
}
