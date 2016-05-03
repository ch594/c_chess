#ifndef BOARD_H
#define BOARD_H
#include <unordered_map>
#include <string>
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

class ChessBoard{
public:
  int board[8][8];
  bool contains;
  unordered_map<int,string> piece_map =
  {
    {0,"-"},
    {1,"P"},
    {2,"R"},
    {3,"N"},
    {4,"B"},
    {5,"Q"},
    {6,"K"},
    {7, "p"},
    {8,"r"},
    {9,"n"},
    {10,"b"},
    {11,"q"},
    {12,"k"}
  };
  ChessBoard();

  void print();
  int getElement(const int &index_i, const int &index_j) const;
  void updateBoard(const int &index_i, const int &index_j, int value);
  void capture(const int &index_i, const int &index_j);
  void movePiece(const int x1, const int y1, const int x2, const int y2);
private:
  void initBoard();

};

#endif