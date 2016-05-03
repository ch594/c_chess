#include "chess_game.h"
#include <iostream>
using namespace std;

void ChessGame::run(){
  while(1){
    cBoard.print();
    cout<<"Enter a move: "<<endl;
    string move;
    move = getMoveInput();
    //cout<<"move is: "<<move<<endl;
    int check = checkForCommands(move);
    //cout<<"check is: "<<check<<endl;
    if(!check) break;
    else{
      moveObject.updateString(move);
      int parse = moveObject.parseString();
      cout<<"parse result is: "<<parse<<endl;
      if(parse == 0){
        bool check_move = moveObject.checkMove(cBoard);
        cout<<"check move is: "<<check_move<<endl;

        if(check_move){

          int cur_x = moveObject.getCur_x();
          int cur_y = moveObject.getCur_y();
          int move_x = moveObject.getMove_x();
          int move_y = moveObject.getMove_y();
          cBoard.movePiece(cur_x, cur_y, move_x, move_y);
        }

      }
    }
  }
}


string ChessGame::getMoveInput(){
  string temp;
  cin >> temp;
  return temp;
}

int ChessGame::checkForCommands(string &input){
  if(input == "quit" || input == "q") return 0;
  else return 1;
}