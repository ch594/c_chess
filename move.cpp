#include "move.h"
#include <iostream>
#include <clocale>
Move::Move():
  b_rook1_moved(0),
  b_rook2_moved(0),
  b_king_moved(0),
  w_rook1_moved(0),
  w_rook2_moved(0),
  w_king_moved(0),
  b_en_passant_active(0),
  w_en_passant_active(0),
  capture_flag(false),
  color(true),
  friendly(false),
  enpassant(false),
  w_king_x(7),
  w_king_y(3),
  b_king_x(0),
  b_king_y(3),
  check_flag(true)
{
  
}

int Move::parseString(){
  if(move.size() != 4){
    cout<<"Invalid format. Please re-enter your move"<<endl;
    return -1;
  }
  curr_x = (((move[0] - '0') - 8)*-1);
  //cout<<"curr_x is: "<<curr_x<<endl;
  if(!checkInRange(curr_x)) return -1;
  curr_y = tolower(move[1]) - 'a';
  //cout<<"curr_y is: "<<curr_y<<endl;
  if(!checkInRange(curr_y)) return -1;
  move_x = (((move[2] - '0') - 8)*-1);
  //cout<<"move_x is: "<<move_x<<endl;
  if(!checkInRange(move_x)) return -1;
  move_y = tolower(move[3]) - 'a';
  //cout<<"move_y is: "<<move_y<<endl;
  if(!checkInRange(move_y)) return -1;
  return 0;
}

void Move::updateString(const string &new_move){
  move = new_move;
}

