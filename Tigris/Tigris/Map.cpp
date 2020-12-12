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
			Area* new_area = new Area(area_counter_id);

			//Set tile properties
			tiles[i % 16][j] = new MapTile(TileType::GROUND, i % 16, j);
			tiles[i % 16][j]->SetToken(new Token(MyTokenType::TEMPLE));
			tiles[i % 16][j]->GetToken()->SetParent(tiles[i % 16][j]);
			tiles[i % 16][j]->GetToken()->SetTreasure();
			tiles[i % 16][j]->SetAreaParent(new_area);

			MapTile* temp_tile = new MapTile(tiles[i % 16][j]);
			new_area->AddTile(temp_tile);
			new_area->GetTile(i % 16, j)->SetAreaParent(new_area);

			AddArea(new_area);
		}
	}
}




void Map::AddArea(Area* area)
{
	areas[area_counter_id] = area;
	++area_counter_id;
}

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
					cout << 'f';
			}
			else
			{
				temp_tile = tiles[i][j];

				if (temp_tile->GetToken() != nullptr)
				{
					if (temp_tile->GetToken()->GetType() == MyTokenType::TEMPLE)
					{
						cout << 't';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == MyTokenType::SETTLEMENT)
					{
						cout << 's';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == MyTokenType::MARKET)
					{
						cout << 'm';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == MyTokenType::KING)
					{
						cout << 'K';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == MyTokenType::FARMER)
					{
						cout << 'F';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == MyTokenType::MERCHANT)
					{
						cout << 'M';
						continue;
					}

					else if (temp_tile->GetToken()->GetType() == MyTokenType::PRIEST)
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
	cout << endl << "=================" << endl;

}

//This call creates a token with its MapTile parent and also sets the token for the tile
void Map::UpdateMap(Token* token, int x, int y)
{
	if (token == nullptr)
		tiles[x][y]->RemoveToken();
	else
	{
		token->SetParent(tiles[x][y]);
		tiles[x][y]->SetToken(token);

		//look if we're puting the token in a tile that will belong to an area
		vector <MapTile*> adjacents = GetAdjacentsToTile(x, y);
		bool tile_with_area_parent = false;

		for (int i = 0; i < adjacents.size() && !tile_with_area_parent; ++i)
		{
			if (adjacents[i]->GetAreaParent() != nullptr)
			{
				tile_with_area_parent = true;
				adjacents[i]->GetAreaParent()->AddTile(tiles[x][y]);
			}
		}
	}
}

bool Map::IsValidTile(int x, int y)
{
	return x < 16 && x >= 0 && y < 11 && y >= 0;
}


Area* Map::GetAreaByTile(int x, int y)
{
	return areas[GetTile(x, y)->GetAreaParent()->GetId()];
}