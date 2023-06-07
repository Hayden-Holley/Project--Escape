
#ifndef SFMLGAME_CONTROLLER_H
#define SFMLGAME_CONTROLLER_H

// Game Engine
#include <SFML/Graphics.hpp>

class Controller
{
 public:
  void checkAxisMove();
  bool checkButtonPressed(int button);

  bool going_left = false;
  bool going_right = false;
  bool going_up = false;
  bool going_down = false;

  bool joystick_moved = false;
  bool camera_switch_pressed = false;

  sf::Vector2f joystick_position;

 private:
  float joystick_dead_zone = 20;
};

#endif // SFMLGAME_CONTROLLER_H
