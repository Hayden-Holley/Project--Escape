
#ifndef SFMLGAME_BOT_H
#define SFMLGAME_BOT_H

#include <ctime>

#include "Character.h"

class Bot : public Character
{
 public:
  void initBot();
  void updateBot(float dt, Tilemap& map);
  void findTilePos(Tilemap& map);
  void setTilePos(int x, int y);
  void findNeighbors(Tilemap& map);
  void moveToTile();
  void stopMove();
  void manageMovement(Tilemap& map);
  sf::Vector2i getRandomDir();
  void setTargetTile(int x, int y);
  void setTargetTile(sf::Vector2i vec);

 private:
  sf::Vector2i tile_pos = sf::Vector2i(0,0);

  sf::Vector2i target_tile_pos = sf::Vector2i(1, 1);
  sf::Vector2i prev_tile_pos = sf::Vector2i(1, 1);
  sf::Vector2i new_target = sf::Vector2i(1, 1);

  int neighbor_wall_count = 0;

  bool target_found = false;
};

#endif // SFMLGAME_BOT_H
