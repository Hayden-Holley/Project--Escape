//
// Created by h2-holley on 10/03/2023.
//

#ifndef SFMLGAME_CLIENT_H
#define SFMLGAME_CLIENT_H
#include "PacketOverrides.h"
#include "Character.h"
#include <SFML/Network.hpp>
#include <atomic>
#include <iostream>
#include <thread>

enum class PacketTags {none=0, string=1, vector2f=2, characterstate=3, characterfacing=4, guardwincondition=5};
class Client
{
 public:
  ~Client();
  void connect(sf::String ip_address);
  void input(sf::TcpSocket& iSocket); //const;
  void run();
  bool stopClient(int code);

  bool onDisconnect();

  std::atomic<bool> disconnected = false;
  std::atomic<bool> close = false;

  std::atomic<bool> running = false;
  std::atomic<bool> connected = false;

  //Variables for Escapee Position Packets
  sf::Vector2f escapeePositionToSend;
  sf::Vector2f escapeePositionReceived;
  bool hasEscapeePositionPacketBeenReceived = false;
  bool hasEscapeePositionChanged = false;

  //Variables for Escapee Animation State Packets
  int escapee_state_to_send = -1;
  int escapee_state_received = -1;
  bool hasEscapeeStatePacketBeenReceived = false;
  bool hasEscapeeStateChanged = false;

  //Variables for Escapee Animation Facing Packets
  int escapee_facing_to_send = -1;
  int escapee_facing_received = -1;
  bool hasEscapeeFacingPacketBeenReceived = false;
  bool hasEscapeeFacingChanged = false;

  //Variables for Guard win condition
  int guard_win_condition_to_send = -1;
  int guard_win_condition_received = -1;
  bool hasEscapeeBeenCaught = false;
  bool hasEscapeeCaughtPacketBeenReceived = false;

  PacketTags tag;
  PacketTags thread_designation = PacketTags::none;
  int tagID = 0;

 private:
  std::unique_ptr<sf::TcpSocket> socket;
};

#endif // SFMLGAME_CLIENT_H
