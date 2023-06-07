//
// Created by Cameron on 27/04/2022.
//

#include <iostream>

#include "FadeInOut.h"

FadeInOut::FadeInOut(sf::RenderWindow& game_window) : window(game_window)
{
  rectangle.setFillColor(sf::Color::Black);

}

FadeInOut::~FadeInOut()
{

}

void FadeInOut::render()
{
  window.draw(rectangle);

}

void FadeInOut::update(float dt)
{

  rectangle.setSize(sf::Vector2f(size.x, size.y));
  //rectangle.setPosition(position.x, position.y);

  if (fade == 1)
  {
    if (opacity <= min_opacity)
    {
      opacity = 1;
    }
    if(opacity < max_opacity)
    {
      opacity += fade_speed * dt;
    }
    else
    {
      opacity = max_opacity;
      fade = 0;
    }

  }
  else if (fade == -1)
  {
    if (opacity >= max_opacity)
    {
      opacity = max_opacity-1;
    }
    if(opacity > min_opacity)
    {
      opacity -= fade_speed * dt;
    }
    else
    {
      opacity = min_opacity;
      fade = 0;
    }

  }

  if (opacity < min_opacity)
  {
    opacity = min_opacity;
  }
  if (opacity > max_opacity)
  {
    opacity = max_opacity;
  }


  rectangle.setFillColor(sf::Color(rectangle.getFillColor().r,rectangle.getFillColor().g,rectangle.getFillColor().b,opacity));

}


void FadeInOut::startFadeIn(float speed)
{
  fade_speed = speed;
  fade = 1;
}

void FadeInOut::startFadeOut(float speed)
{
  fade_speed = speed;
  fade = -1;
}

void FadeInOut::startFadeIn()
{
  fade = 1;
}

void FadeInOut::startFadeOut()
{
  fade = -1;
}

void FadeInOut::setColour(sf::Color colour)
{
  rectangle.setFillColor(colour);
}

bool FadeInOut::OnFinishedFadeOut()
{
  if (opacity > 0)
  {
    is_finished_fade_out = false;
  }

  if(is_finished_fade_out)
  {
    return false;
  }

  if (opacity <= 0 && !is_finished_fade_out)
  {
    is_finished_fade_out = true;
    return true;
  }

  return false;
}

bool FadeInOut::OnFinishedFadeIn()
{
  if (opacity < 255)
  {
    is_finished_fade_in = false;

  }

  if(is_finished_fade_in)
  {
    return false;
  }

  if (opacity >= 255 && !is_finished_fade_in)
  {
    is_finished_fade_in = true;
    return true;
  }
  return false;
}

void FadeInOut::setPosition(float x, float y)
{
  rectangle.setPosition(x, y);
  position.x = x;
  position.y = y;
}

void FadeInOut::setOriginCenter()
{
  rectangle.setOrigin(rectangle.getPosition().x + (size.x/2), rectangle.getPosition().y + (size.y/2));
}
