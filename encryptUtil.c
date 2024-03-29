
/*
*   @author Christopher Lewis
*   @date 2/24/2021
*   @brief Implementation file for XOR encryptor
*
*   @note Please refer to .h file for documenation of 'tricky' calculations.
*/

#include "encryptUtil.h"

//============================================================
int checkArgs(int argc, char *argv[])
{
    
    if(argc != 5 )
    {
        fprintf(stderr,"Error incorrect amount of arguments!\n");
        fprintf(stderr,"Usage: encryptUtil -n #of threads -k keyfile\n");
        return EXIT_FAILURE;
    }

    numofThreads = atoi(argv[2]);

    if(numofThreads > THREAD_MAX || numofThreads <= 0)
    {
        fprintf(stderr,"Number of threads requested out of range.\n");
        fprintf(stderr,"Thread count range is [%d - %d] inclusive.\n", 1, THREAD_MAX);
        return EXIT_FAILURE;
    }

    keyfilePtr = fopen(argv[4], "r+b");

    if(keyfilePtr == NULL)
    {
        fprintf(stderr,"Error: keyfile named ' %s ' did not open!\n", argv[4]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
//============================================================
unsigned long int fileSizeInBytes(FILE *file)
{

    if(file == NULL)
    {
        fprintf(stderr, "ERROR: passed in NULL file pointer cannot calculate size!\n");
        return -1; 
    }

    fseek(file,0 , SEEK_END);

    unsigned long int sizeInBytes = ftell(file);    

    return sizeInBytes;
}
//============================================================
void leftRotate(unsigned char *hexValues, unsigned long int fileSize, int numRotations)
{
    unsigned char curr_msb;
    unsigned char first_bit_in_file;

    while(numRotations > 0 )
    {
        for(unsigned long int i = 0; i < fileSize; i++)
        {
            if(i == 0)
            {
                first_bit_in_file = (hexValues[i] >> 7) & 1;    //< Must capture first bit in file to add to last bit later.
            }
            else if(i == fileSize - 1)
            {
                curr_msb = (hexValues[i] >> 7) & 1;             
                hexValues[i - 1] = (hexValues[i - 1] << 1 | curr_msb);
                hexValues[i] = (hexValues[i] << 1 | first_bit_in_file); //< On final byte in array must add our carry from first bit in array.
            }
            else
            {
                curr_msb = (hexValues[i] >> 7) & 1;             
                hexValues[i - 1] = (hexValues[i - 1] << 1 | curr_msb);
            }
        }

        numRotations--;
    }
    
}
//============================================================
void *threadFunc(void *threadId)
{
    int myid = (int)threadId;

    unsigned char* threadKeyFileValues = (unsigned char*) malloc(keyfileSizeInBytes * sizeof(unsigned char));

    if(threadKeyFileValues == NULL)
    {
        fprintf(stderr,"Error: thread %d was unable to get more memory in - %s", myid,__func__);
        return (void*)EXIT_FAILURE;
    }

    for(unsigned int i = 0; i < keyfileSizeInBytes; i++)
    {
        threadKeyFileValues[i] = initialkeyFileValues[i]; //< deep copy here to avoid changing orignal keyfile.
    }

    int rotateAmount = (myid + chunkCount) % (keyfileSizeInBytes * 8);

    leftRotate(threadKeyFileValues,keyfileSizeInBytes,rotateAmount);

    int chunkIndex = (myid % numofThreads) * keyfileSizeInBytes;

    for(unsigned int i  = 0; i < keyfileSizeInBytes; i++)
    {
        pthread_mutex_lock(&plainTextValuesMtx);
        unsigned char finalVal = threadKeyFileValues[i] ^ plainTextValues[chunkIndex];
        plainTextValues[chunkIndex] = finalVal;
        pthread_mutex_unlock(&plainTextValuesMtx);
        chunkIndex++;
    }

    // Don't forget to free!
    free(threadKeyFileValues);

    return (void*)EXIT_SUCCESS;
}
