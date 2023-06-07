
#include "Controller.h"

void Controller::checkAxisMove()
{
  joystick_position = sf::Vector2f(0,0);
  if ((sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -joystick_dead_zone) ||
      (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > joystick_dead_zone) ||
      (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -joystick_dead_zone) ||
      (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > joystick_dead_zone))
  {
    joystick_moved = true;

    joystick_position.x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
    joystick_position.y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

    // x axis
    // left
    if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -joystick_dead_zone)
    {
      going_left     = true;
      joystick_moved = true;
    }

    // right
    else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > joystick_dead_zone)
    {
      going_right    = true;
      joystick_moved = true;
    }
    else
    {
      going_left     = false;
      going_right    = false;
      joystick_moved = false;
    }

    // y axis
    // up
    if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -joystick_dead_zone)
    {
      going_up       = true;
      joystick_moved = true;
    }

    // down
    else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > +joystick_dead_zone)
    {
      going_down     = true;
      joystick_moved = true;
    }
    else
    {
      going_up       = false;
      going_down     = false;
      joystick_moved = false;
    }
  }
  else
  {
    joystick_moved = false;
  }
}

bool Controller::checkButtonPressed(int button)
{
  if (sf::Joystick::isButtonPressed(0, button))
  {
    return true;
  }
  return false;
}
