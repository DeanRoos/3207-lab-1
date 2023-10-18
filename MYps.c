#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>

//functions to easily create filepath to stat, statm, and cmdline files
char *catStat(const char *pid);
char *catStatm(const char *pid);
char *catCmdline(const char *pid);
char *catStatus(const char *pid);

//functions for displaying the various options
void dispCMD(const char *pid);
void dispState(const char *pid);
void dispUtime(const char *pid);
void dispStime(const char *pid);
void dispVmem(const char *pid);
int isDir(struct dirent *dir);
int getUID(const char *pid);

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

  //struct dirent *dir;

  //while ((dir = readdir(fp))) {
    //if (!isDir(dir)){
      //continue;
    //}

  //}

  //printing commands no -p argument

  printf("\nUIDHERE: %d\n", getUID(options.PID));

  char *newName = catStat(options.PID);
  printf("%s", newName);
  newName = catStatm(options.PID);
  printf("%s", newName);
  newName = catCmdline(options.PID);
  printf("%s", newName);
  free(newName);

  printf("%d\n", getuid());

  fclose(fp);

  dispCMD(options.PID);
  puts("");
  dispState(options.PID);
  puts("");
  dispUtime(options.PID);
  puts("");
  dispStime(options.PID);
  puts("");
  dispVmem(options.PID);

  // printf("options are:\np = %d\nPID = %s\ns = %d\nU = %d\nS = %d\nv = %d\nc = %d\n", options.p, options.PID, options.s, options.U, options.S, options.v, options.c);
  // return 0;

}

char *catStat(const char *pid) {
	char *ret = malloc(strlen(pid) + 12);
	strcpy(ret, "/proc/");
  strcat(ret, pid);
  strcat(ret, "/stat\0");
	return ret; //returning
}

char *catStatm(const char *pid) {
	char *ret = malloc(strlen(pid) + 13);
	strcpy(ret, "/proc/");
  strcat(ret, pid);
  strcat(ret, "/statm\0");
	return ret; //returning
}

char *catCmdline(const char *pid) {
	char *ret = malloc(strlen(pid) + 15);
	strcpy(ret, "/proc/");
  strcat(ret, pid);
  strcat(ret, "/cmdline\0");
	return ret; //returning
}

char *catStatus(const char *pid) {
	char *ret = malloc(strlen(pid) + 14);
	strcpy(ret, "/proc/");
  strcat(ret, pid);
  strcat(ret, "/status\0");
	return ret; //returning
}

//function to display the cmdline 
void dispCMD(const char *pid){

  //uses catCmdline funtions to create file path name
  char *path = catCmdline(pid);
  //creating a buffer array to hold content of /proc/<pid>/cmdline
  char buf[1024];

  //reading from cmdline
  printf("%s", "cmdline: ");
  int fp = open(path, O_RDONLY);

  //error checking on open
  if (fp == -1){
    printf("%s", "Error opening specified files. Exiting program.");
    exit(1);
  }

  //getting length of file
  size_t length = read(fp, buf, sizeof (buf));
  //closing file
  close(fp);

  //null-terminating the end of the buffer so they know its the end of a string
  buf[length] = '\0';
  //string variable for each individual null-terminated part of the cmdline file
  char *arg;

  //looping through the file and printing each null-terminated string 
  arg = buf;
  while (arg < buf + length) {
    printf ("%s\n", arg);
    arg += strlen(arg) + 1;
  }

  free(path);

}

void dispState(const char *pid){

  //using catStat function to get file path for stat file
  char *path = catStat(pid);
  //creating a buffer to read the file into
  char buf[1024];

  //reading from stat
  int fp = open(path, O_RDONLY);
  //reading file

  //error checking on open
  if (fp == -1){
    printf("%s", "Error opening specified files. Exiting program.");
    exit(1);
  }

  read(fp, buf, sizeof(buf));
  //closing file
  close(fp);

  //creating a token strings to parse through the stat file 
  char *token = strtok(buf, " ");

  //because the state is always the 3rd, this goes through 3 times
  for (size_t i = 0; i < 2; i++){
    token = (strtok(NULL, " "));
  }

  //prints the token
  printf("State: %s", token);

  free(path);

}

