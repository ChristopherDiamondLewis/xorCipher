/*
*   @author Christopher Lewis
*   @date 2/24/2021
*   @brief Header file for XOR encryptor
*
*/

#ifndef ENCRYPT_H
#define ENCRYPT_H


#include <unistd.h>
#include <sys/mman.h> //< for mmap
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define THREAD_MAX 99

unsigned char *initialkeyFileValues;    //<Holds all the orignal keyfile hexvalues.
unsigned char *plainTextValues;         //<Holds current chunk of values being transformed.
unsigned long int keyfileSizeInBytes;   //<Size of keyfile in bytes.

unsigned long bytesReadFromPlain;       //<Number of bytes read in from file being transformed.
unsigned long chunkCount;               //<Current chunk of data that program is working on.
int numofThreads;                       //<Current number of threads requested by command line(not always amount of threads in use).
pthread_mutex_t plainTextValuesMtx;     //<Mutex for blocking conncurrent writes and reads.

FILE *keyfilePtr;                       //<Pointer to our keyfile used for mmap-ping to memory.

/*
*   @brief Function will check arguments passed in from command line
*           for correct argument count, ensure number of threads asked for
*           is within defined range and that the file that will be used for 
*           encrypting (keyfile) is a valid file and does open.
*
*   @param[in] argc, of type int meant to be the number of command line arguments.
*   @param[in] argv, of type array of character ptrs, meant to be vector of passed in arguments.
*   @param[out] integer that will be 1 (EXIT_FAILURE) if failure or 0 (EXIT_SUCCESS) if function found no errors.
*/
        int checkArgs(int argc, char *argv[]);
/*
*   @brief Function will circular left rotate any given array of values a 'numRotations' amount of times.
*               because of the way the rotation is done, the hexvalues will repeat every 'N' times, where
*               'N' is the number of BITS in the hexvalues array, or hexvalues array length times 8.
*           
*   @param[in] hexValues, of type unsigned char pointers, used as array of values that will be rotated.
*   @param[in] fileSize, of type unsigned long int, meant to represent size of file in bytes, used to determine how many bytes we must rotate.
*   @param[in] numRotations, of type int meant to represent how many times the function will rotate the whole array.
*
*/  
        void leftRotate(unsigned char *hexValues, unsigned long int fileSize, int numRotations);

/*
*   @brief Function will return the number of bytes in any given file.
*   @note  Must pass in an already opened file as this function will neither open or close it.
*
*   @param[in] file, of type FILE pointer, used to determine where to start size calculation.
*   @param[out] sizeInBytes, of type unsigned long int, size of passed in file.
*/
        unsigned long int fileSizeInBytes(FILE *file);

/*
*   @brief Function that is passed to each thread to work on a specific chunk of 
*           the file we are attempting to modify(encrypt or decrypt)
*
*   @param[in] threadId, of type int, used to identify thread and for multiple calculations
*                        to determine the threads current working chunk.
*
*   @note Below I have included explanations for some of the 'tricky' variable calculations.
*
*           int rotateAmount: This is the amount that the current thread must rotate 
*                            the given keyfile values. It uses 'myid + chunkCount' to know what exact
*                             number the thread must rotate, but then mods that number with
*                            the sizeofkeyfile * numberofbits in a byte, so it can wrap around.
*                             instead of doing 5410 rotations it will only do 2 rotations (on a one byte keyfile).
*
*           int chunkIndex: This is the index of the current chunk thata current thread will start work on.
*                           For any given chunk that the program is processing each thread has to know what part
*                           of that chunk to work on, this calculation will determine that.
*                        
*   @note It is possible for numOfThreads to not be equal to threads actually being ran.
*           This would be done for optimization purposes.
*
*/                          
        void *threadFunc(void *threadId);

//< END OF ENCRYPT DEFINITION

#endif