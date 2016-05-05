#include "move.h"
#include <iostream>
#include <clocale>
Move::Move():
  b_rook1_moved(0),
  b_rook2_moved(0),
  b_king_moved(0),
  b_pawn_moved(0),
  w_rook1_moved(0),
  w_rook2_moved(0),
  w_king_moved(0),
  w_pawn_moved(0),
  b_en_passant_active(0),
  w_en_passant_active(0),
  capture_flag(false),
  color(true),
  friendly(false),
  enpassant(false)
{
  
}

int Move::parseString(){
  if(move.size() != 4){
    cout<<"Invalid format. Please re-enter your move"<<endl;
    return -1;
  }
  curr_x = (((move[0] - '0') - 8)*-1);
  cout<<"curr_x is: "<<curr_x<<endl;
  if(!checkInRange(curr_x)) return -1;
  curr_y = tolower(move[1]) - 'a';
  cout<<"curr_y is: "<<curr_y<<endl;
  if(!checkInRange(curr_y)) return -1;
  move_x = (((move[2] - '0') - 8)*-1);
  cout<<"move_x is: "<<move_x<<endl;
  if(!checkInRange(move_x)) return -1;
  move_y = tolower(move[3]) - 'a';
  cout<<"move_y is: "<<move_y<<endl;
  if(!checkInRange(move_y)) return -1;
  return 0;
}

void Move::updateString(const string &new_move){
  move = new_move;
}

bool Move::checkMove(const ChessBoard &b){
  int piece = b.getElement(curr_x, curr_y);
  int target_location_piece = b.getElement(move_x, move_y);
  cout<<"piece is: "<<piece<<endl;

  if(!piece){
    cout<<"There is no piece there"<<endl;
    return false;
  }

  if((color && piece >= 7) || (!color && piece > 0 && piece < 7)){
    cout<<"Attempting to move opponent's peice"<<endl;
    return false;

  }

  if(piece == 1 || piece == 7){
    resetEnpassant();
    return checkPawn(b);
  }

  /*switch(piece){
    case 0: //empty piece
    case 1: return checkPawn(b);
            break;
    case 2: //white rook
    case 3: //white knight
    case 4: //white bishop
    case 5: //white queen
    case 6: //white king
    case 7: return checkPawn(b);
            break;
    case 8: //black rook
    case 9: //black knight
    case 10: //black bishop
    case 11: //black queen
    case 12: //black king
    default: break;
  }*/
  return false;


  

}

//need to check for promotion still
bool Move::checkPawn(const ChessBoard &b){
  int target_piece = b.getElement(move_x, move_y);
  cout<<"target piece: "<<target_piece<<endl;

  int x_diff = curr_x - move_x;
  int y_diff = curr_y - move_y;
  x_diff = x_diff > 0 ? x_diff : x_diff * -1;
  y_diff = y_diff > 0 ? y_diff : y_diff * -1;
  if(curr_x == 1 || curr_x == 6){ //pawn hasn't moved yet
    if(x_diff == 2){
      if(!target_piece){
        cout<<"before here"<<endl;
        checkEnpassant(b);
        cout<<"after here"<<endl;
        cout<<"black value: "<<b_en_passant_active<<endl;
        cout<<"white value: "<<w_en_passant_active<<endl;
        return true;
      }
    }
    else{
      return pawnMoveCapture(b);
    }
  }
  else{
    return pawnMoveCapture(b);
  }
  
}


bool Move::checkKnight(int target_location_piece){
  int x_diff = curr_x - move_x;
  int y_diff = curr_y - move_y;
  x_diff = x_diff > 0 ? x_diff : x_diff * -1;
  y_diff = y_diff > 0 ? y_diff : y_diff * -1;
  if(color){ //if white
    if((x_diff == 1 && y_diff == 2) || (x_diff == 2 && y_diff == 1)){
      if(!target_location_piece){ //valid move to empty space
        capture_flag = false;
        return true;
      }
      else if(target_location_piece < 7){ //can't move here because piece of same color
        friendly = true;
        return false;
      }
      else{ //must be black piece, able to capture
        capture_flag = true;
        return true;
      }
    }
  }
  else{ //black piece
    if((x_diff == 1 && y_diff == 2) || (x_diff == 2 && y_diff == 1)){
      if(!target_location_piece){ //valid move to empty space
        capture_flag = false;
        return true;
      }
      else if(target_location_piece >= 7){ //can't move here because piece of same color
        friendly = true;
        return false;
      }
      else{ //must be white piece, able to capture
        capture_flag = true;
        return true;
      }
    }
  }
  capture_flag = false;
  return false;
}

