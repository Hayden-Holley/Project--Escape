//
// Created by Cameron on 29/04/2023.
//

#include "EndScreen.h"


EndScreen::EndScreen(sf::RenderWindow& game_window)  : window(game_window)
{
  background = std::make_unique<FadeInOut>(window);
  background->setOriginCenter();
  background->opacity = 0;


  background_rect = std::make_unique<sf::RectangleShape>();
  background_rect->setSize(sf::Vector2f(1080,720));
  background_rect->setOrigin(background_rect->getLocalBounds().width/2, background_rect->getLocalBounds().height/2);
  background_rect->setFillColor(sf::Color::Black);
  //background->opacity = 255;

  end_message = std::make_unique<TextBox>(window, "");

  continue_text = std::make_unique<TextBox>(window, "Press 'A' or 'Enter' to return to the title screen");
  continue_text->text.setCharacterSize(20);
  continue_text->text.setOrigin(continue_text->text.getLocalBounds().width/2, continue_text->text.getLocalBounds().height/2);

  end_message->text.setFillColor(sf::Color(255,255,255,background->opacity));
  continue_text->text.setFillColor(sf::Color(255,255,255,background->opacity));

}

EndScreen::~EndScreen()
{
  background.reset();
  end_message.reset();
  continue_text.reset();
}

void EndScreen::update(float dt)
{
  background->update(dt);
  end_message->text.setOrigin(end_message->text.getLocalBounds().width/2, end_message->text.getLocalBounds().height/2);
  end_message->text.setPosition(position.x, position.y-100);
  continue_text->text.setPosition(position.x, position.y + 150);
  background_rect->setPosition(position.x,position.y);
  background_rect->setFillColor(sf::Color(0,0,0,background->opacity));

  end_message->text.setFillColor(sf::Color(255,255,255,background->opacity));

  if(is_continue)
  {
    int c_opacity = continue_text->text.getFillColor().a;
    if(c_opacity < background->opacity)
    {
      continue_text->text.setFillColor(sf::Color(255,255,255,c_opacity + 1));
    }
    else
    {
      continue_text->text.setFillColor(sf::Color(255,255,255,background->opacity));
    }
  }


}

void EndScreen::setPosition(float x, float y)
{
  position.x = x;
  position.y = y;

}

void EndScreen::setEndMessage(std::string message)
{
  end_message->text.setString(message);
  end_message->text.setOrigin(end_message->text.getLocalBounds().width/2, end_message->text.getLocalBounds().height/2);
}

void EndScreen::startContinue()
{
  is_continue = true;
}

void EndScreen::render()
{
  window.draw(*background_rect);
  //background->render();
  end_message->render();
  continue_text->render();
}

void EndScreen::startFadeIn()
{
  background->startFadeIn();
}
