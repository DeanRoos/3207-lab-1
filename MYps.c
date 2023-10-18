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

//functions for checking if folder is a process and getting uid of process
int isProc(struct dirent *dir);
int myGetUID(const char *pid);

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

  //looping through args to find flags
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

  //Starting with no -p flag

  if (options.p == 0){

  //creating directory parsing 
  DIR *directory;
  struct dirent *d;
  directory = opendir("/proc");

  //looping through all directories in proc folder
  while ((d = readdir(directory))) {
    //checking to make sure the folder is a proccess using my function
    if (!isProc(d)){
      continue;
    }

    //checking to see if uid matches my uid, if so I print the process id
    if (myGetUID(d->d_name) == getuid()){

      //printing pid
      printf("%s ", d->d_name);

      //checking each option for whether it should display then calling the display
      //function for that option.
      if (options.s == 1){
        dispState(d->d_name);
      }
      if (options.U == 0){
        dispUtime(d->d_name);
      }
      if (options.S == 1){
        dispStime(d->d_name);
      }
      if (options.v == 1){
        dispVmem(d->d_name);
      }
      if (options.c == 0){
        dispCMD(d->d_name);
      }
      puts("");

    }

  }

  //closing directory
  closedir(directory);

  }

  //now if there is -p option

  if (options.p == 1){

    //printing pid
      printf("%s ", options.PID);

      //checking each option for whether it should display then calling the display
      //function for that option.
      if (options.s == 1){
        dispState(options.PID);
      }
      if (options.U == 0){
        dispUtime(options.PID);
      }
      if (options.S == 1){
        dispStime(options.PID);
      }
      if (options.v == 1){
        dispVmem(options.PID);
      }
      if (options.c == 0){
        dispCMD(options.PID);
      }
      puts("");

  }

}

//function to get path to stat file of specific pid
char *catStat(const char *pid) {
	char *ret = malloc(strlen(pid) + 12);
	strcpy(ret, "/proc/");
  strcat(ret, pid);
  strcat(ret, "/stat\0");
	return ret; //returning
}

//function to get path to statm file of specific pid
char *catStatm(const char *pid) {
	char *ret = malloc(strlen(pid) + 13);
	strcpy(ret, "/proc/");
  strcat(ret, pid);
  strcat(ret, "/statm\0");
	return ret; //returning
}

//function to get path to cmdline file of specific pid
char *catCmdline(const char *pid) {
	char *ret = malloc(strlen(pid) + 15);
	strcpy(ret, "/proc/");
  strcat(ret, pid);
  strcat(ret, "/cmdline\0");
	return ret; //returning
}

//functino to get path to status file of specific pid
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

  //oping mcdline file
  int fp = open(path, O_RDONLY);

  //error checking on open
  if (fp == -1){
    printf("%s", "Error opening specified files. Exiting program.");
    exit(1);
  }

  //getting length of file and reading file into buffer
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
    printf ("%s", arg);
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
  printf("%s ", token);

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
  printf("utime=%s ", token);

  free(path);

}

void dispStime(const char *pid){

  //using catStat function to get file path for stat file
  char *path = catStat(pid);
  //creating a buffer to read the file into
  char buf[1024];

  //opening file
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
  printf("stime=%s ", token);

  free(path);

}

void dispVmem(const char *pid){

  //using catStat function to get file path for stat file
  char *path = catStatm(pid);
  //creating a buffer to read the file into
  char buf[1024];

  //opening file
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
  printf("vmem=%s ", token);

  free(path);

}

//function to check if the name of a folder in /proc is a number
int isProc(struct dirent *dir) {
  
  char *p;

  //loops through each character of the d_name and returns false if it finds a non-digit
  for (p = dir->d_name; *p; p++) {
    if (!isdigit(*p))
      return 0;
  }

  return 1;
}

int myGetUID(const char *pid){

  //creating/initializing varabiles for the file, each line we tokenize,
  //the length and for the read function
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  size_t read;

  //getting path to status file using catstatus function
  char *path = catStatus(pid);

  //opting file
  fp = fopen(path, "r");

  //freeing memory
  free (path);

  //error checking on open
  if (fp == NULL){
    printf("%s", "Error opening specified files. Exiting program.");
    exit(1);
  }

  //goes through each line looking for the line starting in Uid:
  //It then tokenizes the uid and returns it as an int
  while ((read = getline(&line, &len, fp)) != -1) {

    if ((strstr(line, "Uid")) != NULL){

      char *token = strtok(line + 4, "	");

      return atoi(token);

    }

  }

  //freeing memory
  free(line);

  //returning -1 on error
  return (-1);

}