#include "Bot.h"

void Bot::initBot()
{
  std::srand(std::time(0));
}

void Bot::updateBot(float dt, Tilemap& map)
{
  findTilePos(map);
  findNeighbors(map);
  moveToTile();
  manageMovement(map);
  update(dt, map);
}

void Bot::findTilePos(Tilemap& map)
{
  float mid_x = getLeft() + getWidth() / 2;
  float mid_y = getTop() + getHeight() / 2;

  setTilePos(mid_x / map.getTileWidth(), mid_y / map.getTileHeight());
}

void Bot::setTilePos(int x, int y)
{
  tile_pos.x = x;
  tile_pos.y = y;
}

void Bot::findNeighbors(Tilemap& map)
{
  neighbor_wall_count = 0;

  // left
  if (map.getTile(tile_pos + sf::Vector2i(-1, 0))->getType() == TileType::WALL)
  {
    neighbor_wall_count += 1;
  }

  // right
  if (map.getTile(tile_pos + sf::Vector2i(1, 0))->getType() == TileType::WALL)
  {
    neighbor_wall_count += 1;
  }

  // top
  if (map.getTile(tile_pos + sf::Vector2i(0, -1))->getType() == TileType::WALL)
  {
    neighbor_wall_count += 1;
  }

  // bottom
  if (map.getTile(tile_pos + sf::Vector2i(0, 1))->getType() == TileType::WALL)
  {
    neighbor_wall_count += 1;
  }
}

void Bot::moveToTile()
{
  if ((tile_pos.x == target_tile_pos.x) &&
      (tile_pos.y == target_tile_pos.y))
  {
    stopMove();
  }
  else
  {
    if (target_tile_pos.x > tile_pos.x)
    {
      goRight();
    }

    if (target_tile_pos.x < tile_pos.x)
    {
      goLeft();
    }

    if (target_tile_pos.y > tile_pos.y)
    {
      goDown();
    }

    if (target_tile_pos.y < tile_pos.y)
    {
      goUp();
    }
  }
}

void Bot::stopMove()
{
  dir.x = 0;
  dir.y = 0;
  setCharacterState(CharacterState::IDLE);
}

void Bot::manageMovement(Tilemap& map)
{
  if (tile_pos == new_target)
  {
    target_found = false;
  }

  if (!target_found)
  {
    do
    {
      new_target = tile_pos + getRandomDir();
    }
    while ((new_target == prev_tile_pos) ||
           (new_target == sf::Vector2(28, 18)));

    if (map.getTile(new_target)->getType() == TileType::ROAD)
    {
      setTargetTile(new_target.x, new_target.y);
      prev_tile_pos = tile_pos;
      target_found = true;
    }

    if (neighbor_wall_count == 3)
    {
      prev_tile_pos = tile_pos;
    }
  }

  neighbor_wall_count = 0;
}

sf::Vector2i Bot::getRandomDir()
{
  sf::Vector2i dir;

  // random number
  int i = std::rand() % 4;

  switch (i)
  {
    case 0: // left
    {
      dir.x = -1;
      dir.y = 0;
      break;
    }

    case 1: // right
    {
      dir.x = 1;
      dir.y = 0;
      break;
    }

    case 2: // up
    {
      dir.x = 0;
      dir.y = -1;
      break;
    }

    case 3: // down
    {
      dir.x = 0;
      dir.y = 1;
      break;
    }
  }
  return dir;
}

void Bot::setTargetTile(int x, int y)
{
  target_tile_pos.x = x;
  target_tile_pos.y = y;
}

void Bot::setTargetTile(sf::Vector2i vec)
{
  target_tile_pos = vec;
}
