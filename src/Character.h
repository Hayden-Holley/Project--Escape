
#ifndef SFMLGAME_CHARACTER_H
#define SFMLGAME_CHARACTER_H

#include "GameObject.h"
#include "cmath"

#include "Tilemap.h"

enum class CharacterState {IDLE, WALKING};
enum class CharacterFacing {LEFT, RIGHT, UP, DOWN};

class Character : public GameObject
{
 public:
  // function 1
  // Updates the character's position and state every frame.
  void update(float dt, Tilemap& map);

  // function 2
  // Initiates or stops movement in the corresponding direction or axis.
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  void stopXMovement();
  void stopYMovement();

  // function 3
  // Returns true if the character is moving in the corresponding direction.
  bool goingLeft();
  bool goingUp();

  // function 4
  // Sets the character's state to the provided CharacterState enum value.
  // The state determines the character's current behavior and abilities in the game world.
  void setCharacterState(CharacterState state);

  // function 5
  void manageCollision(Tilemap& map);
  
  CharacterState getCharacterState();
  CharacterFacing getCharacterFacing();
  void setCharacterFacing(CharacterFacing facing);

  void resetTextureRect();

  // collider
  float collider_width;
  float collider_height;
  float collider_reach;

  //Needed for button collision
  bool isEscapee;
  int buttonsHit;

  bool actual_escapee = false;

 protected:
  sf::Vector2i dir = sf::Vector2i(0, 0);

  bool moving_x;
  bool moving_y;

  bool going_left;
  bool going_up;

 private:
  void move(float dt);
  void animate();
  void animateIdle();
  void animateWalking();
  void updateButtons(Tilemap& map, Tile& tile);

  int speed = 200;

  // character state
  CharacterState character_state = CharacterState::IDLE;
  CharacterFacing character_facing = CharacterFacing::DOWN;

  // texture amounts
  const int IDLE_TEXTURE_AMOUNT = 1;
  const int WALK_TEXTURE_AMOUNT = 6;

  int current_texture_amount;

  // frame size
  const int frame_width = 32;
  const int frame_height = 32;

  // animation
  const int animation_speed = 100; // milliseconds
  int current_frame_left = 0;
  sf::IntRect frame_rect{0, 0, frame_width, frame_height};
  int animation_column = 0; // would only work with the current sprite sheet

  sf::Clock animation_clock;
};

#endif // SFMLGAME_CHARACTER_H
