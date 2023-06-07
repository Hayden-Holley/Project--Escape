#include "Client.h"
void Client::connect()
{
  socket = std::make_unique<sf::TcpSocket>();
  if(socket->connect("127.0.0.1", 53000) == sf::Socket::Status::Done)
  {
    std::cout << "You're Connected!" << std::endl;
    std::cout << "Type a display name" << std::endl;
    outputtingMessages("Type a display name");
    awaitingInput();
    displayName = receivedInput;
    receivedInput.clear();
    outputtingMessages("You're Connected!");

    connected = true;
  }
  else
  {
    std::cout << "failed to connect";
    outputtingMessages("failed to connect");
  }
}

void Client::input(sf::TcpSocket& iSocket) //const
{
  while (running)
  {
    // grab the user input and store as a string
    awaitingInput();
    std::string str = displayName + ": " + receivedInput;//text;
    receivedInput.clear();


    // is the client still connected?
    if (connected)
    {
      // send the string and cross your fingers
      // tell the reference to the socket to send the input_string as a char array
      // and how many characters are in the string
      iSocket.send(reinterpret_cast<char*>(str.data()), str.length());
    }
  }
}

void Client::run()
{
  // set the running bool
  running = true;
  // creating a thread that will start the input reading
  // the input_thread is detached so that it runs asynchronously
  std::thread input_thread ([&]{
                             input(*socket);
                            });

  input_thread.detach();

  // receive messages
  while (running && connected)
  {
    // the static buffer is where the received messages are stored
    char static_buffer[1028];
    while (connected)
    {
      std::size_t received;
      auto status = socket->receive(static_buffer, 1028, received);

      if (status == sf::Socket::Status::Disconnected)
      {
        connected = false;
        std::cout << "clean disconnection" << std::endl;
        outputtingMessages("clean disconnection");
        socket->disconnect();
        break;
      }

      else
      {
        // treat all data as string and append null terminator
        if (received < 1028) static_buffer[received] = '\0';
        std::cout << static_buffer << '\n';
        outputtingMessages(static_buffer);
      }
    }
  }
}

void Client::outputtingMessages(std::string outputMessage)
{
  for (int i = sizeof(messagesToOutput)/sizeof(std::string)-1; i > -1; i--)
  {
    if (i != 0)
    {
      messagesToOutput[i] = messagesToOutput[i-1];
    }
    else
    {
      messagesToOutput[i] = outputMessage;
    }
  }
}

void Client::awaitingInput()
{
  while (receivedInput.empty())
  {
    if (!receivedInput.empty())
    {
      continue;
    }
  }
}
