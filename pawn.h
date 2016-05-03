#ifndef PAWN_H
#define PAWN_H

#include "chess_object.h"

class Pawn: public ChessObject{
public:
  Pawn();
  int getId();
  int getPos();
  bool getColor();
  int getUniqueId();
private:
  int position;
  int id;
  bool color;
  static int count;

};
#endif