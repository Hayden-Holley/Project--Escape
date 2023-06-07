//
// Created by Cameron on 27/04/2023.
//

#include "Scene_Escapee.h"

#include <utility>
#include "../Game.h"


Scene_Escapee::Scene_Escapee(sf::RenderWindow& game_window, sf::Music& game_music) : window(game_window), music(game_music)
{


  srand(time(NULL));

}

Scene_Escapee::~Scene_Escapee()
{
  bool is_closing = true;
  client->stopClient(0);
  while(is_closing)
  {
    //std::cout << client->close << " " << client->connected << " " << client->disconnected << std::endl;
    if(client->disconnected)
    {
      client.reset();
      is_closing = false;
    }
  }
}

bool Scene_Escapee::init(Game* game)
{
  std::thread client_thread([&]{client = std::make_unique<Client>();client->connect(game->ip_address);client->run();});
  client_thread.detach();



  //---Tilemap---
  tilemap.initMap(window, "Data/Maps/map29x21.tmx");

  //---Escapee---
  escapee.setTexture("Data/Images/char_a_p1_0bas_humn_v01.png");
  escapee.resetTextureRect();
  escapee.collider_width = tilemap.getTile(0, 0)->getWidth();
  escapee.collider_height = tilemap.getTile(0, 0)->getHeight();
  escapee.collider_reach = tilemap.getTile(0, 0)->getWidth() * 1.5f;
  setToTile(*escapee.getSprite(), 1, 1);
  escapee.isEscapee  = true;
  escapee.buttonsHit = 0;
  escapee.actual_escapee = true;
  //Change colour for Escapee view to see where they are

  //A nice orange :)
  escapee.getSprite()->setColor(sf::Color(	255, 165, 0));


  // bot
  for (int i = 0; i < BOT_AMOUNT; ++i)
  {
      bots[i] = std::make_unique<Bot>();
      bots[i]->setTexture("Data/Images/char_a_p1_0bas_humn_v01.png");
      bots[i]->initBot();
      bots[i]->resetTextureRect();
      bots[i]->collider_width = tilemap.getTile(0, 0)->getWidth();
      bots[i]->collider_height = tilemap.getTile(0, 0)->getHeight();
      bots[i]->collider_reach = tilemap.getTile(0, 0)->getWidth() * 1.5f;
      setToTile(*bots[i]->getSprite(), 1, 1);
      bots[i]->isEscapee = false;
  }

  //---Set camera default position---
  //Set scale and position to view top left area
  //Scale is always 1080 / 2, 720 / 2
  //Top left = 0, 0 Top right = 1080 / 2, 0 Bottom left = 0, 720 / 2 Bottom right = 1080 / 2, 720 / 2
  setCamera(1);

  // guard
  guard_view_blocker.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
  guard_view_blocker.setFillColor(sf::Color::Black);

  end_screen = std::make_unique<EndScreen>(window);

  return true;
}

bool Scene_Escapee::shutDown()
{
  return true;
}

void Scene_Escapee::keyPressed(Game* game, sf::Event event)
{
  if(game_play_state == GameplayStates::GAMEPLAY)
  {
    if (
      (event.key.code == sf::Keyboard::A) ||
      (event.key.code == sf::Keyboard::Left))
    {
      escapee.goLeft();
    }
    if (
      (event.key.code == sf::Keyboard::D) ||
      (event.key.code == sf::Keyboard::Right))
    {
      escapee.goRight();
    }
    if ((event.key.code == sf::Keyboard::W) || (event.key.code == sf::Keyboard::Up))
    {
      escapee.goUp();
    }
    if (
      (event.key.code == sf::Keyboard::S) ||
      (event.key.code == sf::Keyboard::Down))
    {
      escapee.goDown();
    }
  }
  if(game_play_state == GameplayStates::END)
  {
    if (can_leave)
    {
      if (event.key.code == sf::Keyboard::Enter)
      {
        game->changeScene(Scenes::MAIN_MENU);
      }
    }
  }


}

void Scene_Escapee::keyReleased(Game* game, sf::Event event)
{
  if(game_play_state == GameplayStates::GAMEPLAY)
  {
    if (!escapee.goingLeft())
    {
      if (
        (event.key.code == sf::Keyboard::D) ||
        (event.key.code == sf::Keyboard::Right))
      {
        escapee.stopXMovement();
      }
    }

    if (escapee.goingLeft())
    {
      if (
        (event.key.code == sf::Keyboard::A) ||
        (event.key.code == sf::Keyboard::Left))
      {
        escapee.stopXMovement();
      }
    }

    if (!escapee.goingUp())
    {
      if (
        (event.key.code == sf::Keyboard::S) ||
        (event.key.code == sf::Keyboard::Down))
      {
        escapee.stopYMovement();
      }
    }

    if (escapee.goingUp())
    {
      if (
        (event.key.code == sf::Keyboard::W) ||
        (event.key.code == sf::Keyboard::Up))
      {
        escapee.stopYMovement();
      }
    }
  }


}

