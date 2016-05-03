#ifndef CHESS_GAME_H
#define CHESS_GAME_H
#include <string>
#include "board.h"
#include "move.h"
using namespace std;

class ChessGame{
public:

  void run();
  string getMoveInput();
  int checkForCommands(string &input);
private:
  Move moveObject;
  ChessBoard cBoard;
};

#endif