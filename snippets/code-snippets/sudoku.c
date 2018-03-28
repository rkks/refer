/**
 * Written by Tyler Montbriand.  Freeware.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUDOKU_SIZE 9
#define SBIT(X) (1<<(X))

typedef struct sudoku
{
  int overlay[SUDOKU_SIZE][SUDOKU_SIZE];
  int rowmask[SUDOKU_SIZE];
  int colmask[SUDOKU_SIZE];
} sudoku;

int sudoku_solve(sudoku *s);

int main(int arg, char *argv[])
{
  int n;
  sudoku puzzle={
    { {1,2,0, 3,4,0, 5,0,0},
      {6,7,0, 0,0,0, 0,0,0},
      {0,0,0, 0,0,0, 0,0,0},

      {2,0,0, 0,8,0, 7,5,0},
      {0,0,6, 0,5,0, 3,0,0},
      {0,4,1, 0,6,0, 0,0,9},

      {0,0,0, 0,0,0, 0,0,0},
      {0,0,0, 0,0,0, 0,8,4},
      {0,0,9, 0,7,1, 0,6,3} },

    {0,0,0, 0,0,0, 0,0,0},
    {0,0,0, 0,0,0, 0,0,0} };


  if(sudoku_solve(&puzzle)>=0)
  {
    int row,col;
    printf("\aSolved!\n\n");

    for(row=0; row<SUDOKU_SIZE; row++,printf("\n"))
      for(col=0; col<SUDOKU_SIZE; col++)
        printf("\t%d",puzzle.overlay[row][col]);
  }

  return(0);
}

void sudoku_generatemask(sudoku *s);

int sudoku_rc_inc(int *row, int *col)
{
  (*col)++;
  if((*col)>=SUDOKU_SIZE)
  {
    (*row)++;
    (*col)=0;
  }

  return((*row)<SUDOKU_SIZE);
}

void sudoku_set(struct sudoku *s,int r, int c, int val)
{
//  fprintf(stderr,"Setting (%d,%d)=%d\n",r,c,val);
  if(s->rowmask[r] & SBIT(val))
  {
    fprintf(stderr,"Row Collision in (%d,%d)\n",r,c);
    abort();
  }

  if(s->colmask[c] & SBIT(val))
  {
    fprintf(stderr,"Col Collision in (%d,%d)\n",r,c);
    abort();
  }

  s->rowmask[r]|=SBIT(val);
  s->colmask[c]|=SBIT(val);
  s->overlay[r][c]=val;
}

int sudoku_solve_i(sudoku s, int r, int c, sudoku *out)
{
  int v;
  int pvalues;
  if(r>=SUDOKU_SIZE)
  {
    (*out)=s;
    return(0);
  }

  while(s.overlay[r][c])
    if(sudoku_rc_inc(&r,&c)==0) break;

  if(r>=SUDOKU_SIZE)
  {
    (*out)=s;
    return(0);
  }

  for(v=1; v<=SUDOKU_SIZE; v++)
  {
    if(SBIT(v) & (s.rowmask[r] | s.colmask[c])) continue;

    {
      int r2=r;
      int c2=c;
      sudoku s2=s;

      sudoku_set(&s2,r,c,v);
      sudoku_rc_inc(&r2,&c2);
      if(sudoku_solve_i(s2,r2,c2,out)>=0)
        return(0);
    }
  }

  return(-1);
}

int sudoku_solve(struct sudoku *s)
{
  sudoku_generatemask(s);
  return(sudoku_solve_i(*s,0,0,s));
//  return(-1);
}

void sudoku_generatemask(sudoku *s)
{
  int r,c;

  memset(s->rowmask,0,sizeof(s->rowmask));
  memset(s->colmask,0,sizeof(s->colmask));

  for(r=0; r<SUDOKU_SIZE; r++)
  for(c=0; c<SUDOKU_SIZE; c++)
    if(s->overlay[r][c])
      sudoku_set(s,r,c,s->overlay[r][c]);
}