bool Move::checkBishop(const ChessBoard &b){
  int rise = curr_y - move_y; //avoiding using a double
  int run = curr_x - curr_y;

  //using direction int to determine diagonal path
  //using for loop to check each square on path

  int direction = 0;
  if(rise > 0 && run > 0){
    direction = 1;
  }
  else if(rise > 0 && run < 0){
    direction = 2;
  }
  else if(rise < 0 && run > 0){
    direction = 3;
  }

  //the other case is covered by initializing direction to 0
  //it corresponds to rise < 0 && run < 0

  rise = rise > 0 ? rise : rise * -1;
  run = run > 0 ? run : run * -1;
  if(run == rise){ // if slope != |1| then can't be diagonal
    if(direction == 0){
      return checkDiagonal(b, rise, false, false);
    }
    else if(direction == 1){
      return checkDiagonal(b, rise, true, true);
    }
    else if(direction == 2){
      return checkDiagonal(b, rise, true, false);
    }
    else if(direction == 3){
      return checkDiagonal(b, rise, false, true);
    }
  }

  return false;

}




bool Move::checkRook(const ChessBoard &b){
  int y_diff = curr_y - move_y;
  if(!y_diff){//invalid move if rook doesn't stay on rank
    int x_diff = curr_x - move_x;
    bool sign = x_diff > 0 ? true : false;
    x_diff = sign ? x_diff : false;
    return checkSlide(b, x_diff, sign);

  }
  return false;
}



bool Move::checkQueen(const ChessBoard &b){
  int y_diff = curr_y - move_y;
  int x_diff = curr_x - move_x;
  bool sign_x = x_diff > 0 ? true : false;
  bool sign_y = y_diff > 0 ? true : false;
  x_diff = sign_x ? x_diff : x_diff * -1;
  y_diff = sign_y ? y_diff : y_diff * -1;

  if(y_diff == x_diff){ //moving diagonally, need to check for when both are 0 still
    return checkDiagonal(b, y_diff, sign_x, sign_y);
  }
  else if(y_diff == 0 && x_diff != 0){ //sliding
    return checkSlide(b, x_diff, sign_x);
  }
  return false;
}

bool Move::checkKing(const ChessBoard &b){
  int y_diff = curr_y - move_y;
  int x_diff = curr_x - move_x;
  bool sign_x = x_diff > 0 ? true : false;
  bool sign_y = y_diff > 0 ? true : false;
  x_diff = sign_x ? x_diff : x_diff * -1;
  y_diff = sign_y ? y_diff : y_diff * -1;
  int temp_target = b.getElement(move_x, move_y);
  if(x_diff <= 1 && y_diff <= 1){ //king can't move more than 1 space
    if(color && temp_target > 0 && temp_target < 7){
      friendly = true;
      return false;

    }
    else if(!color && temp_target >= 7){
      friendly = true;
      return false;
    }
    else if(color && temp_target > 7){
      capture_flag = true;
      return true;
    }
    else if(!color && temp_target > 0 && temp_target <= 7){
      capture_flag = true;
      return true;
    }
    else if(!temp_target){
      return true;
    }

  }
  return false;
}


bool Move::checkDiagonal(const ChessBoard &b, int steps, bool sign_x, bool sign_y){

  //check for pieces from beggining to 1 minus the end
  for(int i = 1; i < steps; i++){

    //if sign is negative, subtract, if sign is positive add
    int temp_x = sign_x? curr_x + i : curr_x - i;
    int temp_y = sign_y? curr_y + i : curr_y - i;

    int temp_target = b.getElement(temp_x, temp_y);

    if(temp_target >0 && temp_target < 7 && color){ //piece of same color (white) in path
      friendly = true;
      return false;
    }
    //otherwise if bla
    else if(temp_target > 0 && temp_target >=7 && !color ){
      friendly = true;
      return false;

    }
    else if(!temp_target){ //the other case where there's an enemy piece before the end of path
      return false;
    }
  }

  int target_piece = b.getElement(move_x, move_y);
  if(color && !target_piece && target_piece < 7){
    capture_flag = true;
    return true;
  }
  else if(!color && target_piece >= 7 ){
    capture_flag = true;
    return true;
  }
  else if(!target_piece){ //otherwise running into piece before path ends
    return true;
  }
  return false;

}

