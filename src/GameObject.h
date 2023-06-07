
#ifndef SFMLGAME_GAMEOBJECT_H
#define SFMLGAME_GAMEOBJECT_H

// Game Engine
#include <SFML/Graphics.hpp>

class GameObject
{
 public:
  // function 1
  // Sets the texture of the sprite.
  bool setTexture(std::string filename);
  bool setLoadedTexture(const sf::Texture& loaded_texture);

  // function 2
  // Returns a unique pointer to the game object's sprite.
  std::unique_ptr<sf::Sprite>& getSprite();

  // function 3
  // Returns the x position of the right and left side of the sprite
  float getRight();
  float getLeft();

  // function 4
  // Returns the y position of the top and bottom side of the sprite.
  float getTop();
  float getBottom();

  // function 5
  // Returns the x and y position of the center of the game object.
  float getMiddleX();
  float getMiddleY();

  // function 6
  // Returns the width and height of the game object's sprite.
  float getWidth();
  float getHeight();

 private:
  std::unique_ptr<sf::Sprite> sprite = std::make_unique<sf::Sprite>();

  sf::Texture texture;
};

#endif // SFMLGAME_GAMEOBJECT_H
