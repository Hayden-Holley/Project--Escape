#pragma once

#ifndef PACMANSFML_TEST_H
#define PACMANSFML_TEST_H

#include "SFML/Graphics.hpp"
#include <iostream>

class TextBox
{
 public:
  TextBox(sf::RenderWindow& game_window, std::string text_string);
  ~TextBox();

  sf::Text text;

  void render();

  bool visible = true;

 private:
  sf::Font font;
  sf::RenderWindow& window;
};

#endif // PACMANSFML_TEST_H
