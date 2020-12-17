#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <vector>
#include <queue>
#include <string>


enum MyTokenType;
enum TokenColor;
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
	void AddPointsTokenPlaced(Area* area, Token* token);
	void AddPointsRevoltWon(Player* winner, Player* loser, Token* loser_leader);

	//Commands
	bool CommandTile(Player* player);
	bool CommandRefresh(Player* player);
	bool CommandLeader(Player* player);
	bool CommandTreasure(Player* player);
	bool CommandCatastrophe();
	bool CommandRevolt();
	bool CommandMonument();

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
	MyTokenType TranslateStringToToken(std::string name);

	//lmao
	bool AreThereLeadersOfTheSameColor(Token* incoming_leader, Area* area);

	void UpdateArea(Area* area, MapTile* new_tile);

	//Catastrophe functions
	bool ProcessCatastrophe(int x, int y);

	//Revolt
	bool ProcessRevolt(int x, int y);

	//Monument
	bool Check2x2Tokens(int x, int y, TokenColor c1, TokenColor c2);

	//Tools
	void SetCurrentPlayer(Player* player) { current_player = player; }
	TokenColor TranslateStringToColor(std::string s);
	Player* GetPlayerByDinasty(PlayerType type);
	void ClearInput();
	bool CheckValidLeader(MyTokenType type);

	int actual_player_turn_actions = 0;
	int player_count = 0;
	int area_id_counter = 0;
	Player* current_player;
	Map* map;
};
#endif