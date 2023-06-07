//
// Created by h2-holley on 10/03/2023.
//

#ifndef SFMLGAME_SERVER_H
#define SFMLGAME_SERVER_H
#include <SFML/Network.hpp>
#include <thread>
#include <list>
#include <iostream>
#include <mutex>
#include <memory>

class Server
{
 public:
  ~Server();
  void init();
  void run();
  void listen(sf::TcpSocket& cSocket);
  void send(sf::Packet packet, int port);

  void end();

  bool is_end = false;

  bool getRunning();

 private:
  // a vector of threads to let certain processes of the server run at the same time
  std::vector<std::thread> workers;
  std::vector<std::unique_ptr<sf::TcpSocket>> connections;

  std::unique_ptr<sf::TcpListener> listener;
  std::unique_ptr<sf::TcpSocket> socket;

  std::mutex mutex;
  bool running = true;
};

#endif // SFMLGAME_SERVER_H
