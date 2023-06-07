#pragma once

#include "SceneBase.h"
#include "../Objects/FadeInOut.h"

class Scene_Splash : public SceneBase
{
 public:
  Scene_Splash(sf::RenderWindow& game_window, sf::Music& game_music);
  ~Scene_Splash() override;

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

  void StartSceneChange();

  bool is_loaded = false;

 private:
  //Global references
  sf::RenderWindow& window;
  sf::Music& music;

  //Objects
  std::unique_ptr<sf::Texture> splash_background_texture;
  std::unique_ptr<sf::Sprite> splash_background;
  std::unique_ptr<sf::Texture> logo_texture;
  std::unique_ptr<sf::Sprite> logo;

  std::unique_ptr<FadeInOut> fade_in_out;

  sf::Clock clock;

  bool is_start_fade_in = false;

  float logo_scale_target = 1;
  float logo_lerp_speed = 3;

  //Camera
  sf::View camera = sf::View(sf::FloatRect(0,0,1080,720));

  //Next Scene
  bool is_changing_scene = false;
  Scenes next_scene;

};