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


#define THREAD_MAX 20

FILE *keyfilePtr;

int checkArgs(int argc, char *argv[]);

void leftRotate(unsigned char *keyFileHexValues, unsigned long int fileSize);
unsigned long int fileSizeInBytes(FILE *file);
