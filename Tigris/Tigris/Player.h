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
enum TokenType;
class Token;
class Map;
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

	void Refresh();
	std::vector<Token*> GetDeck() { return deck; };
	void ShowPlayer();
	void NowPlaying() { is_playing = true; }
	void EndTurn() { is_playing = false; }
	bool IsPlaying() { return is_playing; }
	bool GetTurn() { return turn; }
	int GetNumTreasures() { return num_treasures; }
	PlayerType GetFaction() { return faction; }
	VictoryPoints GetVP() { return points; }

	//Actions
	bool PlaceToken(Map* map, TokenType type, int x, int y);
	void AcquireNewToken(Token* new_token) { deck.push_back(new_token); }
	Token* GetLeader(TokenType type);

private:
	void DiscardUsedToken(TokenType type);
	void AddPoint(TokenColor color);

	bool is_playing = false;
	int num_treasures = 0;
	int turn;
	PlayerType faction;
	VictoryPoints points;
	std::vector<Token*> deck;
	std::vector<Token*> leaders;
};


#endif;