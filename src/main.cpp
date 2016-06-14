#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "board.h"
#include "pawn.h"
#include "move.h"
#include "chess_game.h"
using namespace std;


int main(){

//  for(int i = 0; i < 8; i++){
//    for(int k = 0; k < 8; k++){
//      cout<<"P ";
//    }
//    cout<<endl;
//  }
//
 /* ChessBoard b;
  //cout<<b.board[3][3];
  b.print();
  Pawn p;
  cout<<p.getId()<<endl;
  Pawn p2;
  cout<<p2.getId()<<endl;

  Move m;
  m.updateString("7B3h");
  m.parseString();
  cout<<m.curr_x<<endl;
  cout<<m.curr_y<<endl;*/
  ChessGame c;
  c.run();
  return 0;
}