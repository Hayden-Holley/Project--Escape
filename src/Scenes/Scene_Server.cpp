#include "Scene_Server.h"
#include "../Game.h"
#include "../Tools/Lerp.h"
#include <SFML/System/Vector2.hpp>


Scene_Server::Scene_Server(sf::RenderWindow& game_window, sf::Music& game_music) : window(game_window), music(game_music)
{
  window.setView(camera);

  //server_code = std::make_unique<TextBox>(window, "");
  //server_code->text.setOrigin()

  is_loaded = true;

  //---Load Textures & Sprites---
  background_texture = std::make_unique<sf::Texture>();
  background_texture->loadFromFile("Data/Sprites/server_background.png");
  background = std::make_unique<sf::Sprite>();
  background->setScale(1.5,1.5);
  background->setTexture(*background_texture);
  background->setOrigin(background->getLocalBounds().width/2, background->getLocalBounds().height/2);
  background->setPosition(camera.getCenter().x, camera.getCenter().y);

  //Text
  main_text = std::make_unique<TextBox>(window, "You are now a dedicated server");
  main_text->text.setCharacterSize(40);
  main_text->text.setOrigin(main_text->text.getLocalBounds().width/2, main_text->text.getLocalBounds().height/2);
  main_text->text.setPosition(camera.getCenter().x, camera.getCenter().y);



  //Fade-in-out
  fade_in_out = std::make_unique<FadeInOut>(window);
  fade_in_out->fade_speed = 100;
  fade_in_out->startFadeOut();
}

Scene_Server::~Scene_Server()
{
  background.reset();
  server.reset();

}

bool Scene_Server::init(Game* game)
{
  return true;
}

bool Scene_Server::shutDown()
{
  return true;
}

void Scene_Server::keyPressed(Game* game, sf::Event event)
{

}

void Scene_Server::keyReleased(Game* game, sf::Event event)
{

}

void Scene_Server::update(Game* game)
{

    fade_in_out->update(game->delta);

    float x = Lerp::lerp(
      background->getScale().x, background_scale_target, 0.5 * game->delta);

    background->setScale(x, x);

    if(window.getSize().y > 600)
    {
      window.setSize(sf::Vector2u(window.getSize().x - 4, window.getSize().y-2));
      camera.setSize(camera.getSize().x - 4, camera.getSize().y-2);
      window.setPosition(sf::Vector2i(window.getPosition().x +2, window.getPosition().y + 1));
    }


}

void Scene_Server::render(Game* game)
{
  window.draw(*background);
  main_text->render();


  fade_in_out->render();


}

void Scene_Server::runServer()
{
  while(server == nullptr)
  {
    server = std::make_unique<Server>();
  }
  if (server != nullptr)
  {
    server->init();
    server->run();
  }
}

void Scene_Server::joystickInput(Game* game, sf::Event event)
{

}

void Scene_Server::endServer()
{
  server->end();
}

void Scene_Server::mouseClicked(Game* game, sf::Event event)
{

}


void Scene_Server::textEntered(Game* game, sf::Event event)
{

}

void Scene_Server::joystickMoved(Game* game, sf::Event event)
{

}