void Scene_Escapee::update(Game* game)
{
  //  if(client->disconnected)
  //  {
  //    next_scene = Scenes::MAIN_MENU;
  //    game->changeScene(next_scene);
  //  }

  end_screen->setPosition(view.getCenter().x, view.getCenter().y);
  end_screen->update(game->delta);

  if (game_play_state == GameplayStates::GAMEPLAY)
  {
    escapee.update(game->delta, tilemap);

    for (int i = 0; i < BOT_AMOUNT; ++i)
    {
      bots[i]->updateBot(game->delta, tilemap);
    }

    handleCursor(game->delta);
    checkForEscapee();

    handleEscapeeCamera();





    // Checking to see if the Escapee has moved
    if (escapeeLastPosition != escapee.getSprite()->getPosition())
    {
      // Storing the Escapee's position in a variable in the Client
      client->escapeePositionToSend = escapee.getSprite()->getPosition();
      // Update the Escapee's last position
      escapeeLastPosition        = escapee.getSprite()->getPosition();
      client->thread_designation = PacketTags::vector2f;
      // Boolean "tells" the Client that the Escapee has moved
      client->hasEscapeePositionChanged = true;
    }

    // Checking to see if the Escapee's state has changed
    if (escapeeLastState != escapee.getCharacterState())
    {
      // Storing the Escapee's state in a variable in the Client
      switch (escapee.getCharacterState())
      {
        case CharacterState::IDLE:
          client->escapee_state_to_send = 0;
          break;
        case CharacterState::WALKING:
          client->escapee_state_to_send = 1;
          break;
      }
      // Storing the Escapee's state in a variable in the Client
      // client->escapee_state_to_send = player.getCharacterState();
      // Update the Escapee's last state
      escapeeLastState           = escapee.getCharacterState();
      client->thread_designation = PacketTags::characterstate;
      // Boolean "tells" the Client that the Escapee has changed state
      client->hasEscapeeStateChanged = true;
    }

    // Checking to see if the Escapee's facing has changed
    if (escapeeLastFacing != escapee.getCharacterFacing())
    {
      // Storing the Escapee's facing in a variable in the Client
      switch (escapee.getCharacterFacing())
      {
        case CharacterFacing::UP:
          client->escapee_facing_to_send = 0;
          break;
        case CharacterFacing::DOWN:
          client->escapee_facing_to_send = 1;
          break;
        case CharacterFacing::LEFT:
          client->escapee_facing_to_send = 2;
          break;
        case CharacterFacing::RIGHT:
          client->escapee_facing_to_send = 3;
          break;
      }
      // Storing the Escapee's facing in a variable in the Client
      // client->escapee_facing_to_send = player.getCharacterFacing();
      // Update the Escapee's last facing
      escapeeLastFacing          = escapee.getCharacterFacing();
      client->thread_designation = PacketTags::characterfacing;
      // Boolean "tells" the Client that the Escapee has changed facing
      client->hasEscapeeFacingChanged = true;
    }

    if (client->hasEscapeeCaughtPacketBeenReceived)
    {
      std::cout << "you lose" << std::endl;
      if (client->guard_win_condition_received == 1)
      {
        client->thread_designation                 = PacketTags::none;
        client->hasEscapeeCaughtPacketBeenReceived = false;
        setGameEnd("You Lose!");
      }
    }

    if (escapee.getSprite()->getPosition().x > window.getSize().x)
    {
      // std::cout<< "Escapee has escaped" << std::endl;
      //game->changeScene(Scenes::MAIN_MENU);
      setGameEnd("You Win!");
    }
  }
  else if (game_play_state == GameplayStates::END)
  {
    if(client->disconnected)
    {
      can_leave = true;
      end_screen->startContinue();
    }
  }
}

void Scene_Escapee::render(Game* game)
{
    //rendering goes here

    //---Tilemap---
    tilemap.render(window);

    // bot
    for (int i = 0; i < BOT_AMOUNT; ++i)
    {
        window.draw(*bots[i]->getSprite());
    }

    // escapee
    window.draw(*escapee.getSprite());

    //End screen
    end_screen->render();

}

void Scene_Escapee::joystickInput(Game* game, sf::Event event)
{
    // show cursor
    cursor_shown = true;

    controller.checkAxisMove();

    //---Joystick movement---
    //Right
    if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 20)
    {
        escapee.goRight();
    }
    //Left
    else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -20)
    {
        escapee.goLeft();
    }
    else
    {
        escapee.stopXMovement();
    }

    //Down
    if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 20)
    {
        escapee.goDown();
    }
    //Up
    else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -20)
    {
        escapee.goUp();
    }
    else
    {
        escapee.stopYMovement();
    }

    if(game_play_state == GameplayStates::END)
    {
        if (can_leave)
        {
          if (controller.checkButtonPressed(0))
          {
            game->changeScene(Scenes::MAIN_MENU);
          }
        }
    }
}

