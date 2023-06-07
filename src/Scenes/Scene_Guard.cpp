//
// Created by Cameron on 28/04/2023.
//

#include "Scene_Guard.h"
#include "../Game.h"

Scene_Guard::Scene_Guard(sf::RenderWindow& game_window, sf::Music& game_music) : window(game_window), music(game_music)
{

}
Scene_Guard::~Scene_Guard()
{



}

bool Scene_Guard::init(Game* game)
{
  //---Networking---
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

  escapeeLastPosition = escapee.getSprite()->getPosition();
  escapeeLastState = escapee.getCharacterState();
  escapeeLastFacing = escapee.getCharacterFacing();

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

  setCamera(1);

  // guard
  guard_view_blocker.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
  guard_view_blocker.setFillColor(sf::Color::Black);

  // cursor
  cursor.setSize(sf::Vector2f(10, 10));
  cursor.setFillColor(sf::Color::White);
  cursor.setPosition(view.getSize().x / 2 - cursor.getGlobalBounds().width / 2,
                     view.getSize().y / 2 - cursor.getGlobalBounds().height / 2);


  end_screen = std::make_unique<EndScreen>(window);

  return true;
}

bool Scene_Guard::shutDown()
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

  return true;
}

void Scene_Guard::keyPressed(Game* game, sf::Event event)
{
  if(game_play_state == GameplayStates::GAMEPLAY)
  {
    // hide cursor
    cursor_shown = false;

    if (
      (event.key.code == sf::Keyboard::Num1) ||
      (event.key.code == sf::Keyboard::Num2) ||
      (event.key.code == sf::Keyboard::Num3) ||
      (event.key.code == sf::Keyboard::Num4))
    {
      // Set current camera
      // Top left = 0, 0 Top right = 1080 / 2, 0 Bottom left = 0, 720 / 2 Bottom right = 1080 / 2, 720 / 2 27, 28, 29, 30 = Keys 1, 2, 3, and 4 so minus 26 to normalise it
      setCamera(event.key.code - 26);
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

void Scene_Guard::keyReleased(Game* game, sf::Event event)
{

}

void Scene_Guard::joystickInput(Game* game, sf::Event event)
{


  // show cursor
  cursor_shown = true;

  // Scroll through cameras
  // Left bumper = 4, Right bumper = 5
  if (controller.checkButtonPressed(4))
  {
    if (!controller.camera_switch_pressed)
    {
      // Go back a camera and if camera = 0 return to camera 4
      current_camera -= 1;
      if (current_camera < 1)
      {
        current_camera = 4;
      }
      setCamera(current_camera);
      controller.camera_switch_pressed = true;
    }
  }

  else if (controller.checkButtonPressed(5))
  {
    if (!controller.camera_switch_pressed)
    {
      // Go forward a camera and if camera = 5 return to camera 1
      current_camera += 1;
      if (current_camera > 4)
      {
        current_camera = 1;
      }
      setCamera(current_camera);
      controller.camera_switch_pressed = true;
    }
  }

  else
  {
    controller.camera_switch_pressed = false;
  }

  if(game_play_state == GameplayStates::GAMEPLAY)
  {
    // select player
    if (controller.checkButtonPressed(2))
    {
      checkForEscapee();
    }
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

void Scene_Guard::update(Game* game)
{
  end_screen->setPosition(view.getCenter().x, view.getCenter().y);
  end_screen->update(game->delta);

  handleCursor(game->delta);


  if (game_play_state == GameplayStates::GAMEPLAY)
  {
    escapee.update(game->delta, tilemap);

    for (int i = 0; i < BOT_AMOUNT; ++i)
    {
      bots[i]->updateBot(game->delta, tilemap);
    }



    handleGuardSickness(game->delta);

    // Checking to see if Escapee's position packet has been received
    if (client->hasEscapeePositionPacketBeenReceived)
    {
      // Update the Escapee's position onto Guard's screen
      escapee.getSprite()->setPosition(client->escapeePositionReceived);
      client->thread_designation = PacketTags::none;
      // Boolean change back to false so there isn't a loop
      client->hasEscapeePositionPacketBeenReceived = false;
      if (escapee.getSprite()->getPosition().x > window.getSize().x)
      {
        //std::cout << "Escapee has escaped" << std::endl;
        setGameEnd("You Lose");
        client->stopClient(5);
      }
    }

    // Checking to see if Escapee's state packet has been received
    if (client->hasEscapeeStatePacketBeenReceived)
    {
      // Update the Escapee's state onto Guard's screen
      switch (client->escapee_state_received)
      {
        case 0:
          escapee.setCharacterState(CharacterState::IDLE);
          break;
        case 1:
          escapee.setCharacterState(CharacterState::WALKING);
          break;
      }

      // Update the Escapee's state onto Guard's screen
      // player.setCharacterState(client->escapee_state_received);
      // Boolean change back to false so there isn't a loop
      client->thread_designation                = PacketTags::none;
      client->hasEscapeeStatePacketBeenReceived = false;
    }

    // Checking to see if Escapee's facing packet has been received
    if (client->hasEscapeeFacingPacketBeenReceived)
    {
      switch (client->escapee_facing_received)
      {
        case 0:
          escapee.setCharacterFacing(CharacterFacing::UP);
          break;
        case 1:
          escapee.setCharacterFacing(CharacterFacing::DOWN);
          break;
        case 2:
          escapee.setCharacterFacing(CharacterFacing::LEFT);
          break;
        case 3:
          escapee.setCharacterFacing(CharacterFacing::RIGHT);
          break;
      }
      // Update the Escapee's facing onto Guard's screen
      // player.setCharacterFacing(client->escapee_facing_received);
      // Boolean change back to false so there isn't a loop
      client->thread_designation                 = PacketTags::none;
      client->hasEscapeeFacingPacketBeenReceived = false;
    }

  }
  else if (game_play_state == GameplayStates::END)
  {
    if(!client->close)
    {
      checkForEscapee();
    }
    if(client->disconnected)
    {
      can_leave = true;
      end_screen->startContinue();
    }
  }
}

void Scene_Guard::render(Game* game)
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

  window.draw(guard_view_blocker);

  if (cursor_shown)
  {
    window.draw(cursor);
  }

  //End screen
  end_screen->render();

}

void Scene_Guard::setCamera(int id)
{
  camera_id = id;

  sf::Vector2f cursor_relative_pos = view.getCenter() - sf::Vector2f(cursor.getPosition().x, cursor.getPosition().y);

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

  // move cursor to new view
  cursor.setPosition(view.getCenter() - cursor_relative_pos);
}

void Scene_Guard::handleGuardSickness(float dt)
{
  float percentage;
  sf::Color view_blocker_color;

  if (camera_id == previous_camera_id)
  {
    if (view_blocker_timer < view_blocker_timer_default)
    {
      view_blocker_timer += 1 * dt;
    }

    percentage = (view_blocker_timer / view_blocker_timer_default) * 255;
  }
  else
  {
    view_blocker_timer = 0;

    percentage = 0;
  }

  previous_camera_id = camera_id;

  view_blocker_color = sf::Color(0, 0, 0, percentage);
  guard_view_blocker.setFillColor(view_blocker_color);
}

void Scene_Guard::setToTile(sf::Sprite& sprite, int x, int y)
{
  // get tile mid
  float tile_mid_x = tilemap.getTile(x, y)->getSprite()->getPosition().x + tilemap.getTileWidth() / 2;
  float tile_mid_y = tilemap.getTile(x, y)->getSprite()->getPosition().y + tilemap.getTileHeight() / 2;

  // set escapee pos
  sprite.setPosition(tile_mid_x - escapee.getWidth() / 2,
                     tile_mid_y - escapee.getHeight() / 2);
}


void Scene_Guard::checkForEscapee()
{

  if (client->hasEscapeeBeenCaught)
  {
    client->stopClient(5);
  }
  if (cursor.getGlobalBounds().intersects(
        escapee.getSprite()->getGlobalBounds()))
  {
    std::cout << "Escapee Found" << std::endl;
    client->guard_win_condition_to_send = 1;
    client->thread_designation          = PacketTags::guardwincondition;
    client->hasEscapeeBeenCaught        = true;
    setGameEnd("You Win!");
    if (client->connected)
    {
      std::cout << "H" << std::endl;
      client->stopClient(5);
    }
  }

}

void Scene_Guard::handleCursor(float dt)
{


  //if (controller.joystick_moved)
  //{
    // move cursor
    cursor.move(cursor_speed * controller.joystick_position.x * dt,
                cursor_speed * controller.joystick_position.y * dt);
  //}

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

void Scene_Guard::mouseClicked(Game* game, sf::Event event)
{
  sf::Vector2i click = sf::Mouse::getPosition(window);
//  // select player
//  if (controller.checkButtonPressed(0))
//  {
//    //checkForEscapee();
//  }

  //setGameEnd("Game End");
}

void Scene_Guard::textEntered(Game* game, sf::Event event)
{

}

void Scene_Guard::setGameEnd(std::string message)
{
  game_play_state = GameplayStates::END;
  end_screen->setEndMessage(std::move(message));
  end_screen->startFadeIn();


}

void Scene_Guard::joystickMoved(Game* game, sf::Event event)
{

  controller.checkAxisMove();


}
