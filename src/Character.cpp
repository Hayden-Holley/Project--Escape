

#include "Character.h"

void Character::update(float dt, Tilemap& map)
{
  move(dt);
  manageCollision(map);
  animate();
}

void Character::move(float dt)
{
  getSprite()->move(speed * dir.x * dt,
                              speed * dir.y * dt);
}

void Character::goLeft()
{
  dir.x = -1;
  going_left = true;
  moving_x = true;
  character_facing = CharacterFacing::LEFT;
  setCharacterState(CharacterState::WALKING);
  animateWalking();
}

void Character::goRight()
{
  dir.x = 1;
  going_left = false;
  moving_x = true;
  character_facing = CharacterFacing::RIGHT;
  setCharacterState(CharacterState::WALKING);
  animateWalking();
}

void Character::goUp()
{
  dir.y = -1;
  going_up = true;
  moving_y = true;
  character_facing = CharacterFacing::UP;
  setCharacterState(CharacterState::WALKING);
  animateWalking();
}

void Character::goDown()
{
  dir.y = 1;
  going_up = false;
  moving_y = true;
  character_facing = CharacterFacing::DOWN;
  setCharacterState(CharacterState::WALKING);
  animateWalking();
}

void Character::stopXMovement()
{
  dir.x = 0;
  moving_x = false;
  animateIdle();

  if (moving_y)
  {
    if (going_up)
    {
      character_facing = CharacterFacing::UP;
    }
    else
    {
      character_facing = CharacterFacing::DOWN;
    }
  }
}

void Character::stopYMovement()
{
  dir.y = 0;
  moving_y = false;
  animateIdle();

  if (moving_x)
  {
    if (going_left)
    {
      character_facing = CharacterFacing::LEFT;
    }
    else
    {
      character_facing = CharacterFacing::RIGHT;
    }
  }
}

bool Character::goingLeft()
{
  return going_left;
}

bool Character::goingUp()
{
  return going_up;
}

void Character::setCharacterState(CharacterState state)
{
  character_state = state;
}

void Character::animate()
{
  if ((!moving_x) &&
      (!moving_y) &&
      actual_escapee)
  {
    character_state = CharacterState::IDLE;
  }

  switch (character_state)
  {
    case CharacterState::IDLE:
    {
      animateIdle();
      break;
    }

    case CharacterState::WALKING:
    {
      animateWalking();
      break;
    }
  }

  frame_rect = {current_frame_left, animation_column * frame_height, frame_width, frame_height};
  if (animation_clock.getElapsedTime().asMilliseconds() > animation_speed)
  {
    // change to next texture. assuming texture sheet moves to the right
    current_frame_left = (current_frame_left + 1) % current_texture_amount;
    frame_rect.left = current_frame_left * frame_width;
    getSprite()->setTextureRect(frame_rect);

    animation_clock.restart();
  }
}

void Character::animateIdle()
{
  current_texture_amount = IDLE_TEXTURE_AMOUNT;

  switch (character_facing)
  {
    case CharacterFacing::LEFT:
    {
      animation_column = 3;
      break;
    }

    case CharacterFacing::RIGHT:
    {
      animation_column = 2;
      break;
    }

    case CharacterFacing::UP:
    {
      animation_column = 1;
      break;
    }

    case CharacterFacing::DOWN:
    {
      animation_column = 0;
      break;
    }
  }
}

void Character::animateWalking()
{
  current_texture_amount = WALK_TEXTURE_AMOUNT;

  switch (character_facing)
  {
    case CharacterFacing::LEFT:
    {
      animation_column = 7;
      break;
    }

    case CharacterFacing::RIGHT:
    {
      animation_column = 6;
      break;
    }

    case CharacterFacing::UP:
    {
      animation_column = 5;
      break;
    }

    case CharacterFacing::DOWN:
    {
      animation_column = 4;
      break;
    }
  }
}

