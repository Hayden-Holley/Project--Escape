//
// Created by Hayden on 28/03/2023.
//
#include "PacketOverrides.h"

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f& vector2F)
{
  return packet << vector2F.x << vector2F.y;
}

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& vector2F)
{
  return packet >> vector2F.x >> vector2F.y;
}