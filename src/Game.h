
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

// Game Engine & Libraries
#include "tmxlite/Map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Codebase
#include "Bot.h"
#include "Character.h"
#include "GameObject.h"
#include "Tilemap.h"
#include "Controller.h"

//Scenes
#include "Scenes/SceneEnums.h"
#include "Scenes/SceneBase.h"
#include "Scenes/Scene_MainMenu.h"
#include "Scenes/Scene_Server.h"
#include "Scenes/Scene_Splash.h"
#include "Scenes/Scene_Escapee.h"
#include "Scenes/Scene_Guard.h"


//Networking
#include "Server.h"
#include "Client.h"

enum class RunType {SERVER, ESCAPEE, GUARD};
//enum class GameState {MAIN_MENU, GAME, GAME_END};



class Game
{
 public:
  Game(sf::RenderWindow& window, sf::Music& music);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);
  void joystickInput(sf::Event event);
  void joystickMoved(sf::Event event);
  void textEntered(sf::Event event);



  void runServer();


  void setToTile(sf::Sprite& sprite, int x, int y);

  void setCamera(int id);
  void handleEscapeeCamera();
  void handleGuardSickness(float dt);
  void handleCursor(float dt);
  void checkForEscapee();

  void changeScene(Scenes scene);

  bool is_loading = false;
  float delta;

  sf::String ip_address = "127.0.0.1";

  //std::unique_ptr<Client> client = nullptr;

 private:
  sf::RenderWindow& window;
  sf::Music& music;



  //std::unique_ptr<Client> client = nullptr;
  //std::unique_ptr<Server> server = nullptr;

  // game state
  //GameState game_state = GameState::GAME;

  //---Tilemap---
  Tilemap tilemap;

  //---escapee---
  Character escapee;
  sf::Vector2f escapeeLastPosition;
  CharacterState escapeeLastState;
  CharacterFacing escapeeLastFacing;

  // bot
  static const int BOT_AMOUNT = 100;
  std::unique_ptr<Bot> bots[BOT_AMOUNT];

  // Guard
  sf::View view;
  int current_camera;

  int camera_id = 1;
  int previous_camera_id = 1;

  float view_blocker_timer = 0;
  float view_blocker_timer_default = 10;

  sf::RectangleShape guard_view_blocker;






  //Game Scenes
  Scenes current_scene;
  Scene_Splash *scene_splash;
  Scene_MainMenu *scene_mainmenu;
  Scene_Server *scene_server;
  Scene_Escapee *scene_escapee;
  Scene_Guard *scene_guard;


  std::vector<SceneBase*> game_scenes;

  // Controller
  Controller controller;

  // cursor
  bool cursor_shown = true;
  float cursor_speed = 5;
  sf::RectangleShape cursor;
};

#endif // PLATFORMER_GAME_H
