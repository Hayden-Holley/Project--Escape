#pragma once

#include "SceneBase.h"

//Networking
#include "../Server.h"
#include "../Client.h"

// Codebase
#include "../Bot.h"
#include "../Character.h"
#include "../GameObject.h"
#include "../Tilemap.h"
#include "../Controller.h"
#include "../Objects/EndScreen.h"




class Scene_Guard : public SceneBase
{
 public:
  Scene_Guard(sf::RenderWindow& game_window, sf::Music& game_music);
  ~Scene_Guard() override;

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

  void handleGuardSickness(float dt);
  void setCamera(int id);
  void setToTile(sf::Sprite& sprite, int x, int y);
  void checkForEscapee();
  void handleCursor(float dt);

  void setGameEnd(std::string message);

  bool is_loaded = false;

 private:
  //Global references
  sf::RenderWindow& window;
  sf::Music& music;

  bool can_leave = false;
  GameplayStates game_play_state = GameplayStates::GAMEPLAY;

  //Networking
  std::unique_ptr<Client> client = nullptr;

  //Objects
  sf::RectangleShape guard_view_blocker;
  std::unique_ptr<EndScreen> end_screen;

  // Controller
  Controller controller;

  // cursor
  bool cursor_shown = true;
  float cursor_speed = 5;
  sf::RectangleShape cursor;

  // Guard
  sf::View view;
  int current_camera;

  int camera_id = 1;
  int previous_camera_id = 1;

  float view_blocker_timer = 0;
  float view_blocker_timer_default = 10;

  //---escapee---
  Character escapee;
  sf::Vector2f escapeeLastPosition;
  CharacterState escapeeLastState;
  CharacterFacing escapeeLastFacing;

  // bot
  static const int BOT_AMOUNT = 100;
  std::unique_ptr<Bot> bots[BOT_AMOUNT];


  //---Tilemap---
  Tilemap tilemap;

  //Camera
  sf::View camera = sf::View(sf::FloatRect(0,0,1080,720));

  //Next Scene
  Scenes next_scene;

};