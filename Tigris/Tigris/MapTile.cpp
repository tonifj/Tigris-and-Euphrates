#include "MapTile.h"
#include "Token.h"
#include "Area.h"

MapTile::MapTile(MapTile* tile) : position_x(tile->position_x), position_y(tile->position_y),
tile_type(tile->tile_type), token(tile->token), belongs_to_kingdom(tile->belongs_to_kingdom),
area_parent(tile->area_parent) {}

void MapTile::SetToken(Token* new_token)
{
	new_token->SetParent(this);
	token = new_token;
}

const bool MapTile::BelongsToKingdom() {

	if (area_parent != nullptr)
	{
		if (area_parent->GetNumOfLeaders() >= 1)
			belongs_to_kingdom = true;
	}
	
	else
	{
		belongs_to_kingdom = false;
	}

	return belongs_to_kingdom;

}

