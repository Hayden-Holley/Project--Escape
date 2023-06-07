#include "Scene_MainMenu.h"
#include "../Game.h"
#include "string"


Scene_MainMenu::Scene_MainMenu(sf::RenderWindow& game_window, sf::Music& game_music) : window(game_window), music(game_music)
{
  //camera.setSize(1080, 720);
  window.setView(camera);
  //Title Text setup
  title_texture = std::make_unique<sf::Texture>();
  title_texture->loadFromFile("Data/Sprites/title.png");
  title = std::make_unique<sf::Sprite>();
  title->setTexture(*title_texture);
  title->scale(0.75,0.75);
  title->setOrigin(title->getLocalBounds().width/2, title->getPosition().y);
  title->setPosition(camera.getCenter().x, -400);
  title_end_pos = Vector2(camera.getCenter().x,30);

  background_texture = std::make_unique<sf::Texture>();
  background_texture->loadFromFile("Data/Sprites/mainmenu_background.png");
  background = std::make_unique<sf::Sprite>();
  background->setTexture(*background_texture);
  background->setOrigin(background->getLocalBounds().width/2, background->getLocalBounds().height/2);
  background->setPosition(camera.getCenter().x, camera.getCenter().y);
  background->setScale(0.8,0.8);

  //IP address setup
  ip_address_text = std::make_unique<TextBox>(window, "127.0.0.1");
  ip_address_text->text.setCharacterSize(50);
  ip_address_text->text.setOrigin(ip_address_text->text.getLocalBounds().width/2, ip_address_text->text.getLocalBounds().height/2);
  ip_address_text->text.setPosition(1620, 200);

  ip_address_input_box = std::make_unique<sf::RectangleShape>(sf::Vector2f(ip_address_text->text.getLocalBounds().width + 100, ip_address_text->text.getLocalBounds().height + 10));
  ip_address_input_box->setOrigin(ip_address_input_box->getLocalBounds().width/2, ip_address_input_box->getLocalBounds().height/2);
  ip_address_input_box->setPosition(ip_address_text->text.getPosition().x, 200 +13);
  ip_address_input_box->setFillColor(sf::Color(0,0,0,200));

  //Play button setup
  host_button = std::make_unique<TextBox>(window, "> Play");
  host_button->text.setCharacterSize(50);
  host_button->text.setOrigin(0, host_button->text.getLocalBounds().height/2);
  host_button->text.setPosition(-500, window.getSize().y/2);
  host_end_pos = Vector2(camera.getCenter().x -500, window.getSize().y/2);

  //Connect button setup
  connect_button = std::make_unique<TextBox>(window, "   Join Server");
  connect_button->text.setCharacterSize(50);
  connect_button->text.setOrigin(0, connect_button->text.getLocalBounds().height/2);
  connect_button->text.setPosition(-500, window.getSize().y/2 + 100);
  connect_end_pos = Vector2(camera.getCenter().x -500, window.getSize().y/2 + 100);

  //Quit button setup
  quit_button = std::make_unique<TextBox>(window, "   Quit");
  quit_button->text.setCharacterSize(50);
  quit_button->text.setOrigin(0, quit_button->text.getLocalBounds().height/2);
  quit_button->text.setPosition(-500, window.getSize().y/2 + 200);
  quit_end_pos = Vector2(camera.getCenter().x -500, window.getSize().y/2 + 200);

  //Volume warning
  volume_warning = std::make_unique<TextBox>(window, "Game is probably very loud; turn down the volume of the exe in windows volume mixer");
  volume_warning->text.setCharacterSize(10);
  volume_warning->text.setOrigin(volume_warning->text.getLocalBounds().width/2, volume_warning->text.getLocalBounds().height);
  volume_warning->text.setPosition(camera.getCenter().x, camera.getSize().y - 10);

  //Escapee choice
  escapee_choice = std::make_unique<TextBox>(window, "> Escapee");
  escapee_choice->text.setCharacterSize(50);
  escapee_choice->text.setOrigin(0, escapee_choice->text.getLocalBounds().height/2 );
  escapee_choice->text.setPosition(1080 + 200, camera.getCenter().y + 100);

  //Guard choice
  guard_choice = std::make_unique<TextBox>(window, "   Guard");
  guard_choice->text.setCharacterSize(50);
  guard_choice->text.setOrigin(guard_choice->text.getLocalBounds().width, guard_choice->text.getLocalBounds().height/2);
  guard_choice->text.setPosition((1080*2) - 200, camera.getCenter().y + 100);

  //Fade-in-out
  fade_in_out = std::make_unique<FadeInOut>(window);
  fade_in_out->setOriginCenter();
  fade_in_out->startFadeOut();

  //is_loaded = true;

}