bool Move::checkMove(ChessBoard &b){
  int piece = b.getElement(curr_x, curr_y);
  int target_location_piece = b.getElement(move_x, move_y);
  //cout<<"piece is: "<<piece<<endl;

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

  else if(piece == 3 || piece == 9){
    return checkKnight(target_location_piece);
  }

  else if(piece == 4 || piece == 10){
    return checkBishop(b);
  }

  else if(piece == 5 || piece == 11){
    return checkQueen(b);
  }

  else if(piece == 2 || piece == 8){
    return checkRook(b);
  }
  else if(piece == 6 || piece == 12){
    return checkKing(b);
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
bool Move::checkPawn(ChessBoard &b){
  int target_piece = b.getElement(move_x, move_y);

  int x_diff = curr_x - move_x;
  int y_diff = curr_y - move_y;
  x_diff = x_diff > 0 ? x_diff : x_diff * -1;
  y_diff = y_diff > 0 ? y_diff : y_diff * -1;
  if(curr_x == 1 || curr_x == 6){ //pawn hasn't moved yet
    if(x_diff == 2){
      if(!target_piece && y_diff == 0){
        checkEnpassant(b);
        //cout<<"black value: "<<b_en_passant_active<<endl;
        //cout<<"white value: "<<w_en_passant_active<<endl;
        return true;
      }
    }
    else{
      if(checkPromotion(b)) return true;
      return pawnMoveCapture(b);
    }
  }
  else{
    if(checkPromotion(b)) return true;
    return pawnMoveCapture(b);
  }
  
}


bool Move::checkKnight(int target_location_piece){
  int x_diff = curr_x - move_x;
  int y_diff = curr_y - move_y;
  x_diff = x_diff > 0 ? x_diff : x_diff * -1;
  y_diff = y_diff > 0 ? y_diff : y_diff * -1;

  if((x_diff == 1 && y_diff == 2) || (x_diff == 2 && y_diff == 1)){
    if(!target_location_piece){
      capture_flag = true;
      return true;
    }
    else if(color && target_location_piece >= 7){
      return true;
    }
    else if(!color && target_location_piece < 7){
      return true;
    }
  }

  capture_flag = false;
  return false;
}

bool Move::checkBishop(const ChessBoard &b){
  int rise = curr_x - move_x; //avoiding using a double
  int run = curr_y - move_y;

  //using direction int to determine diagonal path
  //using for loop to check each square on path

  bool sign_x = rise > 0 ? false : true;
  bool sign_y = run > 0 ? false : true;

  //the other case is covered by initializing direction to 0
  //it corresponds to rise < 0 && run < 0

  rise = rise > 0 ? rise : rise * -1;
  run = run > 0 ? run : run * -1;
     //cout<<"check bishop"<<endl;
     //cout<<"rise is: "<<rise<<endl;
     //cout<<"run is: "<<run<<endl;
  if(run == rise){ // if slope != |1| then can't be diagonal
    return checkDiagonal(b, rise, sign_x, sign_y);

  }

  return false;

}



//add checking in x direction
bool Move::checkRook(const ChessBoard &b){
  int y_diff = curr_y - move_y;
  int x_diff = curr_x - move_x;
  bool sign_x = x_diff > 0 ? true : false;
  bool sign_y = y_diff > 0 ? true : false;
  x_diff = sign_x ? x_diff : x_diff * -1;
  y_diff = sign_y ? y_diff : y_diff * -1;



  if((y_diff && !x_diff) ||(!y_diff && x_diff)){//
    //int x_diff = curr_x - move_x;
    bool direction = y_diff == 0 ? true : false;
    bool sign = (direction ? sign_x : sign_y) ? false : true;
    //x_diff = sign ? x_diff : false;
    //cout<<"x_diff is: "<<x_diff<<endl;
    //cout<<"y_diff is: "<<y_diff<<endl;
    bool result = checkSlide(b, direction ? x_diff : y_diff, direction, sign);
    if(color && result && curr_x == 7 && curr_y == 0){
      w_rook1_moved = true;
    }
    else if(color && result && curr_x == 7 && curr_y == 7){
      w_rook2_moved = true;
    }
    else if(!color && result && curr_x == 0 && curr_y == 0){
      b_rook1_moved = true;
    }
    else if(!color && result && curr_x == 0 && curr_y == 7){
      b_rook2_moved = true;
    }
    return result;

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
    sign_x = sign_x ? false : true;
    sign_y = sign_y ? false : true;
    return checkDiagonal(b, y_diff, sign_x, sign_y);
  }
  else if((y_diff && !x_diff) ||(!y_diff && x_diff)){ //sliding
    bool direction = y_diff == 0 ? true : false;
    bool sign = (direction ? sign_x : sign_y) ? false : true;
    return checkSlide(b, direction ? x_diff : y_diff, direction, sign);

  }
  return false;
}

bool Move::checkKing(ChessBoard &b){
  int y_diff = curr_y - move_y;
  int x_diff = curr_x - move_x;
  bool sign_x = x_diff > 0 ? true : false;
  bool sign_y = y_diff > 0 ? true : false;
  x_diff = sign_x ? x_diff : x_diff * -1;
  y_diff = sign_y ? y_diff : y_diff * -1;
  int temp_target = b.getElement(move_x, move_y);

  if(x_diff == 0 && y_diff == 2){ //checking for castling
    if(!w_king_moved && !w_rook1_moved && sign_y){ //castling with left rook
      if(!b.getElement(curr_x,curr_y -1) && !b.getElement(curr_x,curr_y -1)){
        b.movePiece(7,0, 7, 2);
        w_king_moved = true;
        w_rook1_moved = true;
        w_king_x = move_x;
        w_king_y = move_y;
        return true;
      }
    }

    //white king castling with right rook
    else if(!w_king_moved && !w_rook2_moved && !sign_y){
      int a = b.getElement(curr_x,curr_y +1);
      int a2 = b.getElement(curr_x, curr_y + 2);
      int a3 = b.getElement(curr_x, curr_y + 3);

      if(!( a && a2 && a3)){
        b.movePiece(7,7, 7, 4);
        w_king_moved = true;
        w_rook2_moved = true;
        w_king_x = move_x;
        w_king_y = move_y;
        return true;  
      }

    }

    //black king castling with right rook
    else if(!b_king_moved && !b_rook1_moved && sign_y){
      int a = b.getElement(curr_x,curr_y -1);
      int a2 = b.getElement(curr_x, curr_y - 2);

      if(!( a && a2 )){
        b.movePiece(0,0, 0, 2);
        b_king_moved = true;
        b_rook1_moved = true;
        b_king_x = move_x;
        b_king_y = move_y;
        return true;  
      }

    }

    //black king castling with left rook
    else if(!b_king_moved && !b_rook2_moved && !sign_y){
      int a = b.getElement(curr_x,curr_y +1);
      int a2 = b.getElement(curr_x, curr_y + 2);
      int a3 = b.getElement(curr_x, curr_y + 3);

      if(!( a && a2 && a3)){
        b.movePiece(0,7, 0, 4);
        b_king_moved = true;
        b_rook2_moved = true;
        b_king_x = move_x;
        b_king_y = move_y;
        return true;  
      }

    }


  }

  if(x_diff <= 1 && y_diff <= 1){ //king can't move more than 1 space
    //cout<<"must be here"<<endl;
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
        w_king_x = move_x;
        w_king_y = move_y;
      return true;
    }
    else if(!color && temp_target > 0 && temp_target <= 7){
      capture_flag = true;
      b_king_x = move_x;
      b_king_y = move_y;
      return true;
    }
    else if(!temp_target && color){
      w_king_x = move_x;
      w_king_y = move_y;
      return true;
    }
    else if(!temp_target && !color){
      b_king_x = move_x;
      b_king_y = move_y;
      return true;
    }

  }
  return false;
}


bool Move::checkDiagonal(const ChessBoard &b, int steps, bool sign_x, bool sign_y){

  //check for pieces from beggining to 1 minus the end
  for(int i = 1; i < steps; i++){
    //cout<<"steps"<<endl;
    //if sign is negative, subtract, if sign is positive add
    int temp_x = sign_x? curr_x + i : curr_x - i;
    int temp_y = sign_y? curr_y + i : curr_y - i;
    //cout<<"temp_x: "<<temp_x<<" temp_y "<<temp_y<<endl;
    int temp_target = b.getElement(temp_x, temp_y);
    //cout<<"temp target is: "<<temp_target<<endl;

    if(temp_target >0 && temp_target < 7 && color){ //piece of same color (white) in path
      //cout<<"if"<<endl;
      friendly = true;
      return false;
    }
    //otherwise if bla
    else if(temp_target > 0 && temp_target >=7 && !color ){
      //cout<<"second"<<endl;
      friendly = true;
      return false;

    }
    else if(temp_target){ //the other case where there's an enemy piece before the end of path
      //cout<<"third"<<endl;
      return false;
    }
  }
  //cout<<"here after loop"<<endl;
  int target_piece = b.getElement(move_x, move_y);
  if(color && target_piece >= 7){
    capture_flag = true;
    return true;
  }
  else if(!color && target_piece < 7 && target_piece > 0){
    capture_flag = true;
    return true;
  }
  else if(!target_piece){ //otherwise running into piece before path ends
    return true;
  }
  return false;

}

bool Move::checkSlide(const ChessBoard &b, int steps, bool direction, bool sign){
  //direction true = vertical sliding, false = horizontal sliding

  //int temp_direction = direction ? curr_y : curr_x;
   // cout<<"steps is: "<<steps<<endl;
  int temp_sign = sign ? 1 : -1;
  int temp_x = 0;
  int temp_y = 0;
  for(int i = 1; i < steps; i++){
    //cout<<"direction is "<<direction<<" sign is "<<sign<<endl;
    temp_x = direction ? curr_x + (i*temp_sign) : curr_x;
    temp_y = direction ? curr_y : curr_y + (i*temp_sign) ;
    //int temp_x = sign_x ? curr_x + i : curr_x - i;
    int temp_target = b.getElement(temp_x, temp_y);
    //cout<<"temp target is: "<<temp_target<<endl;
   // cout<<"temp x is "<<temp_x<<" temp y is "<<temp_y<<endl;
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
  if(color && target_piece >= 7){
    capture_flag = true;
    return true;
  }
  else if(!color && target_piece < 7 && target_piece > 0 ){
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
    //cout<<"should be here kaooa"<<endl;

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
      //cout<<"black pawn capture"<<endl;
      //black pawn capturing white piece
      capture_flag = true;
      return true;
    }
    else if(!y_diff){
      //cout<<"advancing one"<<endl;
      //cout<<"color is: "<<color<<endl;
      //cout<<"x diff is: "<<x_diff<<endl;
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

bool Move::checkPromotion(ChessBoard &b){
  if((color && curr_x == 1 && move_x == 0) || (!color && curr_x == 6 && move_x == 7) ){
    if(pawnMoveCapture(b)){
      int piece = getPromotionInput();
      b.updateBoard(curr_x, curr_y, piece);
      return true;
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

int Move::getPromotionInput(){
  cout<<"Enter piece to promote pawn to(Q, B, N, R)"<<endl;
  string unit;
  cin>>unit;
  int piece = 0;
  while (unit != "Q" || unit != "B" || unit != "N" || unit != "R"){
    cout<<"Invalid piece. Please choose from one of Q, B, N, R"<<endl;
    cin>>unit;
  }
  if(color){
    if(unit == "Q") piece = 5;
    else if(unit == "B") piece = 4;
    else if(unit == "N") piece = 3;
    else if (unit == "R") piece = 2;
  }
  else{
    if(unit == "Q") piece = 11;
    else if(unit == "B") piece = 10;
    else if(unit == "N") piece = 9;
    else if (unit == "R") piece = 8;
  }
  
}

bool Move::check(const ChessBoard &b){
  //checking for knight being in range
  //possible knights must be in a 5x5 square with the king in center

  int king_x = color ? b_king_x : w_king_x;
  int king_y = color ? b_king_y : w_king_y;
  int target_piece = color ? 3 : 9;

  for(int i = king_x - 2; i <= king_x + 2; i++){
   
    if(checkInRange(i)){
      cout<<"in this loop"<<endl;
      
      for(int j = king_y - 2; j <= king_y + 2; j++){
     
        if(checkInRange(j)){
          int piece = b.getElement(i,j);
          
          if(piece == target_piece){
            int temp_i = king_x - i;
            int temp_j = king_y - j;
            temp_i = temp_i > 0 ? temp_i : temp_i * -1;
            temp_j = temp_j > 0 ? temp_j : temp_j * -1;
            //cout<<"in inner loop"<<endl;
            //cout<<"temp_i: "<<temp_i<<" temp_j: "<<temp_j<<endl;
            if(3 == temp_i + temp_j ) {
              check_flag = true;
              return true;
            }

          }

        }
      
      }
    }
  }

  //check by bishop or queen diagonally

  target_piece = color ? 10 : 4;


  for(int d = 0 ; d < 3; d++){
    int step_x = 0;
    int step_y = 0;

    if(d == 0){
      step_x = 1;
      step_y =1;
    }
    else if(d == 1){
      step_x = 1;
      step_y = -1;

    }
    else if(d == 2){
      step_x = -1;
      step_y = 1;
    }
    else if(d == 3){
      step_x = -1;
      step_y = -1;
    }

    for(int i = king_x, j = king_y; checkInRange(i) && checkInRange(j); 
      i += step_x, j += step_y){
      int piece = b.getElement(i,j);
      if(piece == 6 || piece == 12) continue;

      //if same color then blocking potential check
      if(!color && piece > 0 && piece < 7) break;
      else if(color && piece >= 7) break;
      else if(!color && (piece == 10 || piece == 11)){
        check_flag = true;
        return true;
      }
      else if(color && (piece == 4 || piece == 5)) {
        check_flag = true;
        return true;
      }
    }

  }

  //rook or queen sliding

  for(int d = 0 ; d < 3; d++){
    int step_x = 0;
    int step_y = 0;

    if(d == 0){
      step_x = 1;
      step_y =0;
    }
    else if(d == 1){
      step_x = 1;
      step_y = 0;

    }
    else if(d == 2){
      step_x = 0;
      step_y = 1;
    }
    else if(d == 3){
      step_x = 0;
      step_y = -1;
    }

    for(int i = king_x, j = king_y; checkInRange(i) && checkInRange(j); 
      i += step_x, j += step_y){
      int piece = b.getElement(i,j);
      if(piece == 6 || piece == 12) continue;

      //if same color then blocking potential check
      if(!color && piece > 0 && piece < 7) break;
      else if(color && piece >= 7) break;
      else if(!color && (piece == 8 || piece == 11)){
        check_flag = true;
        return true;
      }
      else if(color && (piece == 2 || piece == 5)) {
        check_flag = true;
        return true;
      }
    }

  }

  //pawn check
  if(color){
    int temp_i = king_x + 1;
    int temp_j = king_y - 1;
    int temp_j2 = king_y + 1;
    if(checkInRange(temp_i) && checkInRange(temp_j) && checkInRange(temp_j2)){
      if(b.getElement(temp_i, temp_j) == 1){
        check_flag = true;
       return true;
      }
      else if(b.getElement(temp_i, temp_j2) == 1){
        check_flag = true;
       return true;
      }
    }
  }
  else if(!color){
    int temp_i = king_x - 1;
    int temp_j = king_y - 1;
    int temp_j2 = king_y + 1;
    if(checkInRange(temp_i) && checkInRange(temp_j) && checkInRange(temp_j2)){
      if(b.getElement(temp_i, temp_j) == 7){
        check_flag = true;
        return true;
      } 
      else if(b.getElement(temp_i, temp_j2) == 7){
        check_flag = true;
        return true;
      } 
    }    
  }



  return false;
}



bool Move::isStillCheck(const ChessBoard &b){
  //checking for knight being in range
  //possible knights must be in a 5x5 square with the king in center

  int king_x = color ? w_king_x : b_king_x;
  int king_y = color ? w_king_y : b_king_y;
  int target_piece = color ? 9 : 3;

  for(int i = king_x - 2; i <= king_x + 2; i++){
   
    if(checkInRange(i)){
      cout<<"in this loop"<<endl;
      
      for(int j = king_y - 2; j <= king_y + 2; j++){
     
        if(checkInRange(j)){
          int piece = b.getElement(i,j);
          
          if(piece == target_piece){
            int temp_i = king_x - i;
            int temp_j = king_y - j;
            temp_i = temp_i > 0 ? temp_i : temp_i * -1;
            temp_j = temp_j > 0 ? temp_j : temp_j * -1;
            //cout<<"in inner loop"<<endl;
            //cout<<"temp_i: "<<temp_i<<" temp_j: "<<temp_j<<endl;
            if(3 == temp_i + temp_j ) {
              check_flag = true;
              return true;
            }

          }

        }
      
      }
    }
  }

  //check by bishop or queen diagonally



  for(int d = 0 ; d < 3; d++){
    int step_x = 0;
    int step_y = 0;

    if(d == 0){
      step_x = 1;
      step_y =1;
    }
    else if(d == 1){
      step_x = 1;
      step_y = -1;

    }
    else if(d == 2){
      step_x = -1;
      step_y = 1;
    }
    else if(d == 3){
      step_x = -1;
      step_y = -1;
    }

    for(int i = king_x, j = king_y; checkInRange(i) && checkInRange(j); 
      i += step_x, j += step_y){
      int piece = b.getElement(i,j);
      if(piece == 6 || piece == 12) continue;

      //if same color then blocking potential check
      if(color && piece > 0 && piece < 7) break;
      else if(!color && piece >= 7) break;
      else if(color && (piece == 10 || piece == 11)){
        check_flag = true;
        return true;
      }
      else if(!color && (piece == 4 || piece == 5)) {
        check_flag = true;
        return true;
      }
    }

  }

  //rook or queen sliding

  for(int d = 0 ; d < 3; d++){
    int step_x = 0;
    int step_y = 0;

    if(d == 0){
      step_x = 1;
      step_y =0;
    }
    else if(d == 1){
      step_x = 1;
      step_y = 0;

    }
    else if(d == 2){
      step_x = 0;
      step_y = 1;
    }
    else if(d == 3){
      step_x = 0;
      step_y = -1;
    }

    for(int i = king_x, j = king_y; checkInRange(i) && checkInRange(j); 
      i += step_x, j += step_y){
      int piece = b.getElement(i,j);
      if(piece == 6 || piece == 12) continue;

      //if same color then blocking potential check
      if(color && piece > 0 && piece < 7) break;
      else if(!color && piece >= 7) break;
      else if(color && (piece == 8 || piece == 11)){
        check_flag = true;
        return true;
      }
      else if(!color && (piece == 2 || piece == 5)) {
        check_flag = true;
        return true;
      }
    }

  }

  //pawn check
  if(color){
    int temp_i = king_x + 1;
    int temp_j = king_y - 1;
    int temp_j2 = king_y + 1;
    if(checkInRange(temp_i) && checkInRange(temp_j) && checkInRange(temp_j2)){
      if(b.getElement(temp_i, temp_j) == 7){
        check_flag = true;
       return true;
      }
      if(b.getElement(temp_i, temp_j2) == 7){
        check_flag = true;
       return true;
      }
    }
  }
  else if(!color){
    int temp_i = king_x - 1;
    int temp_j = king_y - 1;
    int temp_j2 = king_y + 1;
    if(checkInRange(temp_i) && checkInRange(temp_j) && checkInRange(temp_j2)){
      if(b.getElement(temp_i, temp_j) == 1){
        check_flag = true;
        return true;
      } 
      if(b.getElement(temp_i, temp_j2) == 1){
        check_flag = true;
        return true;
      } 
    }    
  }


  check_flag = false;
  return false;
}

bool Move::getCheckStatus(){
  return check_flag;
}