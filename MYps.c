#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

struct Flags{
  int p;
  int s;
  int U;
  int S;
  int v;
  int c;
} options;

int main(int argc, char *argv[]){

  options.p = 0;
  options.s = 0;
  options.U = 0;
  options.S = 0;
  options.v = 0;
  options.c = 0;

  int flag;

  while ((flag = getopt(argc, argv, "psUSvc")) != -1){
    switch (flag){

      case 'p' : options.p = 1;
      break;
      case 's' : options.s = 1;
      break;
      case 'U' : options.U = 1;
      break;
      case 'S' : options.S = 1;
      break;
      case 'v' : options.v = 1;
      break;
      case 'c' : options.c = 1;
      break;

    }
  }

  printf("options are:\np = %d\ns = %d\nU = %d\nS = %d\nv = %d\nc = %d", options.p, options.s, options.U, options.S, options.v, options.c);
  return 0;

}
