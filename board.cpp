#include "board.h" 

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>


void clear_io(void){
  for(int i=0; i<=21; i++){
    for(int j=0; j<=80; j++){
      mvprintw(i, j, " ");
    }
  }
}


void gen_board(board *b){
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      b->board[i][j]= NULL;
    }
  }
  b->turn = 0;
  //loop and place the pieces can do 4 at a time
  //4 for the type that should be there
  //I can loop and place the pawns no problem
  
  for(int i=0; i<8; i++){
    //red
    b->board[1][i]=new piece(red, i, 1, pawn); 
    //white
    b->board[6][i]=new piece(white, i,6, pawn);
  }
  
  b->board[0][3]=new piece(red, 3, 0, king);  
  b->board[7][3]=new piece(white, 3, 7, king);
  
  b->board[0][4]= new piece(red, 4, 0, queen);  
  b->board[7][4]= new piece(white, 4, 7, queen);
  
  for(int i=0; i<3; i++){
    //left side
    b->board[0][i]=new piece(red, i, 0, static_cast<type_t>(i+1));
    b->board[7][i]=new piece(white, i, 7, static_cast<type_t>(i+1));
  
    //right side
    b->board[0][7-i]=new piece(red, 7-i, 0, static_cast<type_t>(i+1));
    b->board[7][7-i]=new piece(white, 7-i, 7, static_cast<type_t>(i+1));
  }
  
}

