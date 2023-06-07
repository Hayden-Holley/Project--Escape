#include "Server.h"
void Server::init()
{
  // because the listener is a unique pointer we should add a check to make sure it will all work
  if (listener == nullptr) listener = std::make_unique<sf::TcpListener>();
  // open and bind the listener to a port
  // this lets the listener know where the data is coming from
  if (listener->listen(53000) != sf::Socket::Done)
  {
    // error...
  }
}

// run() is where the magic of the server is taking place.
void Server::run()
{
  while(running)
  {
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
}

void Server::listen(sf::TcpSocket& cSocket)
{
  // keep client socket active until connection is no longer valid
  // so the server can listen to all the connected clients and act on them
  bool continue_receiving = true;

  char data[1028];
  std::size_t received;

  while (continue_receiving)
  {
    // is there data ready for us?
    auto status = cSocket.receive(data, 1028, received);
    int received_port = cSocket.getRemotePort();

    if (status == sf::Socket::Status::Disconnected)
    {
      continue_receiving = false; // client disconnected
      std::cout << "Disconnect " << std::endl;
    }

    // add a null terminator, and print as string
    if (received < 1028)
    {
      data[received] = '\0';
    }
    send(data, received_port);

    std::cout << reinterpret_cast<char*>(data) << '\n';
  }
  cSocket.disconnect();
}

void Server::send(std::string buffer, int port)
{
  // a string can't be sent over the network, so it's converted into a char array
  auto message = reinterpret_cast<char*>(buffer.data());

  // locks the mutex so that the message can be sent to everyone.
  // this method locks the mutex until it leaves the scope, so in this case...
  // we don't need to explicitly unlock it.
  std::lock_guard<std::mutex>lck(mutex);

  int i = 0;
  for (auto & connection:connections)
  {
    connection->send(message, buffer.size());
    i++;
  }
}
