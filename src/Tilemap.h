
#ifndef SFMLGAME_TILEMAP_H
#define SFMLGAME_TILEMAP_H

// Game Engine
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

// Codebase
#include "Tile.h"

class Tilemap
{
 public:
  // function 1
  // initializes the map. Takes the used window and tilemap file as parameters
  void initMap(sf::RenderWindow& window, std::string filename);

  // function 2
  // takes the used window as parameter
  void render(sf::RenderWindow& window);

  // function 3
  // returns a pointer to the instance of the tile class. Takes the tile position in the tilemap
  // e.g. getTile(3, 5) returns the tile on column 3 row 5
  std::unique_ptr<Tile>& getTile(int x, int y);
  std::unique_ptr<Tile>& getTile(sf::Vector2i vec);

  // function 4
  // returns how many tiles are in the column / row
  int tilemapWidth();
  int tilemapHeight();

  float getTileWidth();
  float getTileHeight();


 private:
  sf::IntRect findTextureRect(int id);

  sf::Texture tile_sheet;
  tmx::Map map;

  static const int TILEMAP_WIDTH = 29;
  static const int TILEMAP_HEIGHT = 21;

  float tile_width = 0;
  float tile_height = 0;

  std::unique_ptr<Tile> tile[TILEMAP_WIDTH][TILEMAP_HEIGHT];
};

#endif // SFMLGAME_TILEMAP_H