bool Move::checkSlide(const ChessBoard &b, int steps, bool sign_x){
  for(int i = 1; i < steps; i++){
    int temp_x = sign_x ? curr_x + i : curr_x - i;
    int temp_target = b.getElement(temp_x, curr_y);
    if(color && temp_target > 0 && temp_target < 7){ //white rook running into white piece
      friendly = true;
      return false;
    }
    else if(!color && temp_target >= 7){ //black rook running into black piece
      friendly = true;
      return false;
    }
    else if(temp_target){ //otherwise running into piece before path ends
      return false;
    }

  }

  int target_piece = b.getElement(move_x, move_y);
  if(color && target_piece > 0 && target_piece < 7){
    capture_flag = true;
    return true;
  }
  else if(!color && target_piece >= 7 ){
    capture_flag = true;
    return true;
  }
  else if(!target_piece){ //otherwise running into piece before path ends
    return true;
  }
  return false;
}


void Move::checkEnpassant(const ChessBoard &b){
  //check left and right of moved ylocation to see if a pawn is there
  int y1 = move_y + 1;
  int y2 = move_y - 1;
  int p = 0;
  if(checkInRange(y1)){
    p = b.getElement(move_x,y1);
    if(color && p == 7){
      b_en_passant_active = true;
      b_en_passant_direction = true;
    }
    else if(!color && p == 1){
      w_en_passant_active = true;
      w_en_passant_direction = true;
    }
  }

  if(checkInRange(y2)){
    p = b.getElement(move_x,y2);
    if(color && p ==7){
      b_en_passant_active = true;
      b_en_passant_direction = false;
    }
    else if(!color && p == 1){
      w_en_passant_active = true;
      w_en_passant_direction = false;
    }
  }
}

bool Move::pawnMoveCapture(const ChessBoard &b){

  int x_diff = curr_x - move_x;
  int y_diff = curr_y - move_y;
  int abs_x_diff = x_diff > 0 ? x_diff : x_diff * -1;
  int abs_y_diff = y_diff > 0 ? y_diff : y_diff * -1;

  int y1 = move_y + 1;
  int y2 = move_y - 1;
  int target_piece = b.getElement(move_x, move_y);
  if(abs_x_diff == 1){ //pawn moving one space, can either capute or move straight
    cout<<"should be here kaooa"<<endl;

    //check for enpassant
    if(color && w_en_passant_active){
      if((w_en_passant_direction && y_diff == 1) || (!w_en_passant_direction && y_diff == -1)){
        enpassant = true;
        return true;

      }
    }

    else if(!color && b_en_passant_active){
      if((b_en_passant_direction && y_diff == 1) || (!b_en_passant_direction && y_diff == -1)){
        enpassant = true;
        return true;

      }
    }

    //check for basic pawn movement and capture
    if(color && abs_y_diff == 1 && (target_piece >= 7) && (x_diff > 0)){
      //white pawn capturing black piece
      capture_flag = true;
      return true;
    }

    else if(!color && abs_y_diff == 1 && (target_piece >0 && target_piece < 7) && (x_diff < 0)){
      cout<<"black pawn capture"<<endl;
      //black pawn capturing white piece
      capture_flag = true;
      return true;
    }
    else if(!y_diff){
      cout<<"advancing one"<<endl;
      cout<<"color is: "<<color<<endl;
      cout<<"x diff is: "<<x_diff<<endl;
      if(color && (target_piece == 0) && (x_diff > 0)){
        //white advancing one onto empty square
        return true;
      }
      else if(!color && !target_piece && x_diff < 0){
        return true;
      }
    }
  }
  return false;

}


bool Move::checkInRange(int a){
  if(a < 0 || a > 7) return false;
  return true;
}

int Move::getMove_x(){
  return move_x;
}

int Move::getMove_y(){
  return move_y;
}

int Move::getCur_x(){
  return curr_x;
}

int Move::getCur_y(){
  return curr_y;
}

void Move::changeTurns(){
  color = color ? false : true; //if white(true) change to black (false) or vice versa
}

void Move::resetEnpassant(){
  enpassant = false;
  if(!color && w_en_passant_active){
    w_en_passant_active = false;
  }
  else if(color && b_en_passant_active){
    b_en_passant_active = false;
  }
}

bool Move::enpassantFlag(){
  return enpassant;

}

bool Move::getTurn(){
  return color;
}