Scene_MainMenu::~Scene_MainMenu()
{
  host_button.reset();
  quit_button.reset();
  fade_in_out.reset();
  volume_warning.reset();
  connect_button.reset();
  guard_choice.reset();
  escapee_choice.reset();
  title.reset();
  title_texture.reset();
  background_texture.reset();
  background.reset();
  ip_address_text.reset();
  ip_address_input_box.reset();

}

bool Scene_MainMenu::init(Game* game)
{
  //game->background.setPosition(camera.getCenter().x, camera.getCenter().y);
  return true;
}

bool Scene_MainMenu::shutDown()
{
  return true;

}

void Scene_MainMenu::keyPressed(Game* game, sf::Event event)
{


  if (game_state == GameStates::CHOOSING)
  {
    if (event.key.code == sf::Keyboard::S)
    {
      selected_button = MenuButtons(mainmenu_option_buttons.next_entry());
    }
    if (event.key.code == sf::Keyboard::W)
    {
      selected_button = MenuButtons(mainmenu_option_buttons.previous_entry());
    }

    if (event.key.code == sf::Keyboard::Enter)
    {

      if (selected_button == MenuButtons::HOSTSERVER)
      {
        game_state = GameStates::SCENE_TRANSITION;
        next_scene = Scenes::SERVER;
        fade_in_out->startFadeIn();
      }
      else if (selected_button == MenuButtons::CONNECTTOSERVER)
      {
        selected_button = MenuButtons::NONE;
        game_state = GameStates::CHOICE_TRANSITION;
        camera_target_pos = Vector2(1620,360);
      }
      else if (selected_button == MenuButtons::QUIT)
      {
        window.close();
      }
    }
  }
  if (game_state == GameStates::CHOOSING_CHARACTER )
  {

    if (event.key.code == sf::Keyboard::A)
    {
      selected_character_button = CharacterButtons(character_option_buttons.previous_entry());
    }
    if (event.key.code == sf::Keyboard::D)
    {
      selected_character_button = CharacterButtons(character_option_buttons.next_entry());
    }

    if (event.key.code == sf::Keyboard::Enter)
    {
      if (selected_character_button == CharacterButtons::ESCAPEE)
      {
        game_state = GameStates::SCENE_TRANSITION;
        next_scene = Scenes::ESCAPEE;
        fade_in_out->startFadeIn();
      }
      else if (selected_character_button == CharacterButtons::GUARD)
      {
        game_state = GameStates::SCENE_TRANSITION;
        next_scene = Scenes::GUARD;
        fade_in_out->startFadeIn();
      }
    }

  }

  if(game_state == GameStates::TYPING_IP)
  {
    if(event.key.code == sf::Keyboard::Enter)
    {
      game_state == GameStates::CHOOSING_CHARACTER;
    }
  }

  IpInputHandlder(game, event);

}

void Scene_MainMenu::keyReleased(Game* game, sf::Event event)
{

}

