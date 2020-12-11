#include <iostream>

#include "Map.h"
#include "Area.h"
#include "MapTile.h"
#include "Token.h"

using namespace std;

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
			tiles[i % 16][j] = new MapTile(TileType::GROUND, i % 16, j);

		else if (map[i] == '*')
			tiles[i % 16][j] = new MapTile(TileType::RIVER, i % 16, j);

		else if (map[i] == 'T')
		{
			Area* new_area = new Area(areas.size());

			//Set tile properties
			tiles[i % 16][j] = new MapTile(TileType::GROUND, i % 16, j);
			tiles[i % 16][j]->SetToken(new Token(TokenType::TEMPLE));
			tiles[i % 16][j]->GetToken()->SetParent(tiles[i % 16][j]);
			tiles[i % 16][j]->SetAreaParent(new_area);
			MapTile* temp_tile = new MapTile(tiles[i % 16][j]);
			new_area->AddTile(temp_tile);


			new_area->GetTile(i % 16, j)->SetAreaParent(new_area);

			//Add tile to area

			//store area
			AddArea(new_area);

		}
	}
}




void Map::AddArea(Area* area) { areas[area->GetId()] = area; }

vector <MapTile*> Map::GetAdjacentsToTile(int x, int y)
{

	vector<MapTile*> adjacents;
	adjacents.reserve(4);

	if (IsValidTile(x, y + 1)) adjacents.push_back(tiles[x][y + 1]);
	if (IsValidTile(x, y - 1)) adjacents.push_back(tiles[x][y - 1]);

	if (IsValidTile(x + 1, y)) adjacents.push_back(tiles[x + 1][y]);
	if (IsValidTile(x - 1, y)) adjacents.push_back(tiles[x - 1][y]);


	return adjacents;
}

void Map::PrintMap() const
{
	cout << "====== MAP ======" << endl << endl;

	MapTile* temp_tile;

	for (int j = 10; j >= 0; --j)
	{
		for (int i = 0; i < 16; ++i)
		{
			if (tiles[i][j]->IsRiver())
			{
				if (tiles[i][j]->IsEmpty())
					cout << '*';
				else
					cout << 'F';
			}
			else
			{
				temp_tile = tiles[i][j];

				if (temp_tile->GetToken() != nullptr)
				{
					if (temp_tile->GetToken()->GetType() == TokenType::TEMPLE)
					{
						cout << 't';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == TokenType::SETTLEMENT)
					{
						cout << 's';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == TokenType::MARKET)
					{
						cout << 'm';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == TokenType::KING)
					{
						cout << 'K';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == TokenType::FARMER)
					{
						cout << 'F';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == TokenType::MERCHANT)
					{
						cout << 'M';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == TokenType::PRIEST)
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
	cout << endl << "<===============>" << endl;

}

//This call creates a token with its MapTile parent and also sets the token for the tile
void Map::UpdateMap(Token* token, int x, int y)
{
	if (token == nullptr)
		tiles[x][y]->RemoveToken();
	else
		tiles[x][y]->SetToken(token);
}

bool Map::IsValidTile(int x, int y)
{
	return x < 16 && x >= 0 && y < 11 && y >= 0;
}