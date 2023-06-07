#pragma once

#include "SceneBase.h"
#include "../Objects/FadeInOut.h"
#include "../Objects/TextBox.h"
#include "../Tools/Lerp.h"
#include "../Tools/UiEnumsCycle.h"



#ifndef SFMLGAME_MAINMENU_H
#define SFMLGAME_MAINMENU_H

enum class GameStates{CHOOSING, CHOOSING_CHARACTER, SCENE_TRANSITION, CHOICE_TRANSITION, TYPING_IP};

class Scene_MainMenu : public SceneBase
{
 public:
  Scene_MainMenu(sf::RenderWindow& game_window, sf::Music& game_music);
  ~Scene_MainMenu() override;

  //Functions
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

  void IpGUIHandler(Game* game);
  void IpInputHandlder(Game* game, sf::Event event);

  std::string new_ip_string;


  bool is_loaded = false;

  bool joystick_moving = false;
  bool pressed = false;

 private:
  //Global references
  sf::RenderWindow& window;
  sf::Music& music;

  //Game State
  GameStates game_state = GameStates::CHOOSING;


  Vector2 title_end_pos;
  Vector2 host_end_pos;
  Vector2 connect_end_pos;
  Vector2 quit_end_pos;

  Vector2 camera_target_pos =  Vector2(540,360);
  float camera_lerp_speed = 1;

  bool ip_selected = false;

  //UI
  MenuButtons selected_button = MenuButtons(0);
  CharacterButtons selected_character_button = CharacterButtons(0);
  UiEnumsCycle mainmenu_option_buttons = UiEnumsCycle(3);
  UiEnumsCycle character_option_buttons = UiEnumsCycle(2);

  //Camera
  sf::View camera = sf::View(sf::FloatRect(0,0,1080,720));

  //Objects
  std::unique_ptr<TextBox> host_button;
  std::unique_ptr<TextBox> connect_button;
  std::unique_ptr<TextBox> quit_button;
  std::unique_ptr<TextBox> volume_warning;
  std::unique_ptr<TextBox> escapee_choice;
  std::unique_ptr<TextBox> guard_choice;
  std::unique_ptr<TextBox> ip_address_text;

  std::unique_ptr<sf::RectangleShape> ip_address_input_box;

  std::unique_ptr<FadeInOut> fade_in_out;

  std::unique_ptr<sf::Texture> background_texture;
  std::unique_ptr<sf::Sprite> background;
  std::unique_ptr<sf::Texture> title_texture;
  std::unique_ptr<sf::Sprite> title;



  //Next Scene
  Scenes next_scene;





};

#endif // SFMLGAME_MAINMENU_H
