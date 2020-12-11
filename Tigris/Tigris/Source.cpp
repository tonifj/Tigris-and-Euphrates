
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

	cout << "Welcome to Tigris and Euphrates - Console Version" << endl;
	cout << "The objective is to survive as a civilization against other players! Good luck!" << endl;
	cout << "Type 'help' in any moment and I'll help you!" << endl;

	GameManager* gm = new GameManager();

	gm->WaitForPlayers();
	gm->GameLoop();
	return 0;
}