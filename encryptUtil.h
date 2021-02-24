/*
*   @author Christopher Lewis
*   @date 2/24/2021
*   @brief Header file for XOR encryptor
*
*   @TODO documenation
*/

#include <unistd.h>
#include <sys/mman.h> //< for mmap
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define THREAD_MAX 20

FILE *keyfilePtr;
unsigned char *initialkeyFileValues;
unsigned long int keyfileSizeInBytes;

int checkArgs(int argc, char *argv[]);

void leftRotate(unsigned char *keyFileHexValues, unsigned long int fileSize, int numRotations);
unsigned long int fileSizeInBytes(FILE *file);

void *threadFunc(void *threadId);
