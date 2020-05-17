# ifndef BOARD_H
# define BOARD_H

#include <stdint.h>

//#include <ncurses.h>

# include "piece.h"

class piece;

class board{
 public:
  piece *board[8][8];
  int turn;
  //Will start with one and increment. 
  //This will let us "control" whose turn it is
};

void gen_board(board *b);

int move_piece(board *b, int from_y, int from_x, int to_y, int to_x);

int kill(board *b, int pos_y, int pos_x);

void io_display_board(board *b);

void io_handle_input(board *b);

# endif
