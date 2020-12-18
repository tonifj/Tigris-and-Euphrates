#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>


enum PlayerType
{
	BULL,
	LION,
	POT,
	BOW
};

enum TokenColor;
enum MyTokenType;
class Token;
class Map;
class Area;
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
	Player(PlayerType faction, std::vector<Token*>& tokens, int new_turn);

	std::vector<Token*> GetDeck() { return deck; };
	void ShowPlayer();
	void NowPlaying() { is_playing = true; }
	void EndTurn() { is_playing = false; }
	bool IsPlaying() { return is_playing; }
	bool GetTurn() { return turn; }
	int GetNumTreasures() { return num_treasures; }
	int GetNumCatastropheTokens() { return num_catastrophe_tiles; }
	PlayerType GetFaction() { return faction; }
	VictoryPoints GetVP() { return points; }
	void UpdatePoints(TokenColor color);
	int GetTemplesInDeck();
	int GetNumOfTokensByColor(TokenColor);

	//Actions
	bool PlaceToken(Map* map, MyTokenType type, int x, int y);
	bool MoveLeader(Map* map, Area* area, MyTokenType type, int x, int y);
	void AcquireNewToken(Token* new_token) { deck.push_back(new_token); }
	Token* GetLeader(MyTokenType type);

	void AddTreasure() { ++num_treasures; };
	void SubtractCatastropheTile() { --num_catastrophe_tiles; }
	void DiscardUsedToken(MyTokenType type);


private:

	bool is_playing = false;
	int num_catastrophe_tiles = 2;
	int num_treasures = 0;
	int turn;
	PlayerType faction;
	VictoryPoints points;
	std::vector<Token*> deck;
	std::vector<Token*> leaders;

};


#endif;