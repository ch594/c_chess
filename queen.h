#ifndef QUEEN_H
#define QUEEN_H

#include "chess_object.h"

class Queen: public ChessObject{
public:
  Queen();
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