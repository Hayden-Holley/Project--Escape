//
// Created by Cameron on 29/04/2023.
//

#include "TextBox.h"
#include "SFML/Graphics.hpp"
#include "../Tools/Vector2.h"
#include "FadeInOut.h"

#ifndef SFMLGAME_ENDSCREEN_H
#define SFMLGAME_ENDSCREEN_H



class EndScreen
{
 public:
  EndScreen(sf::RenderWindow& game_window);
  ~EndScreen();

  void update(float dt);
  void render();

  void setPosition(float x, float y);
  void setEndMessage(std::string message);

  void startContinue();

  void startFadeIn();

 private:
  sf::RenderWindow& window;

  Vector2 position;

  std::unique_ptr<FadeInOut> background;
  std::unique_ptr<TextBox> end_message;
  std::unique_ptr<TextBox> continue_text;
  std::unique_ptr<sf::RectangleShape> background_rect;

  bool is_continue = false;



};

#endif // SFMLGAME_ENDSCREEN_H
