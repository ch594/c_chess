#ifndef CHESS_OBJECT_H
#define CHESS_OBJECT_H

class ChessObject{
public:
  virtual int getId() = 0; //correspdonds to piece ids defined in board.h
  virtual int getPos() = 0; //position on board
  virtual int getUniqueId() = 0; //unique id to differentiate between multiple pieces of same type
  virtual bool getColor() = 0; // 1 = white, 0 = black
  //virtual ~ChessObject() = 0;
};

#endif