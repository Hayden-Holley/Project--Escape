#pragma once

#include "SceneBase.h"
#include "../Objects/TextBox.h"
#include "../Objects/FadeInOut.h"

//Networking
#include "../Server.h"



#ifndef SFMLGAME_SCENE_SERVER_H
#define SFMLGAME_SCENE_SERVER_H

class Scene_Server : public SceneBase
{
 public:
  Scene_Server(sf::RenderWindow& game_window, sf::Music& game_music);
  ~Scene_Server() override;

  bool init(Game* game);
  bool shutDown();
  void keyPressed(Game* game, sf::Event event);
  void keyReleased(Game* game, sf::Event event);
  void textEntered(Game* game, sf::Event event);
  void mouseClicked(Game* game, sf::Event event);
  void joystickInput(Game* game, sf::Event event);
  void joystickMoved(Game* game, sf::Event event);
  void update(Game* game);
  void render(Game* game);

  void endServer();

  void runServer();

  bool is_loaded = false;

 private:
  //Global references
  sf::RenderWindow& window;
  sf::Music& music;

  //Camera
  sf::View camera = sf::View(sf::FloatRect(0,0,1080,720));

  //Next Scene
  Scenes next_scene;

  //Objects
  std::unique_ptr<Server> server = nullptr;

  bool is_server_running = false;

  std::unique_ptr<sf::Texture> background_texture;
  std::unique_ptr<sf::Sprite> background;
  std::unique_ptr<sf::Texture> vignette_texture;
  std::unique_ptr<sf::Sprite> vignette;

  float background_scale_target = 0.85;

  std::unique_ptr<FadeInOut> fade_in_out;
  std::unique_ptr<TextBox> server_code;
  std::unique_ptr<TextBox> main_text;


};

#endif // SFMLGAME_SCENE_SERVER_H