void dispUtime(const char *pid){

  //using catStat function to get file path for stat file
  char *path = catStat(pid);
  //creating a buffer to read the file into
  char buf[1024];

  //reading from stat
  int fp = open(path, O_RDONLY);
  //error checking on open
  if (fp == -1){
    printf("%s", "Error opening specified files. Exiting program.");
    exit(1);
  }

  //reading file
  read(fp, buf, sizeof(buf));
  //closing file
  close(fp);

  //creating a token strings to parse through the stat file 
  char *token = strtok(buf, " ");

  //because the state is always the 3rd, this goes through 3 times
  for (size_t i = 0; i < 13; i++){
    token = (strtok(NULL, " "));
  }

  //prints the token
  printf("Utime: %s", token);

  free(path);

}

void dispStime(const char *pid){

  //using catStat function to get file path for stat file
  char *path = catStat(pid);
  //creating a buffer to read the file into
  char buf[1024];

  //reading from stat
  int fp = open(path, O_RDONLY);
  //error checking on open
  if (fp == -1){
    printf("%s", "Error opening specified files. Exiting program.");
    exit(1);
  }
  //reading file
  read(fp, buf, sizeof(buf));
  //closing file
  close(fp);

  //creating a token strings to parse through the stat file 
  char *token = strtok(buf, " ");

  //because the state is always the 3rd, this goes through 3 times
  for (size_t i = 0; i < 14; i++){
    token = (strtok(NULL, " "));
  }

  //prints the token
  printf("Stime: %s", token);

  free(path);

}

void dispVmem(const char *pid){

  //using catStat function to get file path for stat file
  char *path = catStatm(pid);
  //creating a buffer to read the file into
  char buf[1024];

  //reading from stat
  int fp = open(path, O_RDONLY);
  //error checking on open
  if (fp == -1){
    printf("%s", "Error opening specified files. Exiting program.");
    exit(1);
  }
  //reading file
  read(fp, buf, sizeof(buf));
  //closing file
  close(fp);

  //creating a token strings to parse through the stat file.
  //Because the Virtual memory size is the first thing listed, it will be the first token.
  char *token = strtok(buf, " ");

  //prints the token
  printf("Virtual memory: %s", token);

  free(path);

}

//function to check if the name of a folder in /proc is a number
int isDir(struct dirent *dir) {
  
  char *p;

  for (p = dir->d_name; *p; p++) {
    if (!isdigit(*p))
      return 0;
  }

  return 1;
}

// int getUID(const char *pid){

//   char *path = catStatus(pid);

//   char buf[1024];

//   //reading from status
//   int fp = open(path, O_RDONLY);
//   //error checking on open
//   if (fp == -1){
//     printf("%s", "Error opening specified files. Exiting program.");
//     exit(1);
//   }

//   //reading file
//   read(fp, buf, sizeof(buf));
//   //closing file
//   close(fp);

//   char *uidstr = strstr(buf, "Uid:");

//   char *tok = strtok(uidstr, " ");
//   tok = strtok(NULL, " ");

//   //puts(tok);

//   free(path);

//   return atoi(tok);

// }

int getUID(const char *pid){

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  size_t read;

  char *path = catStatus(pid);

  fp = fopen(path, "r");

  free (path);

  //error checking on open
  if (fp == NULL){
    printf("%s", "Error opening specified files. Exiting program.");
    return 1;
  }

  while ((read = getline(&line, &len, fp)) != -1) {

    if ((strstr(line, "Uid")) != NULL){

      char *token = strtok(line + 4, "	");

      printf("\nUID HERE THIS IS IN THE FUNCTION%s", token);

    }


  }

  free(line);

  return 0;

}