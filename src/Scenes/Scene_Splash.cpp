
#include "Scene_Splash.h"
#include "../Game.h"
#include "../Tools/Lerp.h"

Scene_Splash::Scene_Splash(sf::RenderWindow& game_window, sf::Music& game_music) : window(game_window), music(game_music)
{
  window.setView(camera);
  //Splash screen background
  splash_background_texture = std::make_unique<sf::Texture>();
  splash_background_texture->loadFromFile("Data/Sprites/splash_background.png");
  splash_background = std::make_unique<sf::Sprite>();
  splash_background->setTexture(*splash_background_texture);
  //Splash screen Logo
  logo_texture = std::make_unique<sf::Texture>();
  logo_texture->loadFromFile("Data/Sprites/splash_logo.png");
  logo = std::make_unique<sf::Sprite>();
  logo->setTexture(*logo_texture);
  logo->setOrigin(logo->getLocalBounds().width/2, logo->getLocalBounds().height/2);
  logo->setPosition(camera.getCenter().x, camera.getCenter().y);
  logo->setScale(80,80);

  fade_in_out = std::make_unique<FadeInOut>(window);
  fade_in_out->startFadeOut();

}

Scene_Splash::~Scene_Splash()
{
  splash_background.reset();
  logo.reset();
  fade_in_out.reset();

}

bool Scene_Splash::init(Game* game)
{
  return true;
}

bool Scene_Splash::shutDown()
{
  return true;
}

void Scene_Splash::keyPressed(Game* game, sf::Event event)
{
  if(event.key.code)
  {
    StartSceneChange();
  }

}

void Scene_Splash::keyReleased(Game* game, sf::Event event)
{

}

void Scene_Splash::update(Game* game)
{
  fade_in_out->update(game->delta);

  float l_scale = Lerp::lerp(logo->getScale().x, logo_scale_target, logo_lerp_speed * game->delta);

  logo->setScale(l_scale,l_scale);

  if(fade_in_out->OnFinishedFadeOut())
  {
    is_start_fade_in = true;
    clock.restart();
  }

  if(clock.getElapsedTime().asSeconds() >= 2 && is_start_fade_in)
  {
    StartSceneChange();
  }

  if(is_changing_scene && fade_in_out->opacity >= 255)
  {
    game->changeScene(next_scene);
  }

}

void Scene_Splash::render(Game* game)
{
  window.draw(*splash_background);
  window.draw(*logo);
  fade_in_out->render();

}

void Scene_Splash::StartSceneChange()
{
  next_scene = Scenes::MAIN_MENU;
  is_changing_scene = true;
  fade_in_out->startFadeIn();
  if (logo->getScale().x < 1.1)
  {
    logo_lerp_speed = 0.1;
    logo_scale_target = 0;
  }

}

void Scene_Splash::joystickInput(Game* game, sf::Event event)
{
  if (sf::Joystick::isButtonPressed(0, 2))
  {
    StartSceneChange();
  }
}

void Scene_Splash::mouseClicked(Game* game, sf::Event event)
{

}

void Scene_Splash::textEntered(Game* game, sf::Event event)
{

}

void Scene_Splash::joystickMoved(Game* game, sf::Event event)
{


}
