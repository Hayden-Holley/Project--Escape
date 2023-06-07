
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window, bool server)
  : window(game_window), isServer(server)
{
  srand(time(NULL));
}

Game::~Game()
{

}

bool Game::init()
{
  // creating a server or a client
  if (isServer)
  {
    server = std::make_unique<Server>();
    server->init();
    server->run();
  }
  else
  {
    std::thread client_thread ([&]{
                                client = std::make_unique<Client>();
                                client->connect();
                                client->run();
                              });
    client_thread.detach();
  }

  if (!font.loadFromFile("Data/Fonts/OpenSans-Bold.ttf"))
  {
    std::cout << "font did not load";
  }

  playerText.setFillColor(sf::Color::White);
  playerText.setFont(font);
  playerText.setCharacterSize(24);
  playerText.setPosition
    (
      10,
      window.getSize().y - 40
    );

  return true;
}

void Game::update(float dt)
{
  for (int i = 0; i < sizeof(messagesOnScreen)/sizeof(sf::Text); i++)
  {
    messagesOnScreen[i].setString(client->messagesToOutput[i]);
    messagesOnScreen[i].setFillColor(sf::Color::White);
    messagesOnScreen[i].setFont(font);
    messagesOnScreen[i].setCharacterSize(24);
    messagesOnScreen[i].setPosition
      (
        10,
        10 + (i * 40)
      );
  }
}

void Game::render()
{
  //rendering goes here
  for (int i = 0; i < sizeof(messagesOnScreen)/sizeof(sf::Text); i++)
  {
    window.draw(messagesOnScreen[i]);
  }
  window.draw(playerText);
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);


}

void Game::keyPressed(sf::Event event)
{
  if (event.key.code == sf::Keyboard::Enter)
  {
    client->receivedInput = playerInput;
    playerInput.clear();
    playerText.setString(playerInput);
  }

  if (event.key.code == sf::Keyboard::BackSpace)
  {
    playerInput = playerInput.substring(0, playerInput.getSize()-1);
    playerText.setString(playerInput);
  }
}

void Game::textInput(sf::Event event)
{
  if (
    event.text.unicode < 128
    && event.text.unicode != 8
    )
  {
    playerInput += event.text.unicode;
    playerText.setString(playerInput);
  }
}
