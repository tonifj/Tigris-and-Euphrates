#include "MapTile.h"

MapTile::MapTile(MapTile* tile) : position_x(tile->position_x), position_y(tile->position_y),
tile_type(tile->tile_type), token(tile->token), belongs_to_kingdom(tile->belongs_to_kingdom),
area_parent(tile->area_parent) {}

