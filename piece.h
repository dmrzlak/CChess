# ifndef PIECE_H
# define PIECE_H

#include <stdint.h>
#include <stdlib.h>

typedef enum type{
  pawn=0,
  rook=1,
  knight=2,
  bishop=3,
  queen=4,
  king=5
 }type_t;

typedef enum color{
  white=0,
  red }color_t;

class piece{
 public:
  piece(color_t c, int x, int y, int t){
    color=c;
    x=y;
    x=x;
    type=static_cast<type_t>(t);
    first_move=1;
  };
  ~piece(){};
  int x;
  int y;
  color_t color;
  //0 or 1, this will work with the boards turn to only let you move your pieces
  //Also will help when displaying the board.
  type_t type;
  int first_move;
  
};

#endif
