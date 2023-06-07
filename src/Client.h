#ifndef CHATROOM_CLIENT_H
#define CHATROOM_CLIENT_H
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <atomic>


class Client
{
 public:
  void connect();
  void input(sf::TcpSocket& iSocket); //const;
  void run();
  void outputtingMessages(std::string outputMessage);
  void awaitingInput();

  std::atomic<bool> running = false;
  std::atomic<bool> connected = false;

  std::string displayName;
  std::string receivedInput;
  std::string messagesToOutput[8];

 private:
  std::unique_ptr<sf::TcpSocket> socket;
};
#endif // CHATROOM_CLIENT_H
