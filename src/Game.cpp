#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window, sf::Music& game_music): window(game_window), music(game_music)
{
//  std::thread client_thread([&]{client = std::make_unique<Client>();client->connect(ip_address);client->run();});
//  client_thread.detach();

  srand(time(NULL));


}

Game::~Game()
{
  //c_server_scene.endServer();
  delete game_scenes.back();

}

bool Game::init()
{
  changeScene(Scenes::SPLASH);
  return true;

}

void Game::update(float dt)
{

  if(dt > 0.016)
  {
    dt = 0.016;
  }
  // wait for scene to load
  if(is_loading)
  {
    if (game_scenes.size() > 0)
    {
      if (game_scenes.back())
      {
        if (game_scenes.back()->init(this))
        {
          std::cout << "new scene loaded" << std::endl;
          is_loading = false;
          dt = 0;
        }
      }
    }
  }

  delta = dt;
  if (!is_loading)
  {
    if(game_scenes.back()->is_running)
    {
      game_scenes.back()->update(this);
    }
  }

}

void Game::render()
{

  if (!is_loading)
  {
    game_scenes.back()->render(this);
  }

}

void Game::mouseClicked(sf::Event event)
{
  sf::Vector2i click = sf::Mouse::getPosition(window);

  if (!is_loading)
  {
    game_scenes.back()->mouseClicked(this, event);
  }


}

void Game::keyPressed(sf::Event event)
{
  if (!is_loading)
  {
    game_scenes.back()->keyPressed(this, event);
  }

}

void Game::keyReleased(sf::Event event)
{
  if (!is_loading)
  {
    game_scenes.back()->keyReleased(this, event);
  }
}

void Game::joystickInput(sf::Event event)
{
  if (!is_loading)
  {
    game_scenes.back()->joystickInput(this, event);
  }

}

void Game::changeScene(Scenes scene)
{
  is_loading = true;
  if (is_loading)
  {
    current_scene = scene;

    if (!game_scenes.empty())
    {
      game_scenes.back()->is_running = false;

      // close current scene
      if (game_scenes.back()->shutDown())
      {
        delete game_scenes.back();
        game_scenes.pop_back();
      }
    }

    //change scene
    if (scene == Scenes::SPLASH)
    {
      //scene_splash = new Scene_Splash(window, music);
      game_scenes.push_back(scene_splash = new Scene_Splash(window, music));
      //std::cout << "splash screen" << std::endl;
    }
    else if (scene == Scenes::MAIN_MENU)
    {
      //scene_mainmenu = new Scene_MainMenu(window, music);
      game_scenes.push_back(scene_mainmenu = new Scene_MainMenu(window, music));
      //std::cout << "main menu" << std::endl;
    }
    else if (scene == Scenes::SERVER)
    {
      //scene_server = new Scene_Server(window, music);
      game_scenes.push_back(scene_server = new Scene_Server(window, music));
      //std::cout << "server" << std::endl;
    }
    else if (scene == Scenes::ESCAPEE)
    {
      //scene_escapee = new Scene_Escapee(window, music);
      game_scenes.push_back(scene_escapee = new Scene_Escapee(window, music) );
      //std::cout << "escapee scene" << std::endl;
    }
    else if (scene == Scenes::GUARD)
    {
      game_scenes.push_back(scene_guard = new Scene_Guard(window, music));
    }

  }
}

void Game::runServer()
{
  while (current_scene != Scenes::SERVER or is_loading)
  {
    if(!window.isOpen())
    {
      return;
    }

  }
  while (current_scene == Scenes::SERVER && !is_loading)
  {
    std::cerr << "starting server" << std::endl;
    if(scene_server != nullptr)
    {
      Scene_Server& c_server_scene = *scene_server;
      c_server_scene.runServer();
    }
    std::cerr << "server closed" << std::endl;
    if(!window.isOpen())
    {
      return;
    }

  }


}

void Game::textEntered(sf::Event event)
{
  if (!is_loading)
  {
    game_scenes.back()->textEntered(this, event);
  }

}

void Game::joystickMoved(sf::Event event)
{
  if (!is_loading)
  {
    game_scenes.back()->joystickInput(this, event);
    game_scenes.back()->joystickMoved(this, event);
  }
}
