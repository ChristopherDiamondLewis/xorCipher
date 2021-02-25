
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
void leftRotate(unsigned char *keyFileHexValues, unsigned long int fileSize, int numRotations)
{
    unsigned char prev;
    unsigned char next;

    while(numRotations > 0 )
    {
        for(unsigned long int i = 0; i < fileSize; i++)
        {
            prev = ( (keyFileHexValues[i] >> 7 ) & 1);
            next = keyFileHexValues[i];
            keyFileHexValues[i] = (keyFileHexValues[i] << 1 | prev ); 
            prev = next;
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

    
    int rotateAmount = myid + chunkCount;
    fprintf(stderr,"My id is %d, and i will rotate keyfile %d times\n", myid,rotateAmount);
    
    leftRotate(threadKeyFileValues,keyfileSizeInBytes,rotateAmount );
    // after initial roation, we rotate threadcount times to get next chunk curr thread will use.

    for(int i = 0; i < keyfileSizeInBytes; i++)
    {
        fprintf(stderr,"my thread id is %d and I wouldve worked on %x\n", myid, threadKeyFileValues[i]);
    }
    
    int chunkIndex = (myid % numofThreads) * 2;

    fprintf(stderr,"My id is %d, and my chunk index is %d on the %d chunk\n",myid,chunkIndex,chunkCount);

    for(int i = chunkIndex ; i < bytesReadFromPlain / numofThreads + chunkIndex; i++)
    {
       fprintf(stderr,"my id is %d and I read THIS from plaintext %x\n",myid, plainTextValues[i]);
       
    } 
    
    for(int i  = 0; i < keyfileSizeInBytes; i++)
    {
        unsigned char finalVal = threadKeyFileValues[i] ^ plainTextValues[chunkIndex];
        fprintf(stderr,"My id is %d, the result of %x ^ %x is - %x\n",myid, threadKeyFileValues[i],plainTextValues[chunkIndex],finalVal);
        plainTextValues[chunkIndex] = finalVal;
        fprintf(stderr,"my id is %d, i assigned finalval to plaintext at %d index\n",myid,chunkIndex);
        chunkIndex++;
    }

}