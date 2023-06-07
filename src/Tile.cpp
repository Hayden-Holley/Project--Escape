
// System libs
#include <iostream>

// Codebase
#include "Tile.h"

void Tile::setType(TileType type)
{
  tile_type = type;
}

TileType Tile::getType()
{
  return tile_type;
}
