//
// Created by o2-chalk on 10/03/2023.
//

#ifndef SFMLGAME_TILE_H
#define SFMLGAME_TILE_H

#include "GameObject.h"

enum class TileType {WALL, ROAD};

class Tile : public GameObject
{
 public:
  void setType(TileType type);
  TileType getType();

  int x;
  int y;
  int id;

  // tile type
  TileType tile_type = TileType::WALL;
};

#endif // SFMLGAME_TILE_H