void io_init(void){
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void io_reset(void){
  endwin();
}

void io_display(board *b){
  clear_io();
  mvprintw(6, 33,"  12345678");
  for(int y=0; y<8; y++){
    mvprintw(y+7, 33, "%c|        |%c", 'A'+y, 'A'+y);
    for(int x=0; x<8; x++){     
      if(b->board[y][x]){
	if(b->board[y][x]->color==1){
	  attron(COLOR_PAIR(COLOR_RED));
	  switch(b->board[y][x]->type){
	  case 0:
	    mvprintw(y+7, x+35, "P");
	    break;
	  case 1:	  
	    mvprintw(y+7, x+35, "R");
	    break;
	  case 2:
	    mvprintw(y+7, x+35, "H");
	    break;
	  case 3:
	    mvprintw(y+7, x+35, "B");
	    break;
	  case 4:
	    mvprintw(y+7, x+35, "Q");
	    break;
	  case 5:
	    mvprintw(y+7, x+35, "K");
	    break;
	  default:
	    mvprintw(y+7, x+35, "*");
	    break;
	  }
	  attroff(COLOR_PAIR(COLOR_RED));
	}
	else{
	  switch(b->board[y][x]->type){
	  case 0:
	    mvprintw(y+7, x+35, "P");
	    break;
	  case 1:	  
	    mvprintw(y+7, x+35, "R");
	    break;
	  case 2:
	    mvprintw(y+7, x+35, "H");
	    break;
	  case 3:
	    mvprintw(y+7, x+35, "B");
	    break;
	  case 4:
	    mvprintw(y+7, x+35, "Q");
	    break;
	  case 5:
	    mvprintw(y+7, x+35, "K");
	    break;
	  default:
	    mvprintw(y+7, x+35, "*");
	    break;
	  }
	}
      }
      else{
	  mvprintw(y+7, x+35, "#");
      }
    }
   
  }
  // mvprintw(9, 13, "-----------------");
  mvprintw(15, 33,"  12345678  ");
  mvprintw(22, 0, "Turn %d", b->turn+1);
  refresh();
}


int move_pawn(board *b, int from_y, int from_x, int to_y, int to_x){
  //Already checked if another piece of the same color is in the way
  //just have to check the first move and if there is a piece inbetween it
  int y_dist=(to_y-from_y)<0 ? from_y-to_y : to_y-from_y;
  int x_dist=(to_x-from_x)<0 ? from_x-to_x : to_x-from_x;
  
  //check the move direction, white can only move up
  //red can only move down
  if(b->board[from_y][from_x]->color==0 && from_y<to_y){
    return 0;
  }
  if(b->board[from_y][from_x]->color==1 && from_y>to_y){
    return 0;
  }
  //check that the pawn is not moveing furter than 2 up
  //check that the pawn is not moving diagonally further than the one square
  if( y_dist>2 || (y_dist>1 && x_dist>1) ){
    return 0;
  }
  //check if the pawn is moving further than 2 up after its first move
  if(!b->board[from_y][from_x]->first_move && y_dist>1){
    return 0;
  }
  if(x_dist==1 && !b->board[to_y][to_x]){
    return 0;
  }
  if(y_dist==1 && x_dist==0 && b->board[to_y][to_x]){
    return 0;
  }

  delete b->board[to_y][to_x];
  b->board[to_y][to_x]=b->board[from_y][from_x];
  b->board[from_y][from_x]->first_move=0;
  b->board[from_y][from_x]=NULL;
  if(b->board[to_y][to_x]->color==0 && to_y==0){
    clear_io();
    mvprintw(10, 20, 
	     "Select the type of piece you'd like to change the pawn to:");
    mvprintw(11, 20, "P, R, H, B, Q");
    int key;
    do{
      key=getch();
    }while(key!=27&&key!='P'&&key!='R'&&key!='H'&&key!='B'&&key!='Q');
    switch(key){
    case 'R':
      delete b->board[to_y][to_x];
      b->board[to_y][to_x]=new piece(white, to_x, to_y, 1);
      break;
    case 'H':
      delete b->board[to_y][to_x];
      b->board[to_y][to_x]=new piece(white, to_x, to_y, 2);
      break;
    case 'B':
      delete b->board[to_y][to_x];
      b->board[to_y][to_x]=new piece(white, to_x, to_y, 3);
      break;
    case 'Q':
      delete b->board[to_y][to_x];
      b->board[to_y][to_x]=new piece(white, to_x, to_y, 4);
      break;
    case 'P':
    case 27:
    default:
      break;  
    }
  }

  if(b->board[to_y][to_x]->color==1 && to_y==7){
    clear_io();
    mvprintw(10, 20, 
	     "Select the type of piece you'd like to change the pawn to:");
    mvprintw(11, 20, "P, R, H, B, Q");
    int key;
    do{
      key=getch();
    }while(key!=27&&key!='P'&&key!='R'&&key!='H'&&key!='B'&&key!='Q');
    switch(key){
    case 'R':
      delete b->board[to_y][to_x];
      b->board[to_y][to_x]=new piece(red, to_x, to_y, 1);
      break;
    case 'H':
      delete b->board[to_y][to_x];
      b->board[to_y][to_x]=new piece(red, to_x, to_y, 2);
      break;
    case 'B':
      delete b->board[to_y][to_x];
      b->board[to_y][to_x]=new piece(red, to_x, to_y, 3);
      break;
    case 'Q':
      delete b->board[to_y][to_x];
      b->board[to_y][to_x]=new piece(red, to_x, to_y, 4);
      break;
    case 'P':
    case 27:
    default:
      break;  
    } 
  }
  io_display(b);
  return 1;
}


int move_rook(board *b, int from_y, int from_x, int to_y, int to_x){
  if(from_y!=to_y && from_x!=to_x){
    //does not end in the same row
    return 0;
  }
  if(from_x-to_x==0){
    if(from_y>to_y){
      for(int i=to_y+1; i<from_y; i++){
	if(b->board[i][from_x]){
	  return 0;
	}
      }
    }
    else{
      for(int i=from_y-1; i>to_y; i--){
	if(b->board[i][from_x]){
	  return 0;
	}
      }
    }
  }
  else if(from_y-to_y==0){
    if(from_x>to_x){
      for(int i=to_x; i<from_x; i++){
	if(b->board[from_y][i]){
	  return 0;
	}
      }
    }
    else{
      for(int i=from_x; i>to_x; i--){
	if(b->board[from_y][i]){
	  return 0;
	}
      }
    }
  }	
  delete b->board[to_y][to_x];			
  b->board[to_y][to_x]=b->board[from_y][from_x];
  b->board[from_y][from_x]->first_move=0;
  b->board[from_y][from_x]=NULL;
  return 1;
}


int move_knight(board *b, int from_y, int from_x, int to_y, int to_x){
  int y_dist=(to_y-from_y)<0 ? from_y-to_y : to_y-from_y;
  int x_dist=(to_x-from_x)<0 ? from_x-to_x : to_x-from_x;
  if(( y_dist==2 && x_dist==1 ) || ( x_dist==2 && y_dist==1 ) ){	       	
    
    delete b->board[to_y][to_x];
    b->board[to_y][to_x]=b->board[from_y][from_x];
    b->board[from_y][from_x]->first_move=0;
    b->board[from_y][from_x]=NULL;
    return 1;
  }
  return 0;
}


int move_bishop(board *b, int from_y, int from_x, int to_y, int to_x){
  int y_dist=(to_y-from_y)<0 ? from_y-to_y : to_y-from_y;
  int x_dist=(to_x-from_x)<0 ? from_x-to_x : to_x-from_x;
  if(x_dist!=y_dist){
    return 0;
  }
 
  if(from_y<to_y){//going down on the board
    if(from_x<to_x){//moving right as well
      for(int i=1; i<y_dist; i++){
	if(b->board[from_y+i][from_x+i]){
	  return 0;
	}
      }
    }
    else{ //going left
      for(int i=1; i<y_dist; i++){
	if(b->board[from_y+i][from_x-i]){
	  return 0;
	}
      }
    }
  }
  else{ //going up on the board
    if(from_x<to_x){//moving right as well
       for(int i=1; i<y_dist; i++){
	if(b->board[from_y-i][from_x+i]){
	  return 0;
	}
      }
    }
    else{ //going left
      for(int i=1; i<y_dist; i++){
	if(b->board[from_y-i][from_x-i]){
	  return 0;
	}
      }
    }
  }

  delete b->board[to_y][to_x];
  b->board[to_y][to_x]=b->board[from_y][from_x];
  b->board[from_y][from_x]->first_move=0;
  b->board[from_y][from_x]=NULL;
  return 1;
}


int move_queen(board *b, int from_y, int from_x, int to_y, int to_x){
  int y_dist=(to_y-from_y)<0 ? from_y-to_y : to_y-from_y;
  int x_dist=(to_x-from_x)<0 ? from_x-to_x : to_x-from_x;
  
   if(x_dist==y_dist){
    return move_bishop(b, from_y, from_x, to_y, to_x);
  }
  if(x_dist==0 && y_dist!=0){
     return move_bishop(b, from_y, from_x, to_y, to_x);
  }
  else if(y_dist==0 && x_dist!=0){
    return move_bishop(b, from_y, from_x, to_y, to_x);
  }
  return 0;
}


int move_king(board *b, int from_y, int from_x, int to_y, int to_x){
  int y_dist=(to_y-from_y)<0 ? from_y-to_y : to_y-from_y;
  int x_dist=(to_x-from_x)<0 ? from_x-to_x : to_x-from_x;
  
  if(y_dist<=1 && x_dist<=1){
    delete b->board[to_y][to_x];
    b->board[to_y][to_x]=b->board[from_y][from_x];
    b->board[from_y][from_x]->first_move=0;
    b->board[from_y][from_x]=NULL;
    return 1; 
  }
  return 0;
}


int move(board *b, int from_y, int from_x, int to_y, int to_x){
  if(from_y<0 ||from_y>7 || from_x<0 || from_x>7 ||
     to_y<0 || to_y>7 || to_x<0 || to_x>7 ||
     (to_y==from_y && to_x==from_y) ||!b->board[from_y][from_x] || 
      (b->board[to_y][to_x] && 
      b->board[from_y][from_x]->color==b->board[to_y][to_x]->color)||
     b->turn % 2!= b->board[from_y][from_x]->color){
    return 0;
  }
  else{
    switch(b->board[from_y][from_x]->type){
    case 0: //pawn
        return move_pawn(b, from_y, from_x, to_y, to_x);
      break;
    case 1: //rook
      return move_rook(b, from_y, from_x, to_y, to_x);
      break;
    case 2: //knight
      return move_knight(b, from_y, from_x, to_y, to_x);
      break;
    case 3: //bishop
      return move_bishop(b, from_y, from_x, to_y, to_x);
      break;
    case 4: //queen
      return move_queen(b, from_y, from_x, to_y, to_x);
      break;
    case 5: //king
      return move_king(b, from_y, from_x, to_y, to_x);
      break;
    default:
      return 0;
    }
  }
  return 0;
}

void io_move(board *b){
  int key;
  int from_x, from_y, to_x, to_y=0;
  io_display(b);
  //slecting the from
  mvprintw(0, 0, 
	   "Please select the Row then Column of the piece you'd like to move"
	   );
  key=getch();
  if(key==27)return;
  from_y=key-'a';
  key=getch();
  if(key==27)return;
  from_x=key-'1';
  do{
  }while( (key=getch()) !='\n');
  mvprintw(0, 0, 
	   "Please select the Row then Column of where you'd like to move    "
	   ); 
  refresh();

  //seletcing the to
  key=getch();
  if(key==27)return;
  to_y=key-'a';
  key=getch();
  if(key==27)return;
  to_x=key-'1';
  do{
  }while( (key=getch()) !='\n');
  io_display(b);
  mvprintw(23, 0, "Move: %c%d to %c%d", from_y+'a', from_x+1, to_y+'a', to_x+1);
  refresh();
  //move piece
  int did_move=move(b, from_y, from_x, to_y, to_x);
  if(did_move==1){
    b->turn++;
    mvprintw(21, 0, "Valid Move  ");
  }
  else{
    mvprintw(21, 0, "Invalid Move");
  }
}

void io_handle_input(board *b){
  int key;
  while(TRUE){
    mvprintw(0, 0, "Press Q to quit or any key to continue                       ");
    key=getch();
    if(key=='Q') break;
    else{
      io_move(b);
      io_display(b);
    }
  }
}

int main(){
  board *game=(board *)malloc(sizeof(board));
  gen_board(game); 
  io_init();
  io_display(game);
  io_handle_input(game);
  io_reset();
  for(int y=0; y<8; y++){
    for(int x=0; x<8; x++){
      if(game->board[y][x]){
	delete game->board[y][x];
      }
    }
  }
  free(game);
  printf("\n\n\n\n\n\t\t\t\t   GAME OVER\n\n\n\n\n");
  return 1;
}
