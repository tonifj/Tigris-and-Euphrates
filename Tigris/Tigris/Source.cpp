
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <climits>
#include <unordered_map>
using namespace std;

//Ex4

#define MAX_DECK_SIZE 6

enum TileType
{
	GROUND = 0,
	RIVER
};

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

enum PlayerType
{
	BULL,
	LION,
	POT,
	BOW
};
class Area;
class MapTile;

class Token
{
public:
	Token(TokenType type);
	Token(TokenType type, PlayerType faction);
	Token(TokenType type, int x, int y);
	Token(string type);
	TokenType GetType();
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

Token::Token(string type)
{
	//adjacents = vector <Token*>(4, nullptr); // same here ^^

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

TokenType Token::GetType()
{
	return token_type;
}

class Leader : public Token
{
public:
	PlayerType GetFaction() { return faction; }
};


class MapTile
{
public:
	MapTile() {}
	MapTile(MapTile* tile);
	MapTile(TileType type) : tile_type(type) {}
	MapTile(TileType type, int x, int y) : tile_type(type), position_x(x), position_y(y) {}
	const bool IsEmpty() const { return token == nullptr; };
	const bool IsRiver() const { return tile_type == TileType::RIVER; };
	void SetToken(Token* new_token) { token = new_token; };
	Token* GetToken() const { return token; }
	void RemoveToken() { token = nullptr; }
	const TokenType GetTokenType() const { return token->GetType(); }
	void SetAreaParent(Area* area) { area_parent = area; }
	Area* GetAreaParent() { return area_parent; }

	const bool BelongsToKingdom() { return belongs_to_kingdom; }
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

MapTile::MapTile(MapTile* tile) : position_x(tile->position_x), position_y(tile->position_y),
tile_type(tile->tile_type), token(tile->token), belongs_to_kingdom(tile->belongs_to_kingdom),
area_parent(tile->area_parent) {}

//Set here in order to detect correctly the MapTile class
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

class Map
{
	MapTile tiles[16][11];

public:
	Map();
	void PrintMap() const;
	void UpdateMap(Token* token, int x, int y);
	MapTile GetTile(int x, int y) { return tiles[x][y]; }
	bool IsValidTile(int x, int y);
	vector <MapTile*> GetAdjacentsToTile(int x, int y);
	unordered_map<int, Area*> GetAreas() { return areas; }
	void AddArea(Area* area);
	void AddArea(int id, Area* area) { areas[id] = area; }

	Area* GetAreaByTile(int x, int y);
	Area* GetAreaById(int id) { return areas[id]; }


private:
	unordered_map<int, Area*> areas;
};

class Area
{
public:
	Area() { id = -1; }
	Area(int id) : id(id) {}
	Area(MapTile* initial_tile) { id = -1; tiles.push_back(initial_tile); }
	Area(MapTile* initial_tile, int new_id) { tiles.push_back(initial_tile); id = new_id; }

	void AddTile(MapTile* tile) { tiles.push_back(tile); }
	void AddLeader(Token* new_leader) { leaders.push_back(new_leader); }

	MapTile* GetTile(int x, int y);
	int GetId() { return id; }

	bool IsSameAs(Area* area) { return id == area->id; }
	bool IsKingdom() { return leaders.size() != 0; }

	vector<Token*> GetLeaders() { return leaders; }
	vector<MapTile*> GetTiles() { return tiles; }
	void SetWarAvailable() { war_availabe = true; }
	void SetWarNotAvailable() { war_availabe = false; }
	void SetRevoltAvailable() { revolt_available = true; }
	void SetRevoltNotAvailable() { revolt_available = false; }


private:
	int id;
	vector<MapTile*> tiles;
	vector<Token*> leaders;
	bool war_availabe = false;
	bool revolt_available = false;
};


MapTile* Area::GetTile(int x, int y)
{
	for (int i = 0; i < int(tiles.size()); ++i)
	{
		if (tiles[i]->position_x == x && tiles[i]->position_y == y)
			return tiles[i];
	}

	return nullptr;
}

Area* Map::GetAreaByTile(int x, int y)
{
	Area* area = areas[GetTile(x, y).GetAreaParent()->GetId()];
	return area;
}
void Map::AddArea(Area* area) { areas[area->GetId()] = area; }

vector <MapTile*> Map::GetAdjacentsToTile(int x, int y)
{

	vector<MapTile*> adjacents;
	adjacents.reserve(4);

	if (IsValidTile(x, y + 1)) adjacents.push_back(&tiles[x][y + 1]);
	if (IsValidTile(x, y - 1)) adjacents.push_back(&tiles[x][y - 1]);

	if (IsValidTile(x + 1, y)) adjacents.push_back(&tiles[x + 1][y]);
	if (IsValidTile(x - 1, y)) adjacents.push_back(&tiles[x - 1][y]);


	return adjacents;
}

Map::Map()
{
	char map[177] =
	{
		"----*****-T-*---"
		"-T--*-------*--T"
		"---**T------**--"
		"****---------***"
		"-------------T**"
		"--------------*-"
		"****----T---***-"
		"-T-****-----*---"
		"------*******-T-"
		"-----T----------"
		"----------T-----"
	};

	int j = 10;
	for (int i = 0; i < 177; ++i)
	{
		if (i % 16 == 0 && i != 0 && j > 0)
			--j;

		if (map[i] == '-')
			tiles[i % 16][j] = MapTile(TileType::GROUND, i % 16, j);

		else if (map[i] == '*')
			tiles[i % 16][j] = MapTile(TileType::RIVER, i % 16, j);

		else if (map[i] == 'T')
		{
			Area* new_area = new Area(areas.size());

			//Set tile properties
			tiles[i % 16][j] = MapTile(TileType::GROUND, i % 16, j);
			tiles[i % 16][j].SetToken(new Token(TokenType::TEMPLE));
			tiles[i % 16][j].GetToken()->SetParent(&tiles[i % 16][j]);
			tiles[i % 16][j].SetAreaParent(new_area);
			MapTile* temp_tile = new MapTile(tiles[i % 16][j]);
			new_area->AddTile(temp_tile);


			new_area->GetTile(i % 16, j)->SetAreaParent(new_area);

			//Add tile to area

			//store area
			AddArea(new_area);

		}
	}
	//PrintMap();
}

void Map::PrintMap() const
{
	MapTile temp_tile;

	for (int j = 10; j >= 0; --j)
	{
		for (int i = 0; i < 16; ++i)
		{
			if (tiles[i][j].IsRiver())
			{
				if (tiles[i][j].IsEmpty())
					cout << '*';
				else
					cout << 'F';
			}
			else
			{
				temp_tile = tiles[i][j];

				if (temp_tile.GetToken() != nullptr)
				{
					if (temp_tile.GetToken()->GetType() == TokenType::TEMPLE)
					{
						cout << 't';
						continue;
					}

					else if (temp_tile.GetToken()->GetType() == TokenType::SETTLEMENT)
					{
						cout << 's';
						continue;
					}

					else if (temp_tile.GetToken()->GetType() == TokenType::MARKET)
					{
						cout << 'm';
						continue;
					}

					else if (temp_tile.GetToken()->GetType() == TokenType::KING)
					{
						cout << 'K';
						continue;
					}

					else if (temp_tile.GetToken()->GetType() == TokenType::FARMER)
					{
						cout << 'F';
						continue;
					}

					else if (temp_tile.GetToken()->GetType() == TokenType::MERCHANT)
					{
						cout << 'M';
						continue;
					}

					else if (temp_tile.GetToken()->GetType() == TokenType::PRIEST)
					{
						cout << 'P';
						continue;
					}
				}
				else
					cout << '-';
			}

		}

		cout << endl;
	}
}

//This call creates a token with its MapTile parent and also sets the token for the tile
void Map::UpdateMap(Token* token, int x, int y)
{
	if (token == nullptr)
		tiles[x][y].RemoveToken();
	else
		tiles[x][y].SetToken(token);
}

bool Map::IsValidTile(int x, int y)
{
	return x < 16 && x >= 0 && y < 11 && y >= 0;
}

class Player
{
	struct VictoryPoints
	{
		int amulets; //red points
		int bricks;  //black points
		int crops;   //blue points
		int goods;   //green points
	};

public:
	Player(PlayerType faction, vector<Token*>& tokens, int new_turn);

	//int RemainingTurns() { return remaining_turns; };

	//void RefillTurns() { remaining_turns = 2; }
	//PlayerType GetType();
	//void PrintPoints();
	void Refresh();
	vector<Token*> GetDeck() { return deck; };
	void ShowPlayer();
	void NowPlaying() { is_playing = true; }
	void EndTurn() { is_playing = false; }
	bool IsPlaying() { return is_playing; }
	bool GetTurn() { return turn; }
	int GetNumTreasures() { return num_treasures; }
	PlayerType GetFaction() { return faction; }
	VictoryPoints GetVP() { return points; }
	//Actions
	bool PlaceToken(Map* map, TokenType type, int tile_x, int tile_y);
	void AcquireNewToken(Token* new_token) { deck.push_back(new_token); }
	//Token* GetLeader(string leader);
	Token* GetLeader(TokenType type);

private:
	void DiscardUsedToken(TokenType type);
	void AddPoint(TokenColor color);

	bool is_playing = false;
	int num_treasures = 0;
	int turn;
	PlayerType faction;
	VictoryPoints points;
	vector<Token*> deck;
	vector<Token*> leaders;
};

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
	if (map->GetTile(x, y).IsRiver())
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

class GameManager
{
public:
	GameManager();
	vector<Player*> players;
	vector<PlayerType> available_types;
	void WaitForPlayers();
	void GameLoop();
	Map* GetMap() { return map; }


private:
	void CreatePlayer(vector<Token*>&);
	bool ReadCommand(Player* player, string& command, int turn_actions);
	bool CheckTokenAvailability(Player* player, TokenType type);
	int ActualPlayerActions() { return actual_player_turn_actions; }

	//Commands
	bool CommandTile(Player* player);
	bool CommandRefresh(Player* player);
	bool CommandLeader(Player* player);

	void PrintPlayersVP();

	//Tile functions
	bool ProcessTile(Player* player, TokenType type);
	bool CheckValidTile(int x, int y);
	bool CheckTileAvailability(int x, int y);
	bool CheckValidTileName(string name) { return name == "settlement" || name == "farm" || name == "market" || name == "temple"; };
	bool CheckValidLeader(string name) { return name == "merchant" || name == "priest" || name == "king" || name == "farmer"; }

	//Leader functions
	bool ProcessLeader(Token* token, int x, int y);
	bool IsTokenPartOfKingdom(Token* token);
	vector <Token*> GetAreaLeaders(int area_id);
	TokenType TranslateStringToToken(string name);
	void MoveLeader(Token* leader, MapTile* old_tile, MapTile* new_tile);
	void RemoveLeader(Token* leader);

	//lmao
	bool AreThereLeadersOfTheSameColor(Token* incoming_leader, Token* old_leader);

	void UpdateArea(Area* area, MapTile* new_tile) { area->AddTile(new_tile); }


	//Tools
	Player* current_player;
	void SetCurrentPlayer(Player* player) { current_player = player; }
	void ClearInput() { cin.ignore(INT_MAX, '\n');  cin.clear(); }
	int actual_player_turn_actions = 0;
	int player_count = 0;
	int area_id_counter = 0;

	Map* map;
};

GameManager::GameManager()
{
	map = new Map();

	available_types.reserve(4);

	available_types.push_back(PlayerType::BOW);
	available_types.push_back(PlayerType::BULL);
	available_types.push_back(PlayerType::POT);
	available_types.push_back(PlayerType::LION);
}

void GameManager::WaitForPlayers()
{
	bool waiting_more_players = true;
	int token_counter = 0;

	string token_name;

	vector<Token*> temp_player_tokens;
	temp_player_tokens.reserve(MAX_DECK_SIZE);

	while (waiting_more_players && cin >> token_name)
	{
		if (token_counter % MAX_DECK_SIZE == 0 && token_counter != 0)
		{
			CreatePlayer(temp_player_tokens);
			//cout << "player created!" << endl;
			++player_count;
			token_counter = 0;
			temp_player_tokens.clear();
			temp_player_tokens.shrink_to_fit();
			actual_player_turn_actions = 0;
		}

		if (token_name == "----" && token_counter == 0)
		{
			if (player_count < 2)
				cout << "you need one more player :'D" << endl;

			else
			{
				waiting_more_players = false;
				//cout << "no more players!" << endl;
			}
		}

		else
		{
			if (token_name == "settlement")
			{
				temp_player_tokens.push_back(new Token(TokenType::SETTLEMENT));
				++token_counter;
				continue;
			}

			if (token_name == "temple")
			{
				temp_player_tokens.push_back(new Token(TokenType::TEMPLE));
				++token_counter;
				continue;
			}

			if (token_name == "farm")
			{
				temp_player_tokens.push_back(new Token(TokenType::FARM));
				++token_counter;
				continue;
			}

			if (token_name == "market")
			{
				temp_player_tokens.push_back(new Token(TokenType::MARKET));
				++token_counter;
				continue;
			}

			cout << "enter a valid token!" << endl;

		}

	}
}

void GameManager::CreatePlayer(vector<Token*>& tokens)
{
	players.push_back(new Player(available_types.back(), tokens, player_count));
	available_types.pop_back();
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
	}

	if (command == "refresh")
	{
		CommandRefresh(player);
		player->EndTurn();
		actual_player_turn_actions = 0;
		return true;
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
	return map->GetTile(x, y).IsEmpty();
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
			if (!map->GetTile(x, y).IsRiver())
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

				map->GetTile(x, y).SetAreaParent(old_region_token->GetTileParent()->GetAreaParent()); // add parent to the tile
				MoveLeader(leader, nullptr, &map->GetTile(x, y));
				old_region_token->GetTileParent()->GetAreaParent()->AddLeader(leader);

			}

			else
			{
				if (leader->GetTileParent() != nullptr)
					RemoveLeader(leader);

				//add to the kingdom the new leader
				map->GetTile(x, y).SetAreaParent(old_kingdom_token->GetTileParent()->GetAreaParent()); // add parent to the tile where we're placing the leader
				MoveLeader(leader, leader->GetTileParent(), &map->GetTile(x, y));
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
	map->GetTile(x, y).SetToken(leader);
	leader->SetParent(&map->GetTile(x, y));
	leader->GetTileParent()->SetToken(leader);
	map->GetTile(x, y).SetToken(leader);
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

int main() {
	GameManager* gm = new GameManager();
	gm->WaitForPlayers();
	gm->GameLoop();


	/*vector <MapTile*> asd = gm->GetMap()->GetAdjacentsToTile(5, 7);

	for (int i = 0; i < asd.size(); ++i)
	{
		if (asd[i]->GetToken() != nullptr)
			cout << asd[i]->GetToken()->GetType() << endl;
		else
			cout << "empty" << endl;
	}*/

	//cout << gm->GetMap()->GetTile(5, 8).GetTokenType() << endl;

	return 0;
}