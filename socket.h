#ifndef SOCKET_H
#define SOCKET_H

#include <SFML/Network.hpp>
#include <string>
using namespace std;

class cMessage{
public:

  cMessage(bool server);
  void sendMessage(string message);
  string receiveMessage();
  string getInput();

private:
  bool server;
  sf::TcpSocket socket;
  void clientInit();
  void serverInit();
};

#endif