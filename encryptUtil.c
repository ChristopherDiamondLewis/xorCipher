
/*
*   @author Christopher Lewis
*   @date 2/24/2021
*   @brief Implementation file for XOR encryptor
*
*   @TODO documenation
*/
#include "encryptUtil.h"

int checkArgs(int argc, char *argv[])
{
    
    if(argc != 5 )
    {
        fprintf(stderr,"Error amount of arguments!\n");
        fprintf(stderr,"Usage: encryptUtil -n #of threads -k keyfile\n");
        return EXIT_FAILURE;
    }

    int threadCount = atoi(argv[2]);

    if(threadCount > THREAD_MAX || threadCount < 0)
    {
        fprintf(stderr,"Number of threads requested out of range.\n");
        fprintf(stderr,"Thread count range is [%d - %d] inclusive.\n", 1, THREAD_MAX);
        return EXIT_FAILURE;
    }

    keyfilePtr = fopen(argv[4], "r+b");

    if(keyfilePtr == NULL)
    {
        fprintf(stderr,"Error keyfile named ' %s ' did not open!\n", argv[4]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
//============================================================
unsigned long int fileSizeInBytes(FILE *file)
{

    // FILE *currentFile = fopen(filename, "r+b");

    if(file == NULL)
    {
        fprintf(stderr, "ERROR: passed in NULL file pointer cannot calculate size!\n");
        return -1; 
    }

    // fseek to end of file
    fseek(file,0 , SEEK_END);

    // retrieve number of bytes within file.
    unsigned long int sizeInBytes = ftell(file);    

    return sizeInBytes;
}
//============================================================
void leftRotate(unsigned char *keyFileHexValues, unsigned long int fileSize)
{
    unsigned char prev;
    unsigned char next;


    // will left rotate entire file
    for(unsigned long int i = 0; i < fileSize; i++)
    {
        prev = ( (keyFileHexValues[i] >> 7 ) & 1);
        fprintf(stderr,"This is current data before rotate: %x\n", keyFileHexValues[i]);
        next = keyFileHexValues[i];
        keyFileHexValues[i] = (keyFileHexValues[i] << 1 | prev ); 
        prev = next;
        fprintf(stderr, "This is current data after rotate: %x\n", keyFileHexValues[i]);
    }
}