#include "chess_game.h"
#include <iostream>
using namespace std;

void ChessGame::run(){


  cBoard.print();

  
  cout<<"Enter a move: "<<endl;
  string move;
  //move = getMoveInput();
  while(cin >> move){
    //cout<<"move is: "<<move<<endl;
    int check = checkForCommands(move);
    //cout<<"check is: "<<check<<endl;
    if(!check) break;
    else{
      moveObject.updateString(move);
      int parse = moveObject.parseString();
      if(parse == 0){
        bool check_move = moveObject.checkMove(cBoard);

        if(check_move){

          ChessBoard *b = new ChessBoard(cBoard);
          int cur_x = moveObject.getCur_x();
          int cur_y = moveObject.getCur_y();
          int move_x = moveObject.getMove_x();
          int move_y = moveObject.getMove_y();
          b->movePiece(cur_x, cur_y, move_x, move_y);
          checkFlags();
          if(moveObject.getCheckStatus()){
            if(moveObject.isStillCheck(*b)){
              cout<<"You are still in check, make a new move"<<endl;
              continue;
            }
          }
          delete b;
          cBoard.movePiece(cur_x, cur_y, move_x, move_y);
          if(moveObject.check(cBoard)) cout<<"You are in check"<<endl;
          moveObject.changeTurns();
        }

      }
    }
      cBoard.print();
  cout<<"Enter a move: "<<endl;
    
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

void ChessGame::checkFlags(){
  if(moveObject.enpassantFlag()){
    if(moveObject.getTurn()){//white turn
      cBoard.capture(moveObject.getMove_x() + 1, moveObject.getMove_y());
    }
    else{//black turn
      cBoard.capture(moveObject.getMove_x() - 1, moveObject.getMove_y());

    }
  }
}