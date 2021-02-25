
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

    numofThreads = atoi(argv[2]);

    if(numofThreads > THREAD_MAX || numofThreads < 0)
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
void leftRotate(unsigned char *hexValues, unsigned long int fileSize, int numRotations)
{
    unsigned char prev;
    unsigned char next;

    while(numRotations > 0 )
    {
        for(unsigned long int i = 0; i < fileSize; i++)
        {
            prev = ( (hexValues[i] >> 7 ) & 1);
            next = hexValues[i];
            hexValues[i] = (hexValues[i] << 1 | prev ); 
        }

        numRotations--;
    }
    
}
//============================================================
void *threadFunc(void *threadId)
{
    int myid = (int)threadId;

    // check if malloc failed.
    unsigned char* threadKeyFileValues = (unsigned char*) malloc(keyfileSizeInBytes * sizeof(unsigned char));

    //deep copy initialkeyvalues to threadkeyvalues;

    for(int i = 0; i < keyfileSizeInBytes; i++)
    {
        threadKeyFileValues[i] = initialkeyFileValues[i];
    }

    int rotateAmount = (myid + chunkCount) % (keyfileSizeInBytes * 8);

    leftRotate(threadKeyFileValues,keyfileSizeInBytes,rotateAmount);

    int chunkIndex = (myid % numofThreads) * keyfileSizeInBytes;

    for(int i  = 0; i < keyfileSizeInBytes; i++)
    {
        pthread_mutex_lock(&plainTextValuesMtx);
        unsigned char finalVal = threadKeyFileValues[i] ^ plainTextValues[chunkIndex];
        plainTextValues[chunkIndex] = finalVal;
        pthread_mutex_unlock(&plainTextValuesMtx);
        chunkIndex++;
    }

    free(threadKeyFileValues);
}
