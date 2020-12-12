
#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include <climits>
#include <unordered_map>

#include "Map.h"
#include "MapTile.h"

#include "Player.h"
#include "Token.h"
#include "Area.h"
#include "GameManager.h"
using namespace std;


int main() {

	GameManager* gm = new GameManager();
	/*MapTile* new_tile = new MapTile(TileType::GROUND, 6, 6);
	Token* new_token = new Token("farm");
	new_tile->SetToken(new_token);

	MapTile* new_tile2 = new MapTile(TileType::GROUND, 6, 5);
	Token* new_token2 = new Token("market");
	new_tile2->SetToken(new_token2);

	Area* area = new Area(new_tile);
	gm->GetMap()->AddArea(area);

	gm->GetMap()->GetAreaById(10)->AddTile(new_tile2);

	cout << new_tile2->GetAreaParent()->GetId();
	*/



	gm->WaitForPlayers();
	gm->GameLoop();
	return 0;
}