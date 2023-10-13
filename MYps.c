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
  char *PID;
} options;

int main(int argc, char *argv[]){

  options.p = 0;
  options.s = 0;
  options.U = 0;
  options.S = 0;
  options.v = 0;
  options.c = 0;
  options.PID = "";

  int flag;

  while ((flag = getopt(argc, argv, "p:sUSvc")) != -1){
    switch (flag){

      case 'p' : options.p = 1;
      options.PID = optarg;
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

  FILE *fp;

  fp = fopen("/proc/11/stat", "r");

  //error checking on open
      if (fp == NULL){
        printf("%s", "Error opening specified files. Exiting program.");
        return 1;
      }

  char line[150];

  while (!feof(fp)){
          fgets(line, 150, fp);
          puts(line);
  }

  fclose(fp);

  printf("options are:\np = %d\nPID = %s\ns = %d\nU = %d\nS = %d\nv = %d\nc = %d\n", options.p, options.PID, options.s, options.U, options.S, options.v, options.c);
  return 0;

}