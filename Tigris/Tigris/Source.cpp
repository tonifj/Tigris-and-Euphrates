
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
	gm->WaitForPlayers();
	gm->GameLoop();
	return 0;
}