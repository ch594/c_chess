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
  bool checkMove(ChessBoard &b); 


  bool checkPawn(ChessBoard &b); //1 = white, 0 = black
  bool checkKnight(int target_location_piece); //1 = white, 0 = black
  bool checkBishop(const ChessBoard &b); //1 = white, 0 = black
  bool checkRook(const ChessBoard &b);
  bool checkQueen(const ChessBoard &b);
  bool checkKing(ChessBoard &b);
  void changeTurns();
  bool enpassantFlag();//return true if enpassant capture happened
  bool getTurn(); //return value of color (whose turn it is)

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
  bool b_en_passant_direction; // true = right, false = left
  bool w_en_passant_active;
  bool w_en_passant_direction; // true = right, false = left
  bool friendly; //flag if move runs into piece of same color
  bool enpassant; //flag to determine if enpassant happened

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
  bool checkSlide(const ChessBoard &b, int steps, bool, bool);

  //check if piece is in board 
  bool checkInRange(int a);

  //checks if a pawn moving two spaces allows the opponent to do an en passant the next turn
  void checkEnpassant(const ChessBoard &b);

  //helper function to see if pawn can move and/or capture
  bool pawnMoveCapture(const ChessBoard &b);

  //reset the enpassant flags the next turn
  void resetEnpassant();

  //check for promotion of pawn and promote
  bool checkPromotion(ChessBoard &b);

  int getPromotionInput();
};


#endif