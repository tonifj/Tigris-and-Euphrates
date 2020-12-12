#include "Area.h"
#include "MapTile.h"
#include "Map.h"
#include "Token.h"

MapTile* Area::GetTile(int x, int y)
{
	for (int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i]->position_x == x && tiles[i]->position_y == y)
			return tiles[i];
	}

	return nullptr;
}


bool Area::IsKingdom()
{
	int num_leaders = 0;
	for (int i = 0; i < tiles.size(); ++i)
	{
		if(CheckValidLeader(tiles[i]->GetToken()->GetType()))
			++num_leaders;
	}

	return num_leaders > 0;
}

bool Area::CheckValidLeader(MyTokenType type)
{
	return type == MyTokenType::KING || type == MyTokenType::MERCHANT || type == MyTokenType::PRIEST || type == MyTokenType::FARMER;
}

int Area::GetNumOfLeaders()
{
	int num_leaders = 0;
	for (int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i]->GetToken() != nullptr && CheckValidLeader(tiles[i]->GetToken()->GetType()))
			++num_leaders;			
	}

	return num_leaders;
}

void Area::RemoveTile(int x, int y)
{
	int position_to_remove_tile = 0;
	bool found = false;
	for (int i = 0; i < tiles.size() && !found; ++i)
	{
		if (tiles[i]->position_x == x && tiles[i]->position_y)
		{
			position_to_remove_tile = i;
			found = true;
		}
	}

	tiles.erase(tiles.begin() + position_to_remove_tile);
	tiles.shrink_to_fit();

}

void Area::AddTile(MapTile* tile) 
{ 
	tile->SetAreaParent(this); 
	tiles.push_back(tile); 
}