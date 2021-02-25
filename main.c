/*
*   @author Christopher Lewis
*
*   @date - 2/22/2021
*
*   @brief XOR cypher.
*
*   @TODD lots of documentation
*   
*/


#include <stdio.h>


#include "encryptUtil.h"


void restoreTestKeyfile(FILE *fileptr);
void restoreTestPlainFile(FILE *fileptr);

int main(int argc, char *argv[])
{

    if(checkArgs(argc,argv) == EXIT_FAILURE)
    {
        return 1;
    }

    
    keyfileSizeInBytes = fileSizeInBytes(keyfilePtr);

    // get file descriptor for keyfile 
    int fileDes = fileno(keyfilePtr);
    // use mmap to map file to process current memory
    void *fileMap = mmap(NULL, keyfileSizeInBytes, PROT_READ|PROT_WRITE, MAP_PRIVATE,fileDes,0);
    // cast file map to array of unsigned chars
    initialkeyFileValues = (unsigned char*)fileMap;





    //allocate 'chunk' for plain text array.
    //TODO CHECK VALUE OF MALLOC
    plainTextValues = (unsigned char*)malloc(sizeof(unsigned char) * keyfileSizeInBytes * numofThreads);
 
    

    pthread_t threadArr[numofThreads];

    // keep reading until bytes read less than 0
    bytesReadFromPlain = fread(plainTextValues,1, keyfileSizeInBytes * numofThreads, stdin);

    while(bytesReadFromPlain > 0 )
    {
        int optimalThreadCount = (bytesReadFromPlain / keyfileSizeInBytes) + (bytesReadFromPlain % keyfileSizeInBytes);
        
        if(numofThreads < optimalThreadCount)
        {
            optimalThreadCount = numofThreads;
        }

        

        for(int i = 0; i < optimalThreadCount  ; i++)
        {
            pthread_create(&threadArr[i],NULL, &threadFunc, (void*)(long)i);
        }

        for(int i = 0; i < optimalThreadCount ; i++)
        {
            pthread_join(threadArr[i], NULL);
        }
        
        // write this result to stdout and process next chunk
        for(int i = 0; i < bytesReadFromPlain; i++)
        {
            fwrite(&plainTextValues[i], sizeof(unsigned char),1,stdout);
           // fflush(stdout);
        }

        chunkCount +=optimalThreadCount;

       bytesReadFromPlain = fread(plainTextValues,1, keyfileSizeInBytes * numofThreads, stdin);
    }

    fclose(keyfilePtr);
    free(plainTextValues);

    return 0;
}
