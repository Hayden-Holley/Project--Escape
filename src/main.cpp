#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"


int main()
{
  std::cout << "You should see a window that opens as well as this writing to console..."
            << std::endl;


  sf::Image icon;
  icon.loadFromFile("../Data/Sprites/pe-icon.png");

  // create window and set up
  sf::RenderWindow window(sf::VideoMode(1080, 720), "Project: escape", sf::Style::Titlebar | sf::Style::Close);
  window.setFramerateLimit(60);
  window.setIcon(icon.getSize().x,icon.getSize().y,icon.getPixelsPtr());

  sf::Music music;

  //environment variable to determine server, guard, and escapee
  int type_index;
  RunType game_type;

  //initialise an instance of the game class
  Game game(window, music);

  //run the init function of the game class and check it all initialises ok
  if (!game.init())
  {
    return 0;
  }


  std::thread t1(&Game::runServer, &game);
  t1.detach();



  // A Clock starts counting as soon as it's created
  sf::Clock clock;

  // Game loop: run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;

    //calculate delta time
    sf::Time time = clock.restart();
    float dt = time.asSeconds();

    //'process inputs' element of the game loop
    while (window.pollEvent(event))
    {
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyPressed)
        game.keyPressed(event);

      if (event.type == sf::Event::KeyReleased)
      {
        game.keyReleased(event);
      }
      if (event.type == sf::Event::TextEntered)
      {
        game.textEntered(event);
      }
      if (event.type == sf::Event::MouseButtonPressed)
      {
        game.mouseClicked(event);
      }

      //---Controller input---
      if (sf::Joystick::isConnected(0))
      {

        if(window.hasFocus())
        {
          // game.joystickMoved(event);
          //  Player movement / Joystick movement
          if (event.type == sf::Event::JoystickMoved)
          {
            game.joystickMoved(event);
          }

          // Interact pressed / Button pressed
          if (event.type == sf::Event::JoystickButtonPressed)
          {
            game.joystickInput(event);
          }
        }
      }
    }



    //'update' element of the game loop
    game.update(dt);

    window.clear(sf::Color::Black);

    //'render' element of the game loop
    game.render();
    window.display();
  }



  return 0;
}
