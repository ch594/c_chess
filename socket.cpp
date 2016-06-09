#include "socket.h"
#include <SFML/Network.hpp>
#include <iostream>


cMessage::cMessage(bool server = true) :server(server) {
  if(server){
    serverInit();
  }
  else{
    clientInit();
  }

} 


void cMessage::serverInit(){

  sf::TcpListener listener;
  int port = 5001;
  listener.listen(port);
  listener.accept(socket);
  cout<<"Connection established with client"<<endl;


}

void cMessage::clientInit(){

  string ip_address("");
  int port;
  cout <<"Enter the IP Address and port"<<endl;
  cout<<"IP Address: "<<endl;
  cin >> ip_address;
  cout <<"Port: "<<endl;
  cin >> port;
  cout <<"port is: " << port << endl;
  if(socket.connect(ip_address, port) == sf::Socket::Done){
    cout <<"Connected with server"<<endl;
  }
  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

}

void cMessage::sendMessage(string message){
  sf::Packet packet;
  packet << message;
  socket.send(packet);
}

string cMessage::receiveMessage(){
  sf::Packet packet;
  socket.receive(packet);
  string message;
  packet >> message;
  return message;
}

string cMessage::getInput(){

  string s;
  cout<<"Enter input: ";
  getline(cin, s);  
  return s;

}