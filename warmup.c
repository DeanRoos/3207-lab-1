#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main(){

  //initializing variables for file and search process
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  size_t read;

  //opening cpuinfo folder
  fp = fopen("/proc/cpuinfo", "r");

  //error checking on open
  if (fp == NULL){
    printf("%s", "Error opening specified files. Exiting program.");
    return 1;
  }

  //parsing lines of text from the file
  while ((read = getline(&line, &len, fp)) != -1) {

    //looking for lines starting with processor (will do this for each processor)
    if ((strstr(line, "processor")) != NULL){

      //tokenizing number of processor
      char *token = strstr(line, ": ");

      //printing the name
      printf("\nProcessor%s", token);

    }

    //finding line starting with cache size
    if ((strstr(line, "cache size")) != NULL){
      
      //tokenizing it
      char *token = strstr(line, ": ");

      //printing it
      printf("Cache size of%s", token);

    }

  }

  //freeing memory
  free(line);

  fclose(fp);

  //opening meminfo in proc
  fp = fopen("/proc/meminfo", "r");

  //error checking
  if (fp == NULL){
    printf("%s", "Error opening specified files. Exiting program.");
    return 1;
  }

  //parsing lines
  while ((read = getline(&line, &len, fp)) != -1) {

    //finding Memfree line
    if ((strstr(line, "MemFree")) != NULL){

      //tokenizing and printing it
      char *token = strstr(line, ": ");
      printf("\nFree memory%s", token);

    }

    //finding Buffer line
    if ((strstr(line, "Buffers")) != NULL){

      //tokenizing and printing it
      char *token = strstr(line, ": ");
      printf("Buffers%s\n", token);

    }

  }
  //closing file
  fclose(fp);

  return 0;

}
