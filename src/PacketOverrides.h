//
// Created by Hayden on 20/04/2023.
//

#ifndef SFMLGAME_PACKETOVERRIDES_H
#define SFMLGAME_PACKETOVERRIDES_H
#include <SFML/Network.hpp>
#include "Character.h"

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2f& vector2F);

sf::Packet& operator >>(sf::Packet& packet, sf::Vector2f& vector2F);

#endif // SFMLGAME_PACKETOVERRIDES_H
