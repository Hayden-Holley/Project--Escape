//
// Created by h2-holley on 10/03/2023.
//

#include "Server.h"
void Server::init()
{
  std::cerr << "new server" << std::endl;
  // because the listener is a unique pointer we should add a check to make sure it will all work
  if (listener == NULL)
  {
    listener = std::make_unique<sf::TcpListener>();
  }
  // open and bind the listener to a port
  // this lets the listener know where the data is coming from
  if (listener->listen(53000) != sf::Socket::Done)
  {
    std::cerr << "listener error" << std::endl;
    // error...
  }

}
void Server::run()
{
  while(running)
  {

    std::cerr << "server running" << std::endl;
    // add pointer for new_connection and get reference to it
    sf::TcpSocket& cSock =
      connections.emplace_back(std::make_unique<sf::TcpSocket>()).operator*();

    // when we get a new_connection (Socket)
    // if the new_connection is invalid - return
    if (listener->accept(cSock) != sf::Socket::Done)
    {
      // remove the invalid connection
      connections.pop_back();
      return;
    }
    // add new_connection to connections
    // set up the connection for listening & sending

    std::cout << "Client connected @ " << cSock.getRemotePort() << std::endl;

    // created a new thread in the workers vector which will run the listen function
    workers.emplace_back(
      [&]
      {
        // listens for messages on the connected socket
        listen(cSock);
        // we know the socket disconnected, so we remove it from the list
        std::cout << "detected disconnect\n";
        std::lock_guard<std::mutex> lck(mutex); // guard access to socket list

        // we go through all connections and erase the one that disconnected.
        // we lock the mutex as soon as we detect the disconnect to make sure that... we don't try and send a message to a disconnected socket.
        for (int i = 0; i < connections.size(); ++i)
        {
          // check if the connection matches else move onto the next one
          if (connections[i]->getLocalPort() == cSock.getLocalPort())
            continue;

          // erase the i-th element of the connections (the correct one)
          connections.erase(std::next(connections.begin(), i));

          // the vector has been changed, so we need to break out of the for loop
          break;
        }
      });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  std::cerr << "server stopped" << std::endl;
}
void Server::listen(sf::TcpSocket& cSocket)
{
  // keep client socket active until connection is no longer valid
  // so the server can listen to all the connected clients and act on them
  bool continue_receiving = true;

  sf::Packet packet;

  while (continue_receiving)
  {

    // is there data ready for us?
    auto status = cSocket.receive(packet);
    int received_port = cSocket.getRemotePort();

    if (status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false; // client disconnected
      std::cout << "Disconnect " << std::endl;
    }

    // add a null terminator, and print as string
    send(packet, received_port);
  }
  cSocket.disconnect();
}
void Server::send(sf::Packet packet, int port)
{
  // locks the mutex so that the message can be sent to everyone.
  // this method locks the mutex until it leaves the scope, so in this case...
  // we don't need to explicitly unlock it.
  std::lock_guard<std::mutex>lck(mutex);

  int i = 0;
  for (auto & connection:connections)
  {
    if (connections[i]->getRemotePort() != port)
    {
      connection->send(packet);
    }
    i++;
  }
}

Server::~Server()
{
  listener.reset();
  is_end = true;
  running = false;
  std::cerr << "server deleted" << std::endl;
}

bool Server::getRunning()
{
  return running;
}

void Server::end()
{
  running = false;
  is_end = true;
  std::cerr << "server ended" << std::endl;
}