void Scene_MainMenu::joystickInput(Game* game, sf::Event event)
{
  if (game_state == GameStates::CHOOSING)
  {
    // Down
    if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 20)
    {
      if (!joystick_moving)
      {
        selected_button = MenuButtons(mainmenu_option_buttons.next_entry());
        joystick_moving = true;
      }
    }
    // Up
    else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -20)
    {
      if (!joystick_moving)
      {
        selected_button = MenuButtons(mainmenu_option_buttons.previous_entry());
        joystick_moving = true;
      }
    }
    else
    {
      joystick_moving = false;
    }

    if (sf::Joystick::isButtonPressed(0, 2))
    {
      if (selected_button == MenuButtons::HOSTSERVER)
      {
        game_state = GameStates::SCENE_TRANSITION;
        next_scene = Scenes::SERVER;
        fade_in_out->startFadeIn();
      }
      else if (selected_button == MenuButtons::CONNECTTOSERVER)
      {
        selected_button   = MenuButtons::NONE;
        game_state        = GameStates::CHOICE_TRANSITION;
        camera_target_pos = Vector2(1620, 360);
      }
      else if (selected_button == MenuButtons::QUIT)
      {
        window.close();
      }
      joystick_moving = false;
      pressed = true;
    }
  }

  else if (game_state == GameStates::CHOOSING_CHARACTER )
  {
    // Right
    if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 20)
    {
      if (!joystick_moving)
      {
        selected_character_button = CharacterButtons(character_option_buttons.previous_entry());
        joystick_moving = true;
      }
    }
    // Left
    else if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -20)
    {
      if (!joystick_moving)
      {
        selected_character_button = CharacterButtons(character_option_buttons.next_entry());
        joystick_moving = true;
      }
    }
    else
    {
      joystick_moving = false;
    }

    if (!pressed)
    {
      if (sf::Joystick::isButtonPressed(0, 2))
      {
        if (selected_character_button == CharacterButtons::ESCAPEE)
        {
          game_state = GameStates::SCENE_TRANSITION;
          next_scene = Scenes::ESCAPEE;
          fade_in_out->startFadeIn();
        }
        else if (selected_character_button == CharacterButtons::GUARD)
        {
          game_state = GameStates::SCENE_TRANSITION;
          next_scene = Scenes::GUARD;
          fade_in_out->startFadeIn();
        }
      }
    }
    else
    {
      pressed = false;
    }
  }
}

void Scene_MainMenu::update(Game* game)
{
  camera.setCenter(Lerp::lerp(camera.getCenter().x, camera_target_pos.x, camera_lerp_speed * game->delta), Lerp::lerp(camera.getCenter().y, camera_target_pos.y, camera_lerp_speed * game->delta));

  background->setPosition(camera.getCenter().x, camera.getCenter().y);
  fade_in_out->setPosition(camera.getCenter().x, camera.getCenter().y);

  title->setPosition(
    title->getPosition().x,
    Lerp::lerp(title->getPosition().y, title_end_pos.y, 5 * game->delta));

  host_button->text.setPosition(
    Lerp::lerp(
      host_button->text.getPosition().x, host_end_pos.x, 5 * game->delta),
    host_button->text.getPosition().y);
  connect_button->text.setPosition(
    Lerp::lerp(
      connect_button->text.getPosition().x, connect_end_pos.x, 5 * game->delta),
    connect_button->text.getPosition().y);
  quit_button->text.setPosition(
    Lerp::lerp(
      quit_button->text.getPosition().x, quit_end_pos.x, 5 * game->delta),
    quit_button->text.getPosition().y);

  float background_l_scale = Lerp::lerp(background->getScale().x, 0.5, 0.03 * game->delta);
  background->setScale(background_l_scale, background_l_scale);

  IpGUIHandler(game);

  //---Update UI---
  if (selected_button == MenuButtons::HOSTSERVER)
  {
    {
      host_button->text.setString("> Host Server");
      connect_button->text.setString("   Join Server");
      quit_button->text.setString("   Quit");
    }
  }
  else if (selected_button == MenuButtons::CONNECTTOSERVER)
  {
    host_button->text.setString("   Host Server");
    connect_button->text.setString("> Join Server");
    quit_button->text.setString("   Quit");
  }
  else if (selected_button == MenuButtons::QUIT)
  {
    host_button->text.setString("   Host Server");
    connect_button->text.setString("   Join Server");
    quit_button->text.setString("> Quit");
  }
  if(selected_button == MenuButtons::NONE)
  {
    host_button->text.setString("   Host Server");
    connect_button->text.setString("   Join Server");
    quit_button->text.setString("   Quit");
  }

  if(game_state == GameStates::CHOOSING_CHARACTER)
  {
    if (selected_character_button == CharacterButtons::ESCAPEE)
    {
      escapee_choice->text.setString("> Escapee");
      guard_choice->text.setString("   Guard");
    }
    else if (selected_character_button == CharacterButtons::GUARD)
    {
      escapee_choice->text.setString("   Escapee");
      guard_choice->text.setString("> Guard");
    }
  }
  else
  {
    escapee_choice->text.setString("   Escapee");
    guard_choice->text.setString("   Guard");
  }

  fade_in_out->update(game->delta);

  if (game_state == GameStates::SCENE_TRANSITION)
  {
    if (fade_in_out->opacity >= 255)
    {
      game->changeScene(next_scene);
      return;
    }
  }

  if(game_state == GameStates::CHOICE_TRANSITION)
  {
    game_state = GameStates::CHOOSING_CHARACTER;
  }

}



