#ifndef MOVE_H
#define MOVE_H
#include <string>
#include "board.h"
using namespace std;


class Move{
public:
  Move();
  int parseString();
  void updateString(const string &new_move);
  bool checkMove(const ChessBoard &b); 


  bool checkPawn(int target_location_piece); //1 = white, 0 = black
  bool checkKnight(int target_location_piece); //1 = white, 0 = black
  bool checkBishop(const ChessBoard &b); //1 = white, 0 = black
  bool checkRook(const ChessBoard &b);
  bool checkQueen(const ChessBoard &b);
  bool checkKing(const ChessBoard &b);

  int getMove_x();
  int getMove_y();
  int getCur_x();
  int getCur_y();

//private:
  bool b_rook1_moved;
  bool b_rook2_moved;
  bool b_king_moved;
  bool b_pawn_moved;
  bool w_rook1_moved;
  bool w_rook2_moved;
  bool w_king_moved;
  bool w_pawn_moved;
  bool b_en_passant_active;
  bool w_en_passant_active;
  bool friendly; //flag if move runs into piece of same color

  bool color; //what piece
  bool capture_flag; //true if move results in enemy piece captured

  string move;

  int curr_x; //rank, horizontal rows, for selected piece
  int curr_y; //file, vertical collumns for selected piece

  int move_x; //rank, horizontal rows, for desired location
  int move_y; //file, vertical collumns for desired location

  void capture(const int &index_i, const int &index_j, 
              int target_location_piece);

  //helper functions for sliding pieces
  bool checkDiagonal(const ChessBoard &b, int steps, bool sign_x, bool sign_y);
  bool checkSlide(const ChessBoard &b, int steps, bool sign_x);

  //
  bool checkInRange(int a);

};


#endif