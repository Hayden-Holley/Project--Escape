
// System libs
#include "iostream"

// Codebase
#include "Tilemap.h"

void Tilemap::initMap(sf::RenderWindow& window, std::string filename)
{
  if (!map.load(filename))
  {
    std::cout<< "Failed to load map" << std::endl;
  }

  const auto& layers = map.getLayers();

  // get each layer's tiles
  const auto layer0  = layers[0]->getLayerAs<tmx::TileLayer>();
  const auto& tiles0 = layer0.getTiles();
  const auto layer1  = layers[1]->getLayerAs<tmx::TileLayer>();
  const auto& tiles1 = layer1.getTiles();

  int x;
  int y;

  sf::Texture texture;
  if (!texture.loadFromFile("Data/Maps/tilesheet3.png"))
  {
    std::cout << "Couldn't load the texture properly" << std::endl;
    return;
  }


  for (int i = 0; i < tiles0.size(); ++i)
  {
    x = i % TILEMAP_WIDTH;
    y = i / TILEMAP_WIDTH;

    tile[x][y] = std::make_unique<Tile>();

    // set texture
    tile[x][y]->setLoadedTexture(texture);

    // set id
    tile[x][y]->id = tiles1[i].ID;

    // set tile type
    if ((tile[x][y]->id == 54) ||
        (tile[x][y]->id == 0))
    {
      tile[x][y]->setType(TileType::ROAD);
    }

    // set to background
    tile[x][y]->getSprite()->setTextureRect(sf::IntRect(0, 0, 64, 64));

    tile[x][y]->getSprite()->setScale(window.getSize().x / (tile[x][y]->getSprite()->getGlobalBounds().width * TILEMAP_WIDTH),
                                      window.getSize().y / (tile[x][y]->getSprite()->getGlobalBounds().height * TILEMAP_HEIGHT));
    tile[x][y]->getSprite()->setPosition(x * tile[x][y]->getSprite()->getGlobalBounds().width,
                                         y * tile[x][y]->getSprite()->getGlobalBounds().height);

    // if possible, set to wall and pellets
    tile[x][y]->getSprite()->setTextureRect(findTextureRect(tile[x][y]->id));
  }

  //get tile size
  tile_width = tile[0][0]->getSprite()->getGlobalBounds().width;
  tile_height = tile[0][0]->getSprite()->getGlobalBounds().height;

  //Set door as 29, 20
  tile[28][19]->getSprite()->setTextureRect(findTextureRect(34));
  tile[28][19]->setType(TileType::WALL);
  tile[28][19]->id = 33;
}

sf::IntRect Tilemap::findTextureRect(int id)
{
  // get id to 0, 1, 2, ...
  id -= 1;

  return {(id % 9) * 64, (id / 9) * 64, 64, 64};
}

void Tilemap::render(sf::RenderWindow& window)
{
  for (int i = 0; i < TILEMAP_WIDTH; ++i)
  {
    for (int j = 0; j < TILEMAP_HEIGHT; ++j)
    {
      window.draw(*tile[i][j]->getSprite());
    }
  }
}

std::unique_ptr<Tile>& Tilemap::getTile(int x, int y)
{
  return tile[x][y];
}

std::unique_ptr<Tile>& Tilemap::getTile(sf::Vector2i vec)
{
  return tile[vec.x][vec.y];
}

int Tilemap::tilemapWidth()
{
  return TILEMAP_WIDTH;
}

int Tilemap::tilemapHeight()
{
  return TILEMAP_HEIGHT;
}

float Tilemap::getTileWidth()
{
  return tile_width;
}

float Tilemap::getTileHeight()
{
  return tile_height;
}
