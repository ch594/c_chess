#include "pawn.h"

int Pawn::count = 0;
Pawn::Pawn(){
  count++;
}

int Pawn::getId(){
  return 0;
}

int Pawn::getUniqueId(){
  return count;
}

bool Pawn::getColor(){
  return color;
}

int Pawn::getPos(){
  return 0;
}