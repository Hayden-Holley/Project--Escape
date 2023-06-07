//
// Created by Cameron on 27/04/2022.
//

#include "SFML/Graphics.hpp"
#include "../Tools/Vector2.h"


#ifndef PLATFORMERSFML_FADEINOUT_H
#define PLATFORMERSFML_FADEINOUT_H

class FadeInOut
{
 public:
  FadeInOut(sf::RenderWindow& game_window);
  ~FadeInOut();

  void render();
  void update(float dt);


  void startFadeIn();

  void startFadeIn(float speed);

  void startFadeOut();

  void setPosition(float x, float y);

  void setOriginCenter();

  void setColour(sf::Color colour);

  void startFadeOut(float speed);

  bool OnFinishedFadeOut();
  bool OnFinishedFadeIn();

  int fade = 0;

  float opacity = 255;
  float max_opacity = 255;
  float min_opacity = 0;

  float fade_speed = 150;



  sf::Vector2f position;

  Vector2 size = Vector2(1080,720);





 private:
  sf::RenderWindow& window;

  sf::RectangleShape rectangle;


  bool is_finished_fade_out = false;
  bool is_finished_fade_in = true;



};

#endif // PLATFORMERSFML_FADEINOUT_H
