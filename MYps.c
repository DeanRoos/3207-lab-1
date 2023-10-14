#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

//functions to easily create filepath to stat, statm, and cmdline files
char *catStat(const char *a);
char *catStatm(const char *a);
char *catCmdline(const char *a);

//struct for the flag options from command line
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

  //setting all flags default to 0, 1 would mean that I was selected
  options.p = 0;
  options.s = 0;
  options.U = 0;
  options.S = 0;
  options.v = 0;
  options.c = 0;
  options.PID = "";

  int flag;

  //looping through args tyo find flags
  while ((flag = getopt(argc, argv, "p:sUSvc")) != -1){
    switch (flag){

      case 'p' : options.p = 1;
      //setting pid if -p is used
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
      default:
      printf("%s", "Invalid option. Exiting program.");
      return 1;

    }
  }

  //creating file
  FILE *fp;

  //opting proc folder
  fp = fopen("/proc", "r");

  //error checking on open
      if (fp == NULL){
        printf("%s", "Error opening specified files. Exiting program.");
        return 1;
      }

  //printing commands no -p argument

  char *newName = catStat(options.PID);
  printf("%s", newName);
  newName = catStatm(options.PID);
  printf("%s", newName);
  newName = catCmdline(options.PID);
  printf("%s", newName);
  free(newName);

  printf("%d\n", getuid());

  fclose(fp);

  printf("options are:\np = %d\nPID = %s\ns = %d\nU = %d\nS = %d\nv = %d\nc = %d\n", options.p, options.PID, options.s, options.U, options.S, options.v, options.c);
  return 0;

}

char *catStat(const char *a) {
	char *ret = malloc(strlen(a) + 12);
	strcpy(ret, "/proc/");
  strcat(ret, a);
  strcat(ret, "/stat\0");
	return ret; //returning
}

char *catStatm(const char *a) {
	char *ret = malloc(strlen(a) + 13);
	strcpy(ret, "/proc/");
  strcat(ret, a);
  strcat(ret, "/statm\0");
	return ret; //returning
}

char *catCmdline(const char *a) {
	char *ret = malloc(strlen(a) + 15);
	strcpy(ret, "/proc/");
  strcat(ret, a);
  strcat(ret, "/statm\0");
	return ret; //returning
}