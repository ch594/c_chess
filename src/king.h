#ifndef KING_H
#define KING_H

#include "chess_object.h"

class King: public ChessObject{
public:
  King();
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