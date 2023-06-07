//
// Created by h2-holley on 10/03/2023.
//

#include "Client.h"
void Client::connect(sf::String ip_address)
{
  while (!close && !connected)
  {
    socket = std::make_unique<sf::TcpSocket>();


    // the IP address needs to be changed, this can be done by asking the escapee to enter the IP of the machine holding the server.
    if (
      socket->connect(sf::IpAddress(ip_address), 53000) ==
      sf::Socket::Status::Done)
    {
      std::cout << "You're Connected!" << std::endl;

      connected = true;
      running = true;
      return;
    }
    else if(!close)
    {
      std::cout << "failed to connect" << std::endl;
    }
  }
  if(close)
  {
    stopClient(1);
    std::cout << "failed to connect - closing" << std::endl;
  }

}



void Client::input(sf::TcpSocket& iSocket)
{
  while (running && !close)
  {
    switch (thread_designation)
    {
      case PacketTags::string:
      {
        // grab the user input and store as a string
        std::string str;
        std::getline(std::cin, str);
        std::cin.clear();
        tag = PacketTags::string;
        sf::Packet packetID;
        sf::Packet strPacket;
        strPacket << str;
        packetID << int(tag);

        // is the client still connected?
        if (connected)
        {
          // send the string and cross your fingers
          iSocket.send(packetID);
          iSocket.send(strPacket);
        }
        break;
      }
      case PacketTags::vector2f:
      {
        sf::Packet escapeePositionPacket;
        sf::Packet packetID;
        if (hasEscapeePositionChanged)
        {
          tag = PacketTags::vector2f;
          //Wrap the Escapee's position in a packet
          escapeePositionPacket << escapeePositionToSend;
          packetID << int(tag);
          //Change the boolean back to false so there isn't a loop
          hasEscapeePositionChanged = false;

          // is the client still connected?
          if (connected)
          {
            // send the vector2f and cross your fingers
            iSocket.send(packetID);
            iSocket.send(escapeePositionPacket);
          }
        }
        break;
      }
      case PacketTags::characterstate:
      {
        sf::Packet escapeeStatePacket;
        sf::Packet packetID;
        if (hasEscapeeStateChanged)
        {
          tag = PacketTags::characterstate;
          //Wrap the Escapee's state in a packet
          escapeeStatePacket << escapee_state_to_send;
          packetID << int(tag);
          //Change the boolean back to false so there isn't a loop
          hasEscapeeStateChanged = false;

          // is the client still connected?
          if (connected)
          {
            // send the character state and cross your fingers
            iSocket.send(packetID);
            iSocket.send(escapeeStatePacket);
          }
        }
        break;
      }
      case PacketTags::characterfacing:
      {
        sf::Packet escapeeFacingPacket;
        sf::Packet packetID;
        if (hasEscapeeFacingChanged)
        {
          tag = PacketTags::characterfacing;
          //Wrap the Escapee's facing in a packet
          escapeeFacingPacket << escapee_facing_to_send;
          packetID << int(tag);
          //Change the boolean back to false so there isn't a loop
          hasEscapeeFacingChanged = false;

          // is the client still connected?
          if (connected)
          {
            // send the character facing and cross your fingers
            iSocket.send(packetID);
            iSocket.send(escapeeFacingPacket);
          }
        }
        break;
      }
      case PacketTags::guardwincondition:
      {
        sf::Packet guardWinPacket;
        sf::Packet packetID;
        if (hasEscapeeBeenCaught)
        {
          tag = PacketTags::guardwincondition;
          guardWinPacket << guard_win_condition_to_send;
          packetID << int(tag);
          hasEscapeeBeenCaught = false;
        }

        // is the client still connected?
        if (connected)
        {
          // send the character facing and cross your fingers
          iSocket.send(packetID);
          iSocket.send(guardWinPacket);
        }
      }
      case PacketTags::none:
      {
        break;
      }
    }
  }
}
void Client::run()
{
  while(!close)
  {
    // set the running bool
    running = true;
    // creating a thread that will start the input reading
    // the input_thread is detached so that it runs asynchronously
    std::thread input_thread([&] { input(*socket); });

    input_thread.detach();

    // receive messages
    while (running && connected)
    {
      while (connected)
      {
        sf::Packet packet;
        auto status = socket->receive(packet);

        if (status == sf::Socket::Status::Disconnected)
        {
          stopClient(1);
          connected = false;
        }

        else
        {
          switch (tagID)
          {
            case 0:
              packet >> tagID;
              break;

            case 1:
            {
              std::string str;
              packet >> str;
              tagID = 0;
              break;
            }

            case 2:
              packet >> escapeePositionReceived;
              hasEscapeePositionPacketBeenReceived = true;
              tagID                                = 0;
              break;

            case 3:
              packet >> escapee_state_received;
              hasEscapeeStatePacketBeenReceived = true;
              tagID                             = 0;
              break;

            case 4:
              packet >> escapee_facing_received;
              hasEscapeeFacingPacketBeenReceived = true;
              tagID                              = 0;
              break;

            case 5:
              packet >> guard_win_condition_received;
              hasEscapeeCaughtPacketBeenReceived = true;
              tagID = 0;
              break;
          }
        }
      }
    }
  }
  disconnected = true;
  std::cerr << "client disconnected" << std::endl;

}

Client::~Client()
{

}

bool Client::stopClient(int code)
{
  socket->disconnect();
  std::cout << "disconneting: " << code << std::endl;
  close = true;
  running = false;
  connected = false;
  return true;
}
