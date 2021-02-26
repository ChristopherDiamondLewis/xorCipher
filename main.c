/*
*   @author Christopher Lewis
*
*   @date - 2/22/2021
*
*   @brief This is a 'simple' XOR cypher utility.
*           Essentially given any file this program will transform it to any output stream
*           requested. Because of how encryption works, if you feed the transformed file back 
*           into the program it will pop out the original file. This works on large text, high resolution
*           pictures and so on.
*
*   @note I recommend using the provided makefile but just incase none was provided below is the compile command.
*
*   @compile gcc -Wextra -Wall -pedantic -lpthread -pthread -c encryptUtil.c main.c -o encryptUtil
*
*   Usage: ./encryptUtil -n #numofthreads -k keyfile 
*
*   @note The input to this program is taken from stdin and the result written to stdout.
*
*/


#include <stdio.h>


#include "encryptUtil.h"

int main(int argc, char *argv[])
{

    if(checkArgs(argc,argv) == EXIT_FAILURE)
    {
        return 1;
    }

    //grab the size of the keyile in bytes.
    keyfileSizeInBytes = fileSizeInBytes(keyfilePtr);

    // get file descriptor for keyfile. 
    int fileDes = fileno(keyfilePtr);
    // use mmap to map file to process current memory.
    void *fileMap = mmap(NULL, keyfileSizeInBytes, PROT_READ|PROT_WRITE, MAP_SHARED,fileDes,0);
    // cast file map to array of unsigned chars.
    initialkeyFileValues = (unsigned char*)fileMap;

    // allocate space for largest chunk we could read in from plain text.
    plainTextValues = (unsigned char*)malloc(sizeof(unsigned char) * keyfileSizeInBytes * numofThreads);
    
    if(plainTextValues == NULL)
    {
        fprintf(stderr,"Error unable to get more memory in - %s", __func__);
        return EXIT_FAILURE;
    }
    

    pthread_t threadArr[numofThreads];

    // read full size of plain text array from stdin.
    bytesReadFromPlain = fread(plainTextValues,1, keyfileSizeInBytes * numofThreads, stdin);

    while(bytesReadFromPlain > 0 )
    {
        // This optimization is done so if user request 80 threads for a file 2 bytes long
        // we don't make all 80 threads for no reason, this will determine how many chunks we have read in.
        // and calculate the optimal amount of threads for the current job, rounding up by 1 if given a 'partial' chunk.

        int optimalThreadCount = (bytesReadFromPlain / keyfileSizeInBytes) + (bytesReadFromPlain % keyfileSizeInBytes);
        
        if(numofThreads < optimalThreadCount)   //< if the threads requested by user are less than optimal, we change optimal to 
        {                                       //< threads requested. Again, done to avoid making more threads than needed.
            optimalThreadCount = numofThreads;
        }

        // create our threads and send them to do their chunk of processing.
        for(int i = 0; i < optimalThreadCount  ; i++)
        {
            pthread_create(&threadArr[i],NULL, &threadFunc, (void*)(long)i);
        }
        // wait for all threads to come back, finished with their chunk.
        for(int i = 0; i < optimalThreadCount ; i++)
        {
            pthread_join(threadArr[i], NULL);
        }
        
        // write our result to stdout and process next chunk.
        for(unsigned int i = 0; i < bytesReadFromPlain; i++)
        {
            fwrite(&plainTextValues[i], sizeof(unsigned char),1,stdout);
        }
        // add to chunk count the amount of threads that just did work
        // this is always rounded up by 1 if there are 'partial' chunks.
        chunkCount +=optimalThreadCount;

       // attempt to read in more bytes from our plaintext. 
       bytesReadFromPlain = fread(plainTextValues,1, keyfileSizeInBytes * numofThreads, stdin);
    }

    // Don't forget to free!
    fclose(keyfilePtr);
    free(plainTextValues);

    return 0;
}
