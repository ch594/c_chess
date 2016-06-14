#include "socket.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>

cMessage::cMessage(bool server = true) :server(server) {
  if(server){
    serverInit();
  }
  else{
    clientInit();
  }

} 


void cMessage::serverInit(){

  listener = new sf::TcpListener;
  int port = 5001;
  listener->listen(port);
  listener->accept(socket);
  cout<<"Connection established with client"<<endl;


}

void cMessage::clientInit(){

  string ip_address;
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
  else{
    cout<<"Unable to connect"<<endl;
    //exit(0);
  }
  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

}

void cMessage::sendMessage(string message){
  sf::Packet packet;
  packet << message;
  if(socket.send(packet) == sf::Socket::Done){
    ;
  }
  else{
    cout<<"Error sending packet"<<endl;
    exit(EXIT_FAILURE);
  }
}

string cMessage::receiveMessage(){
  sf::Packet packet;
  if(socket.receive(packet) == sf::Socket::Done){
    string message;
    packet >> message;
    return message;
    
  }
  else{
    cout<<"Error receiving packet"<<endl;
    exit(EXIT_FAILURE);
  }
}

string cMessage::getInput(){

  string s;
  cout<<"Enter input: ";
  getline(cin, s);  
  return s;

}

cMessage::~cMessage(){
  socket.disconnect();
  if(server){
    listener->close();
    delete listener;
  }
}