void Scene_Escapee::setToTile(sf::Sprite& sprite, int x, int y)
{
  // get tile mid
  float tile_mid_x = tilemap.getTile(x, y)->getSprite()->getPosition().x + tilemap.getTileWidth() / 2;
  float tile_mid_y = tilemap.getTile(x, y)->getSprite()->getPosition().y + tilemap.getTileHeight() / 2;

  // set escapee pos
  sprite.setPosition(tile_mid_x - escapee.getWidth() / 2,
                     tile_mid_y - escapee.getHeight() / 2);
}

void Scene_Escapee::handleEscapeeCamera()
{
  if (
    escapee.getSprite()->getPosition().x +
        escapee.getSprite()->getLocalBounds().width / 2 < 1080 / 2 &&
    escapee.getSprite()->getPosition().y +
        escapee.getSprite()->getLocalBounds().height / 2 < 720 / 2)
  {
      setCamera(1);
  }
  else if (
    escapee.getSprite()->getPosition().x +
        escapee.getSprite()->getLocalBounds().width / 2 > 1080 / 2 &&
    escapee.getSprite()->getPosition().y +
        escapee.getSprite()->getLocalBounds().height / 2 < 720 / 2)
  {
      setCamera(2);
  }
  else if (
    escapee.getSprite()->getPosition().x +
        escapee.getSprite()->getLocalBounds().width / 2 < 1080 / 2 &&
    escapee.getSprite()->getPosition().y +
        escapee.getSprite()->getLocalBounds().height / 2 > 720 / 2)
  {
      setCamera(3);
  }
  else if (
    escapee.getSprite()->getPosition().x +
        escapee.getSprite()->getLocalBounds().width / 2 > 1080 / 2 &&
    escapee.getSprite()->getPosition().y +
        escapee.getSprite()->getLocalBounds().height / 2 > 720 / 2)
  {
      setCamera(4);
  }
}

void Scene_Escapee::setCamera(int id)
{
  camera_id = id;
  switch (id)
  {
      case 1:
      {
        //Top left
        view = sf::View(sf::FloatRect(0, 0, 1080 * 0.517, 720 * 0.524));
        current_camera = 1;
        break;
      }
      case 2:
      {
        //Top right
        view = sf::View(sf::FloatRect(1080 * 0.483, 0, 1080 * 0.517, 720 * 0.524));
        current_camera = 2;
        break;
      }
      case 3:
      {
        //Bottom left
        view = sf::View(sf::FloatRect(0, 720 * 0.476, 1080 * 0.517, 720 * 0.524));
        current_camera = 3;
        break;
      }
      case 4:
      {
        //Bottom right
        view = sf::View(sf::FloatRect(1080 * 0.483, 720 * 0.476, 1080 * 0.517, 720 * 0.524));
        current_camera = 4;
        break;
      }
      default:
      {
        //Do nothing
      }
  }

  //Move camera to view
  window.setView(view);
}

void Scene_Escapee::checkForEscapee()
{
  if (cursor.getGlobalBounds().intersects(escapee.getSprite()->getGlobalBounds()))
  {
    //std::cout<< "Escapee Found" << std::endl;
    setGameEnd("You Lose");
  }
}

void Scene_Escapee::handleCursor(float dt)
{
  if (controller.joystick_moved)
  {
    // move cursor
    cursor.move(cursor_speed * controller.joystick_position.x * dt,
                cursor_speed * controller.joystick_position.y * dt);
  }

  float view_left = view.getCenter().x - view.getSize().x / 2;
  float view_right = view.getCenter().x + view.getSize().x / 2;
  float view_top = view.getCenter().y - view.getSize().y / 2;
  float view_bottom = view.getCenter().y + view.getSize().y / 2;

  // make sure cursor won't go out of view
  if (cursor.getPosition().x < view_left)
  {
    cursor.setPosition(view_left, cursor.getPosition().y);
  }

  if (cursor.getPosition().x + cursor.getSize().x > view_right)
  {
    cursor.setPosition(view_right - cursor.getSize().x, cursor.getPosition().y);
  }

  if (cursor.getPosition().y < view_top)
  {
    cursor.setPosition(cursor.getPosition().x, view_top);
  }

  if (cursor.getPosition().y + cursor.getSize().y > view_bottom)
  {
    cursor.setPosition(cursor.getPosition().x, view_bottom - cursor.getSize().y);
  }
}

void Scene_Escapee::mouseClicked(Game* game, sf::Event event)
{
  sf::Vector2i click = sf::Mouse::getPosition(window);
  //setGameEnd("Game End");

}
void Scene_Escapee::textEntered(Game* game, sf::Event event)
{

}

void Scene_Escapee::setGameEnd(std::string message)
{
  game_play_state = GameplayStates::END;
  end_screen->setEndMessage(std::move(message));
  end_screen->startFadeIn();
  client->stopClient(5);


}

void Scene_Escapee::joystickMoved(Game* game, sf::Event event)
{

}
