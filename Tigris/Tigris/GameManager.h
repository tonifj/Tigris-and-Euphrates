#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <vector>
#include <queue>
#include <string>


enum MyTokenType;
enum PlayerType;
class Player;
class Token;
class Map;
class MapTile;
class Area;
class GameManager
{
public:
	GameManager();
	std::vector<Player*> players;
	std::queue<PlayerType> available_types;
	void WaitForPlayers();
	void GameLoop();
	Map* GetMap() { return map; }


private:
	void CreatePlayer(std::vector<Token*>&);
	bool ReadCommand(Player* player, std::string& command, int turn_actions);
	bool CheckTokenAvailability(Player* player, MyTokenType type);
	int ActualPlayerActions() { return actual_player_turn_actions; }
	void AddPointsToPlayer(Area* area, Token* token);

	//Commands
	bool CommandTile(Player* player);
	bool CommandRefresh(Player* player);
	bool CommandLeader(Player* player);

	void PrintPlayersVP();

	//Tile functions
	bool ProcessTile(Player* player, MyTokenType type);
	bool CheckValidTile(int x, int y);
	bool CheckTileAvailability(int x, int y);
	bool CheckValidTileName(std::string name) { return name == "settlement" || name == "farm" || name == "market" || name == "temple"; };
	bool CheckValidLeader(std::string name) { return name == "merchant" || name == "priest" || name == "king" || name == "farmer"; }

	//Leader functions
	bool ProcessLeader(Token* token, int x, int y);
	bool IsTokenPartOfKingdom(Token* token);
	std::vector <Token*> GetAreaLeaders(int area_id);
	MyTokenType TranslateStringToToken(std::string name);

	//lmao
	bool AreThereLeadersOfTheSameColor(Token* incoming_leader, Area* area);

	void UpdateArea(Area* area, MapTile* new_tile);


	//Tools
	Player* current_player;
	void SetCurrentPlayer(Player* player) { current_player = player; }
	Player* GetPlayerByDinasty(PlayerType type);
	void ClearInput();
	int actual_player_turn_actions = 0;
	int player_count = 0;
	int area_id_counter = 0;

	Map* map;
};
#endif