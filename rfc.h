//Saúl Enrique Labra Cruz A01020725
//activity 4 - Rail Fence Cipher

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"
#include "string.h"

#define BUFFER_SIZE 1000

void preparePipes(int in_pipe[], int out_pipe[]);

void closePipes(int in_pipe[], int out_pipe[]);

void encode(int numRails, char * fileName, char *newFileName);

void decode(int numRails, char * fileName, char * newFileName);

void encDec(int in_pipe[], int out_pipe[], char option, char * fileName, int numRails);

void ui();