void Scene_MainMenu::render(Game* game)
{
  window.setView(camera);
  window.draw(*background);
  window.draw(*title);
  quit_button->render();
  connect_button->render();
  host_button->render();
  escapee_choice->render();
  guard_choice->render();
  window.draw(*ip_address_input_box);
  ip_address_text->render();
  //volume_warning->render();


  fade_in_out->render();


}

void Scene_MainMenu::mouseClicked(Game* game, sf::Event event)
{
  sf::Vector2f click = window.mapPixelToCoords(sf::Mouse::getPosition(window));

  if(game_state == GameStates::CHOOSING_CHARACTER)
  {
    if (ip_address_input_box->getGlobalBounds().contains(click.x, click.y))
    {
      new_ip_string = "";
      game_state = GameStates::TYPING_IP;
    }
  }
  else if(game_state == GameStates::TYPING_IP)
  {
    new_ip_string = "";
    game_state = GameStates::CHOICE_TRANSITION;
  }


  //if()

}

void Scene_MainMenu::IpGUIHandler(Game* game)
{

  if (game_state == GameStates::TYPING_IP)
  {
    ip_address_text->text.setString(new_ip_string);
    ip_address_text->text.setFillColor(sf::Color(255,255,255,255));

  }
  else
  {
    ip_address_text->text.setString(game->ip_address);
    ip_address_text->text.setFillColor(sf::Color(100,100,100,255));
  }

  ip_address_text->text.setOrigin(ip_address_text->text.getLocalBounds().width/2, ip_address_text->text.getLocalBounds().height/2);

}

void Scene_MainMenu::IpInputHandlder(Game* game, sf::Event event)
{
  if(game_state == GameStates::TYPING_IP)
  {
    if (event.key.code == sf::Keyboard::Enter)
    {
      game->ip_address = new_ip_string;
      game_state = GameStates::CHOICE_TRANSITION;
    }
    else if (event.key.code == sf::Keyboard::Escape)
    {
      new_ip_string = "";
      game_state = GameStates::CHOICE_TRANSITION;
    }


  }
}

void Scene_MainMenu::textEntered(Game* game, sf::Event event)
{

  if(game_state == GameStates::TYPING_IP)
  {
    if(event.text.unicode == '\b')
    {
      new_ip_string.erase(new_ip_string.size() -1, 1);
    }
    else if (event.type == sf::Event::TextEntered)
    {
      if (event.text.unicode < 128)
      {
        new_ip_string += static_cast<char>(event.text.unicode);
        std::cout << new_ip_string << std::endl;
      }
    }
  }
}

void Scene_MainMenu::joystickMoved(Game* game, sf::Event event)
{

}
