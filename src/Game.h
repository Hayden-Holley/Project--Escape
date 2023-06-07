
#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SFML/Graphics.hpp>
#include <thread>
#include "Server.h"
#include "Client.h"

class Game
{
 public:
  Game(sf::RenderWindow& window, bool server);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void textInput(sf::Event event);

 private:
  sf::RenderWindow& window;
  bool isServer = true;

  std::unique_ptr<Client> client = nullptr;
  std::unique_ptr<Server> server = nullptr;

  sf::Font font;
  sf::String playerInput;
  sf::Text playerText;
  sf::Text messagesOnScreen[8];

};

#endif // PLATFORMER_GAME_H
