//
#pragma once

#include "SceneBase.h"

// Codebase
#include "../Bot.h"
#include "../Character.h"
#include "../GameObject.h"
#include "../Tilemap.h"
#include "../Controller.h"
#include "../Objects/FadeInOut.h"
#include "../Objects/EndScreen.h"

//Networking
#include "../Server.h"
#include "../Client.h"



class Scene_Escapee : public SceneBase
{
 public:
  Scene_Escapee(sf::RenderWindow& game_window, sf::Music& game_music);
  ~Scene_Escapee() override;

  bool init(Game* game);
  bool shutDown();
  void keyPressed(Game* game, sf::Event event);
  void keyReleased(Game* game, sf::Event event);
  void mouseClicked(Game* game, sf::Event event);
  void textEntered(Game* game, sf::Event event);
  void joystickInput(Game* game, sf::Event event);
  void joystickMoved(Game* game, sf::Event event);
  void update(Game* game);
  void render(Game* game);

  void setToTile(sf::Sprite& sprite, int x, int y);
  void setCamera(int id);
  void handleEscapeeCamera();
  void handleCursor(float dt);
  void checkForEscapee();

  void setGameEnd(std::string message);

  bool is_loaded = false;

  bool can_leave = false;

 private:
  //Global references
  sf::RenderWindow& window;
  sf::Music& music;

  //Variables
  int camera_id = 1;
  int current_camera;

  GameplayStates game_play_state = GameplayStates::GAMEPLAY;

  //---escapee---
  Character escapee;
  sf::Vector2f escapeeLastPosition;
  CharacterState escapeeLastState;
  CharacterFacing escapeeLastFacing;

  //Objects
  Tilemap tilemap;
  sf::RectangleShape guard_view_blocker;
  std::unique_ptr<EndScreen> end_screen;

  // bot
  static const int BOT_AMOUNT = 100;
  std::unique_ptr<Bot> bots[BOT_AMOUNT];

  std::unique_ptr<Client> client = nullptr;
  //std::unique_ptr<Server> server = nullptr;

  //Cameras
  //sf::View camera = sf::View(sf::FloatRect(0,0,1080,720));
  sf::View view = sf::View(sf::FloatRect(0,0,1080,720));

  // Controller
  Controller controller;


  // cursor
  bool cursor_shown = true;

  float cursor_speed = 5;


  sf::RectangleShape cursor;

  //Next Scene
  Scenes next_scene;

};