void Character::manageCollision(Tilemap& map)
{
  for (int i = 0; i < map.tilemapWidth(); ++i)
  {
    for (int j = 0; j < map.tilemapHeight(); ++j)
    {
      auto& tile = map.getTile(i, j);

      if (tile->id != 0)
      {
        auto& tile_pos = tile->getSprite()->getPosition();
        auto player_pos = getSprite()->getPosition();

        float tile_mid_x = tile_pos.x + tile->getSprite()->getGlobalBounds().width / 2;
        float tile_mid_y = tile_pos.y + tile->getSprite()->getGlobalBounds().height / 2;

        float character_mid_x = player_pos.x + getSprite()->getGlobalBounds().width / 2;
        float character_mid_y = player_pos.y + getSprite()->getGlobalBounds().height / 2;

        // check if tile is close enough to collide
        if (sqrt(pow((tile_mid_x - character_mid_x), 2) + pow((tile_mid_y - character_mid_y), 2)) < collider_reach)
        {
          // check if colliding
          // escapee left
          if ((character_mid_x - collider_width / 2 < tile_pos.x + map.getTileWidth()) &&
              (character_mid_x - collider_width / 2 > tile_pos.x) &&
              (character_mid_y > tile_pos.y) &&
              (character_mid_y < tile_pos.y + map.getTileHeight()))
          {
            //If the tile is a button and was pressed by the escapee
            if (tile->id == 54)
            {
              if (isEscapee)
              {
                updateButtons(map, *tile);
              }
            }
            else
            {
              getSprite()->setPosition(
                tile_pos.x + map.getTileWidth() + map.getTileWidth() / 2 -
                  getWidth() / 2,
                player_pos.y);
            }
          }

          // escapee right
          if ((character_mid_x + collider_width / 2 > tile_pos.x) &&
              (character_mid_x + collider_width / 2 < tile_pos.x + map.getTileWidth()) &&
              (character_mid_y > tile_pos.y) &&
              (character_mid_y < tile_pos.y + map.getTileHeight()))
          {
            if (tile->id == 54)
            {
              if (isEscapee)
              {
                updateButtons(map, *tile);
              }
            }
            else
            {
              getSprite()->setPosition(
                tile_pos.x - map.getTileWidth() / 2 - getWidth() / 2,
                player_pos.y);
            }
          }

          // escapee top
          if ((character_mid_y - collider_height / 2 < tile_pos.y + map.getTileHeight()) &&
              (character_mid_y - collider_height / 2 > tile_pos.y) &&
              (character_mid_x > tile_pos.x) &&
              (character_mid_x < tile_pos.x + map.getTileWidth()))
          {
            if (tile->id == 54)
            {
              if (isEscapee)
              {
                updateButtons(map, *tile);
              }
            }
            else
            {
              getSprite()->setPosition(
                player_pos.x,
                tile_pos.y + map.getTileHeight() + map.getTileHeight() / 2 -
                  getHeight() / 2);
            }
          }

          // escapee bottom
          if ((character_mid_y + collider_height / 2 > tile_pos.y) &&
              (character_mid_y + collider_height / 2 < tile_pos.y + map.getTileHeight()) &&
              (character_mid_x > tile_pos.x) &&
              (character_mid_x < tile_pos.x + map.getTileWidth()))
          {
            if (tile->id == 54)
            {
              if (isEscapee)
              {
                updateButtons(map, *tile);
              }
            }
            else
            {
              getSprite()->setPosition(
                player_pos.x,
                tile_pos.y - map.getTileHeight() / 2 - getHeight() / 2);
            }
          }
        }
      }
    }
  }
}

void Character::resetTextureRect()
{
  getSprite()->setTextureRect(sf::IntRect(0, 0, 64, 64));
}

void Character::updateButtons(Tilemap& map, Tile& tile)
{
  //Set texture to background and update/check buttonsHit
  tile.getSprite()->setTextureRect(sf::IntRect(0, 0, 64, 64));
  tile.id = 0;
  buttonsHit += 1;

  //Check if last button was hit
  if (buttonsHit == 3)
  {
    //Open door (missing door sprite)
    map.getTile(28, 19)->getSprite()->setTextureRect(sf::IntRect(0, 0, 64, 64));
    map.getTile(28, 19)->id = 0;
  }
}

CharacterState Character::getCharacterState()
{
  return character_state;
}

CharacterFacing Character::getCharacterFacing()
{
  return character_facing;
}

void Character::setCharacterFacing(CharacterFacing facing)
{
  character_facing = facing;
}
