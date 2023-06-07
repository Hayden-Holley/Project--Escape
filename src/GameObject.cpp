
// System libs
#include <iostream>

// Codebase
#include "GameObject.h"


bool GameObject::setTexture(std::string filename)
{
  if (!texture.loadFromFile(filename))
  {
    std::cout << "Texture did not load\n";
    return false;
  }

  sprite->setTexture(texture);

  return true;
}

bool GameObject::setLoadedTexture(const sf::Texture& loaded_texture)
{
  texture = loaded_texture;

  sprite->setTexture(texture);

  return false;
}

std::unique_ptr<sf::Sprite>& GameObject::getSprite()
{
  return sprite;
}

float GameObject::getRight()
{
  return sprite->getGlobalBounds().left + sprite->getGlobalBounds().width;
}

float GameObject::getBottom()
{
  return sprite->getGlobalBounds().top + sprite->getGlobalBounds().height;
}

float GameObject::getLeft()
{
  return sprite->getGlobalBounds().left;
}

float GameObject::getTop()
{
  return sprite->getGlobalBounds().top;
}

float GameObject::getMiddleX()
{
  float x = sprite->getGlobalBounds().left + sprite->getGlobalBounds().width / 2;
  return x;
}

float GameObject::getMiddleY()
{
  float x = sprite->getGlobalBounds().top + sprite->getGlobalBounds().height / 2;
  return x;
}

float GameObject::getWidth()
{
  return sprite->getGlobalBounds().width;
}

float GameObject::getHeight()
{
  return sprite->getGlobalBounds().height;
}
