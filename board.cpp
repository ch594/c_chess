#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "board.h"
#include <cassert>
using namespace std;

/*piece encodings
white pawn: 1
white rook: 2
white knight: 3
white bishop: 4
white queen: 5
white king: 6
black pawn: 7
black rook: 8
black knight: 9
black bishop: 10
black queen: 11
black king: 12
empty: 0
*/


void ChessBoard::initBoard(){
  board[0][0] = 8; board[0][7] = 8; //black rook
  board[0][1] = 9; board[0][6] = 9; //black knight
  board[0][2] = 10; board[0][5] = 10; //black bishop
  board[0][3] = 12; //black king
  board[0][4] = 11; //black queen
  for(int i = 0; i < 8; i++) board[1][i] = 7;

  board[7][0] = 2; board[7][7] = 2; //white rook
  board[7][1] = 3; board[7][6] = 3; //white knight
  board[7][2] = 4; board[7][5] = 4; //white bishop
  board[7][3] = 6; //white king
  board[7][4] = 5; //white queen
  for(int i = 0; i < 8; i++) board[6][i] = 1;
//  for(int i = 2; i < 6; i++){
//    for(int j = 0; j < 7; j++){
//      board[i][j] = 0;
//    }
//  }

}


ChessBoard::ChessBoard(): board{}{
  initBoard();
}

void ChessBoard::print(){
  for(int i = 0; i <8; i++){
    cout<<8-i<<"  ";
    for(int j = 0; j < 8; j++){
      cout<<piece_map[board[i][j]]<<" ";
    }
    cout<<endl;
  }
  cout<<endl<<"   ";
  for(int i = 0; i <8; i++){
    cout<<char('A' + i)<<" ";
  }
  cout<<endl;
}

int ChessBoard::getElement(const int &index_i, const int &index_j) const{
  assert(0<= index_i && index_i<8 && 0<=index_j && index_j < 8);
  return board[index_i][index_j];
}
void ChessBoard::capture(const int &index_i, const int &index_j){
  assert(0<= index_i && index_i<8 && 0<=index_j && index_j < 8);
  board[index_i][index_j] = 0;
}

void ChessBoard::updateBoard(const int &index_i, const int &index_j, int value){
  assert(0<= index_i && index_i<8 && 0<=index_j && index_j < 8);
  board[index_i][index_j] = value;

}

void ChessBoard::movePiece(const int x1, const int y1, const int x2, const int y2){
  assert(0<= x1 && x1 <8 && 0<=y1 && y1 < 8);
  assert(0<= x2 && x2 <8 && 0<=y2 && y2 < 8);
  int temp = board[x1][y1];
  board[x1][y1] = 0;
  board[x2][y2] = temp;

}