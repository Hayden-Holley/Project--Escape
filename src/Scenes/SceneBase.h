

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
#include "SceneEnums.h"
//#include "../Game.h"

#ifndef SFMLGAME_SCENEBASE_H
#define SFMLGAME_SCENEBASE_H

class Game;

enum class GameplayStates{GAMEPLAY, END};

class SceneBase
{
 public:

  virtual bool init(Game* game) = 0;
  virtual bool shutDown() = 0;
  virtual void keyPressed(Game* game, sf::Event) = 0;
  virtual void keyReleased(Game* game, sf::Event) = 0;
  virtual void textEntered(Game* game, sf::Event) = 0;
  virtual void mouseClicked(Game* game, sf::Event) = 0;
  virtual void joystickInput(Game* game, sf::Event) = 0;
  virtual void joystickMoved(Game* game, sf::Event) = 0;
  virtual void update(Game* game) = 0;
  virtual void render(Game* game) = 0;


  virtual ~SceneBase(){};

  bool is_loaded = false;
  bool is_running = true;

 private:
  bool paused = false;

 protected:
  SceneBase(){};
};



#endif // SFMLGAME_SCENEBASE_H
