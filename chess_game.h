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
  void checkFlags();
  bool ask_for_network();
  void run_network();
  int doMove(string &move);
};

#endif